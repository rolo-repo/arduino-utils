#include "Switch.h"
#include <Constants.h>

namespace arduino
{
    namespace utils
    {
        Switch::~Switch()
        {
        }
        void Switch::doRun()
        {
            if (digitalRead(m_pin) == HIGH)
            {
                if (m_switchState == B_HIGH)
                {
                    return;
                }

                if (!(m_switchState & B_HIGH) && m_stateChageTime > millis() - 50)
                {
                    return;
                }

                m_stateChageTime = millis();

                //if not HIGH
                if (m_switchState & B_LOW)
                {
                    m_switchState = (B_HIGH | B_SWITCH_ON);
                    (*m_on_handler)();
                }
                else
                {
                    m_switchState = B_HIGH;
                }
            }
            else
            {
                if (m_switchState == B_LOW)
                {
                    return;
                }

                if (!(m_switchState & B_LOW) && m_stateChageTime > millis() - 50)
                {
                    return;
                }

                m_stateChageTime = millis();

                if (m_switchState & B_HIGH)
                {
                    m_switchState = B_LOW | B_SWITCH_OFF;
                    (*m_off_handler)();
                }
                else
                {
                    m_switchState = B_LOW;
                }
            }
        }
    }
}
