#include "TimeManager.h"
#if defined ARDUINO_ARCH_ESP8266

#include <TimeLib.h>

#define ENABLE_LOGGER
#include <SerialOutput.h>
#include <Constants.h>
arduino::utils::TimeManager  TIME;
#define NTP_SYNC_PERIOD_SEC 60 * 60 

namespace arduino {
    namespace utils {

        long TimeManager::getEpochDate() const
        {
            return  m_seconds - ( ::hour( m_seconds ) * 60 * 60 + ::minute( m_seconds ) * 60 + second( m_seconds ));
        }

        const char * TimeManager::getTimeDateStr( unsigned long i_seconds ) const
        {
            unsigned long seconds = (i_seconds) ? i_seconds : m_seconds;
           
            memset(m_buffer, 0x0, sizeof(m_buffer));
            snprintf(m_buffer, sizeof(m_buffer), "%02d:%02d:%02d %02d/%02d/%04d", ::hour(seconds), ::minute(seconds), ::second(seconds), ::day(seconds), ::month(seconds), ::year(seconds));
            return m_buffer;
        }

        const char * TimeManager::getShortTimeDateStr(unsigned long i_seconds) const
        {
            unsigned long seconds = (i_seconds) ? i_seconds : m_seconds;

            memset(m_buffer, 0x0, sizeof(m_buffer));
            snprintf(m_buffer, sizeof(m_buffer), "%02d:%02d %02d/%02d", ::hour(seconds), ::minute(seconds),  ::day(seconds), ::month(seconds));
            return m_buffer;
        }

        const char* TimeManager::getTimeStr( unsigned long i_seconds ) const
        {
            unsigned long seconds = (i_seconds) ? i_seconds : m_seconds;

            memset(m_buffer, 0x0, sizeof(m_buffer));
            snprintf(m_buffer, sizeof(m_buffer), "%02d:%02d:%02d", ::hour(seconds), ::minute(seconds), ::second(seconds));
            return m_buffer;
        }

        const char* TimeManager::getDateStr( unsigned long i_seconds )  const
        {
            unsigned long seconds = (i_seconds) ? i_seconds : m_seconds;

            memset(m_buffer, 0x0, sizeof(m_buffer));
            snprintf(m_buffer, sizeof(m_buffer), "%02d/%02d/%04d", ::day(seconds), ::month(seconds), ::year(seconds));
            return m_buffer;
        }

        bool TimeManager::begin(short i_GMT_OffsetHours, short i_GMT_OffsetMinutes)
        {
            if (!m_ok)
            {
                LOG_MSG("Going to connect to NTP server on pool.ntp.org" << " GMT OFFSET " << i_GMT_OffsetHours << " " << i_GMT_OffsetMinutes);

                m_ok = NTP.begin( "pool.ntp.org", i_GMT_OffsetHours, true, i_GMT_OffsetMinutes );

                if ( m_ok )
                {
                    NTP.onNTPSyncEvent([this](NTPSyncEvent_t i_event)
                    {
                        LOG_MSG("Synchronization trigered");

                        if ( timeSyncd == i_event )
                        {
                            this->m_seconds = ( ! this->synced() ||  abs ( NTP.getLastNTPSync() - m_seconds < NTP_SYNC_PERIOD_SEC  ) ) ? NTP.getLastNTPSync() : this->m_seconds;

                            LOG_MSG("Synchronization done , new time " << this->m_seconds << " seconds ");
                        }
                        else
                        {
                            LOG_MSG("!ERROR! Failed to synchronize with NTP server");
                        }
                    });
                }
                
                LOG_MSG("TimeManager is " << ((m_ok) ? "ready" : "not ready"));
            }
            return m_ok;
        }

        void TimeManager::run( bool i_forceSync )
        {
            unsigned long l_millis = millis();
            
            if ( l_millis < m_lastMillis )
                m_lastMillis = l_millis;

            if ( l_millis >= m_lastMillis + 1000 )
            {
                m_seconds += ( l_millis - m_lastMillis ) / 1000; 
                m_lastMillis = l_millis;
            }

            if ( m_ok && ( i_forceSync || m_seconds < 0x1000000L || m_seconds - NTP.getLastNTPSync() > NTP_SYNC_PERIOD_SEC ) )
            {
                LOG_MSG("Sync time i_forceSync " << i_forceSync << " m_seconds " << m_seconds);

                // 0 in case of error
                NTP.getTime();
            }
        }

        bool TimeManager::synced()  const
        {
            return (m_seconds > 0x1000000L);
        }

   /////////////////////////////////////////////////////////////////

        const short Timer::m_max;

        void Timer::run()
        {
            for ( int i = 0; i < m_tasksCount; i++ )
            {
                if (TIME.getEpochTime() >= m_tasks[i].first)
                {
                    LOG_MSG( m_name << " Executing task [" << i + 1 <<"] scheduled to " << (TimeValue) m_tasks[i].first);
                    m_tasks[i].first = TIME.getEpochTime();
                    (m_tasks[i].second)(m_tasks[i].first);
                 
                    if( m_tasks[i].first <= TIME.getEpochTime() )
                    {
                        LOG_MSG( m_name << " Task [" << i + 1 << "] "  << m_tasks[i].first << "] is not resceduled deleting it");

                        //delete the task ,if only 1 only decrease the counter to 0
                        m_tasksCount--;
                        for ( int j = i ; j < m_tasksCount ; j++ )
                        {
                            m_tasks[j] = m_tasks[j + 1];
                        }    
                    }
                    else
                    {
                        LOG_MSG(m_name << " Task [" << i + 1 << "] rescheduled to " << (TimeValue)m_tasks[i].first);
                    }
                }
            }
        }

        bool Timer::addTask( long i_time, Handler i_handler )
        {
            LOG_MSG( m_name << " adding task sceduled to " << (TimeValue) i_time );
            if ( m_tasksCount < m_max )
            {
                m_tasks[m_tasksCount] = std::make_pair(i_time, i_handler);
                m_tasksCount++;
                LOG_MSG( m_name << " adding task - left places " << m_max - m_tasksCount);

                return true;
            }
           /* else
            {
                LOG_MSG( m_name << " adding task , trying to free space");

                for (int i = 1; i < m_max; i++)
                {
                    m_tasks[i - 1] = m_tasks[i];
                }
                m_tasks[m_max - 1] = std::make_pair(i_time, i_handler);

               
            }     */

            return false;
           
        }
    
        bool Timer::addRecuringTask( long i_startTime , long i_interval , Handler i_function )
        {
            return addTask(i_startTime, [=](long &o_time)
            {
                i_function(o_time);
                o_time += i_interval;
            });
        }


    }
}
#endif