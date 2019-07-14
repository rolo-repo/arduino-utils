#include <Button.h>

#include <SerialOutput.h>
#include <Constants.h>

namespace arduino
{
    namespace utils
    {
        //////////////////////////////////////////////////////////////////////////////////////
        void Button::onLongClick()
        {
            LOG_MSG(F("LongClick"));
        }
        void Button::onSingleClick()
        {
            LOG_MSG(F("SingleClick"));
        }
        void Button::onDoubleClick()
        {
            LOG_MSG(F("DoubleClick"));
        }
        //////////////////////////////////////////////////////////////////////////////////////
        bool Button::doRun()
        {
            bool r = false;
            if (digitalRead(m_pin) == HIGH)
            {
                if (!(m_buttonState & B_HIGH) && m_stateChageTime > millis() - 50)
                {
                    return false;
                }

                digitalWrite(LED_BUILTIN, LED_ON);
                m_stateChageTime = millis();

                //if not HIGH
                if (!(m_buttonState & B_HIGH))
                {
                    if (m_buttonState & B_RELEASED)
                    {
                        m_buttonState = (B_HIGH | B_DOUBLE_PRESSED);
                    }
                    else
                    {
                        m_buttonState = (B_HIGH | B_PRESSED);
                    }
                    m_numberOfClicks++;
                    m_lastPressedTime = millis();
                }
                else
                {
                    if (m_buttonState & B_PRESSED && m_lastPressedTime + m_longClickTimer < millis())
                    {
                        m_buttonState |= B_LONG_PRESSED;
                        m_lastPressedTime = millis();
                        (*m_lc_handler)();
                        r = true;
                    }
                }
            }
            else
            {
                if (m_buttonState == B_LOW)
                {
                    return false;
                }

                if (!(m_buttonState & B_LOW) && m_stateChageTime > millis() - 50)
                {
                    return false;
                }

                digitalWrite(LED_BUILTIN, LED_OFF);
                m_stateChageTime = millis();

                if (m_buttonState & B_DOUBLE_PRESSED)
                {
                    m_buttonState = B_LOW;
                    (*m_dc_handler)();
                    r = true;
                }

                if (m_buttonState & B_LONG_PRESSED)
                {
                    m_buttonState = B_LOW;
                }

                if (m_buttonState & B_PRESSED)
                {
                    m_buttonState = (B_LOW | B_RELEASED);
                }

                if (m_buttonState & B_RELEASED && m_lastPressedTime + m_doubleClickTimer < millis())
                {
                    m_buttonState = B_LOW;
                    (*m_sc_handler)();
                    r = true;
                }

                if (!(m_buttonState & B_LOW))
                {
                    m_buttonState = B_LOW;
                }
            }
            return r;
        }
    }//utils
}//arduino