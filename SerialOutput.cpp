#include "SerialOutput.h"
#include "TimeLib.h"

namespace arduino {
    namespace utils{
        LogItem& LogItem::operator<<( const __FlashStringHelper *i_in )
        {
            return *this << (const String) i_in;
        }
        LogItem& LogItem::operator<<(bool i_in)
        {
            this->m_buffer += (i_in ? "true" : "false");
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(int i_in)
        {
            char buffer[11];

            sprintf(buffer, "%d", i_in);
            this->m_buffer += buffer;
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(long i_in)
        {
            char buffer[19];

            sprintf(buffer, "%ld", i_in);
            this->m_buffer += buffer;
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(short i_in)
        {
            return *this << (int)i_in;
        }

        LogItem& LogItem::operator << (unsigned long i_in)
        {
            char buffer[19];

            sprintf(buffer, "%lu", i_in);
            this->m_buffer += buffer;
            return *this;
        }

        LogItem& LogItem::operator << (unsigned int i_in)
        {
            return *this << (unsigned long)i_in;
        }

        LogItem& LogItem::operator << (unsigned short i_in)
        {
            return *this << (unsigned int)i_in;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(float i_in)
        {
            char buffer[19];

            sprintf(buffer, "%.2f", i_in);
            this->m_buffer += buffer;
            return *this;
        }



        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(const void * i_in)
        {
            if (i_in)
            {
                char buffer[19];

                sprintf(buffer, "0x%LX", i_in);
                this->m_buffer += buffer;
            }

            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(const char* i_in)
        {
            if (i_in)
            {
                this->m_buffer += i_in;
            }
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(const LogItem& i_in)
        {
            this->m_buffer += i_in.m_buffer;
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(const String& i_in)
        {
            return *this << (char*)(i_in.c_str());
        }
        //////////////////////////////////////////////////////////////////////////
        LogItem& LogItem::operator<<(const TimeValue &i_in)
        {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d %02d/%02d/%04d", ::hour(i_in), ::minute(i_in), ::second(i_in), ::day(i_in), ::month(i_in), ::year(i_in));
            this->m_buffer += buffer;

            return *this;
        }
        //////////////////////////////////////////////////////////////////////////
        void SerialOutput::write(const LogItem &i_logItem)
        {
            Serial.println( i_logItem.m_buffer );
        }

    }
}