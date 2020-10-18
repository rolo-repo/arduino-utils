/*
 Name:		MulticastOutput.h
 Created:	26-Aug-18 16:04:00
 Author:	ROMANL
 Editor:	http://www.visualmicro.com
*/

#ifndef _MulticastOutput__
#define _MulticastOutput__

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#if defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "Constants.h"
#include "SerialOutput.h"

#define __F__ FPSTR(__PRETTY_FUNCTION__)

#if defined ENABLE_LOGGER
#undef  LOG_MSG
#define LOG_MSG(...)\
{\
  arduino::utils::LogItem __item__;\
  arduino::utils::SerialOutput::write( __item__<< millis() <<"> " << __F__ <<":["<<__LINE__<<"] " << __VA_ARGS__ );\
  __item__ = arduino::utils::LogItem();\
  arduino::utils::MulticastOutput::write( __item__ <<  arduino::utils::Constants::ID() <<  " " << __F__  << ":[" <<__LINE__<< "] " << __VA_ARGS__ );\
}
#else
#undef  LOG_MSG
#define LOG_MSG(...)\
{\
  arduino::utils::LogItem __item__; \
  arduino::utils::MulticastOutput::write( __item__ << arduino::utils::Constants::ID() << " " << __F__  << ":[" << __LINE__ << "] " << __VA_ARGS__); \
}
#endif

namespace arduino
{
    namespace utils
    {
        class MulticastOutput
        {
        public:
            MulticastOutput( const IPAddress &i_address = IPAddress( 224, 0, 55, 55 ) , unsigned short i_port = 2020 )
            {
                m_multicastIP = i_address ;
                m_port = i_port ;

            }

            static void write(const arduino::utils::LogItem& i_data);

        public:
            bool sendMetric( const arduino::utils::LogItem& i_data )
            {
                if ( WiFi.status () == WL_CONNECTED )
                {
                    if (!m_client.beginPacketMulticast( m_multicastIP, m_port, WiFi.localIP()) )
                        return false;

                    short size = strlen( i_data.c_str() );
                    std::unique_ptr<char[]> buffer( new char[ ( size + sizeof(size) ) ] );
                    
                  //  char * buffer = new char[(size + sizeof(size))];
                    *(reinterpret_cast<short*> ( buffer.get() )) = size;
                    memcpy( buffer.get() + sizeof(size), i_data.c_str(), size);

                    if (!m_client.write(buffer.get(), size + sizeof(size)))
                        return false;

                    if (!m_client.endPacket())
                        return false;
                }
            }

        private:
            WiFiUDP m_client;
            
            IPAddress m_multicastIP;   
            unsigned short m_port;
        };
    }        
}

#else
#warning "Incorrect platform. Only ESP8266 MCUs are valid."
#include "SerialOutput.h"
#endif // NETWORK_TYPE

#endif //_MulticastOutput__

