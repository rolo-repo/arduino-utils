#ifndef _TimeManager_h
#define _TimeManager_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32 || defined ESP8266
#include <NtpClientLib.h>

#ifdef min
#undef min
#endif

namespace arduino {
    namespace utils {
    	

        class TimeManager
        {
        public:

            TimeManager() :m_lastMillis(0), m_seconds(0), m_ok(false){}

            bool begin( short i_GMT_OffsetHours = 2, short i_GMT_OffsetMinutes = 0 );
            void run(bool i_forceSync = false);
            
            long getEpochTime() const
            {
                return  m_seconds;
            }
            
            long getEpochDate() const;

            const char * getTimeDateStr( unsigned long i_seconds = 0 ) const;

            const char * getShortTimeDateStr( unsigned long i_seconds = 0 ) const;
 
            const char* getTimeStr( unsigned long i_seconds = 0 ) const;

            const char* getDateStr( unsigned long i_seconds = 0 )  const;

			const char* getDateStrYYYY_MM_DD(unsigned long i_seconds = 0)  const;

            bool synced() const;

            short hour() const
            {
                return ::hour(m_seconds);
            }

            short min() const
            {
                return ::minute(m_seconds);
            }

        private:
            unsigned long m_seconds;
            unsigned long m_lastMillis;

            mutable char m_buffer[20];
            bool m_ok;
        };   
    }
}

extern  arduino::utils::TimeManager  TIME;
#endif //ARDUINO_ARCH_ESP8266

#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32 
#include <functional>
typedef std::function<void(long&)> Handler;
#else
typedef    void(*Handler)(long&);
#endif

#include "Pair.h"

namespace arduino
{
    namespace utils
    {
        class Timer
        {
        public:

            enum  Interval
            {
                MINUTE = 60,
                HOUR = MINUTE * 60,
                DAY = HOUR * 24,
                WEEK = DAY * 7
            };

            Timer ( const char* i_name = "UNKNOWN" ) : m_tasksCount(0), m_name(i_name){}

            Timer( long i_time, Handler i_handler , const char* i_name = "UNKNOWN")
            {
                m_tasksCount = 0;
                m_name = i_name;
                addTask(i_time, i_handler);
            }

            Timer( const Timer &i_timer )
            { 
                m_tasksCount = 0;

                for ( int i = 0; i < i_timer.m_tasksCount; i++ )
                {
                    addTask( i_timer.m_tasks[i].first, i_timer.m_tasks[i].second );
                }
            }

            Timer& operator=( const Timer& i_timer)
            {
                m_tasksCount = 0;

                for (int i = 0; i < i_timer.m_tasksCount; i++)
                {
                    addTask( i_timer.m_tasks[i].first, i_timer.m_tasks[i].second );
                }

                return *this;
            }
            
            Timer& operator+=( const Timer& i_timer)
            {
                for (int i = 0; i < i_timer.m_tasksCount; i++)
                {
                    addTask( i_timer.m_tasks[i].first, i_timer.m_tasks[i].second );
                }

                return *this;
            }

            bool addTask( long , Handler );
            bool addRecuringTask( long , long , Handler );
            ////////////////////////////////////
            void run();
 
        private:
            static const short m_max = 10;
            Pair< long, Handler > m_tasks[m_max];

            short  m_tasksCount;
            String m_name;
        };

    }
}

#endif //_TimeManager_h