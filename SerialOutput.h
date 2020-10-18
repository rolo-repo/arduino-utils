/*
Name:		SerialOutput.h
Created:	26-Aug-18 16:04:00
Author:	ROMANL
Editor:	http://www.visualmicro.com
*/

#ifndef _SerialOutput_h
#define _SerialOutput_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "TimeLib.h"

#define __F__ FPSTR(__PRETTY_FUNCTION__)

#ifdef ENABLE_LOGGER
#define LOG_MSG_BEGIN(BIT_RATE)  Serial.begin(BIT_RATE)
#if defined  __ESP8266_ESP8266__ || defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_wifi_kit_8 || defined ARDUINO_ESP8266_WEMOS_D1MINI 
#define LOG_MSG(...)\
{\
  arduino::utils::LogItem __item__;\
  arduino::utils::SerialOutput::write( __item__<< millis() << "> " << __F__ << ":["<<__LINE__<<"] "<<__VA_ARGS__ );\
}
#else
#define LOG_MSG(...)\
{\
  arduino::utils::LogItem __item__;\
  arduino::utils::SerialOutput::write( __item__<< millis() << F("> ") << __F__ << F(":[")<<__LINE__<<F("] ")<<__VA_ARGS__ );\
}
#endif
#else
#define LOG_MSG(...)
#define LOG_MSG_BEGIN(...)
#endif


namespace arduino
{
    namespace utils
    {
        class TimeValue 
        {
        public:
            TimeValue ( unsigned long i_value ) : m_value ( i_value ){}
            operator time_t() const { return (time_t)m_value; }
        private:
            unsigned long m_value;
        };

        class LogItem
        {
            friend class SerialOutput;

        public :
#ifndef  __ESP8266_ESP8266__
            LogItem& operator<<(const __FlashStringHelper *);
#endif
            LogItem& operator<<(bool i_in);
            LogItem& operator<<(short i_in);
            LogItem& operator<<(int i_in);
            LogItem& operator<<(long i_in);
            LogItem& operator<<(unsigned int i_in);
            LogItem& operator<<(unsigned long i_in);
            LogItem& operator<<(unsigned short i_in);
            LogItem& operator<<(float i_in);
            LogItem& operator<<(const char* i_in);
            LogItem& operator<<(const LogItem& i_in);
            LogItem& operator<<(const void * i_in);
            LogItem& operator<<(const String &i_in);
            LogItem& operator<<(const TimeValue &i_in);
            const char* c_str() const { return m_buffer.c_str(); }

        private:
            String m_buffer;
        };

        class SerialOutput
        {
        public:
            static void write( const LogItem &i_logItem );
        };
    }
}
#endif