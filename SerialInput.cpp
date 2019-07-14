/*
 Name:		SerialProxy.cpp
 Created:	26-Aug-18 16:04:00
 Author:	ROMANL
 Editor:	http://www.visualmicro.com
*/

#include "SerialInput.h"

namespace arduino
{
    namespace utils
    {
        bool SerialInput::get(char &o_value)
        {
            o_value = m_buffer[0];
            return true;
        }

        bool SerialInput::get(short &o_value)
        {
            // m_buffer[m_length] = 0x0;
            // o_value = atoi(m_buffer);

            return sscanf(m_buffer, "%hd", &o_value);
        }

        bool SerialInput::get(unsigned short &o_value)
        {
            // m_buffer[m_length] = 0x0;
            // o_value = atoi(m_buffer);

            return sscanf(m_buffer, "%hu", &o_value);
        }

        bool SerialInput::get(int &o_value)
        {
            // m_buffer[m_length] = 0x0;
            // o_value = atoi(m_buffer);

            return sscanf(m_buffer, "%d", &o_value);

        }

        bool SerialInput::get(unsigned int &o_value)
        {
            // m_buffer[m_length] = 0x0;
            // o_value = atoi(m_buffer);

            return sscanf(m_buffer, "%u", &o_value);

        }

        bool SerialInput::get(long &o_value)
        {
            return sscanf(m_buffer, "%ld", &o_value);
        }

        bool SerialInput::get(unsigned long &o_value)
        {
            return sscanf(m_buffer, "%lu", &o_value);
        }

        bool SerialInput::get(String &o_value)
        {
            o_value = String(m_buffer);

            return true;
        }
    }
}

