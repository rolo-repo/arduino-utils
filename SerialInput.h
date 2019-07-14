/*
 Name:		SerialProxy.h
 Created:	26-Aug-18 16:04:00
 Author:	ROMANL
 Editor:	http://www.visualmicro.com
*/

#ifndef _SerialInput_h
#define _SerialInput_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
namespace arduino
{
    namespace utils
    {
        template<class T> struct Result
        {
            T m_result;
            bool m_ok;
            Result() :m_ok(false){}
        };

        class SerialInput
        {
        public:
            SerialInput(short i_timeout = 50) :m_timeout(i_timeout), m_length(0){}
            template <typename T> Result<T> read();
        private:
            bool get(short &o_value);
            bool get(unsigned short &o_value);
            bool get(int &o_value);
            bool get(unsigned int &o_value);
            bool get(long &o_value);
            bool get(unsigned long &o_value);
            bool get(char &o_value);
            bool get(String &o_value);

            char m_buffer[32];
            char m_padding;//to avoid memory overflow

            unsigned long m_exp_ms;
            const short m_timeout;
            short m_length;
        };

        template <typename T>
        Result<T> SerialInput::read()
        {
            bool done = false;
            Result<T> result;

            while (Serial.available() > 0)
            {
                if (0 == m_length)
                {
                    m_exp_ms = m_timeout + millis();
                }

                if ( m_length < sizeof(m_buffer) - 1)
                {
                    m_buffer[m_length] = Serial.read();
                    ++m_length;
                }
            }

            if ((m_exp_ms < millis() && m_length > 0) || m_length == sizeof(m_buffer))
            {
                m_buffer[sizeof(m_buffer) - 1] = 0x0;
                result.m_ok = this->get( result.m_result );
                memset(m_buffer, 0x0, sizeof(m_buffer));
                m_length = 0;
            }

            return result;
        }
    }
}

typedef arduino::utils::SerialInput InputReader_t;


#endif //_SerialInput_h

