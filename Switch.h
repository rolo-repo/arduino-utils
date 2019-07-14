#ifndef _Switch_h
#define _Switch_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

typedef    void(*OnHandler)();
typedef    void(*OffHandler)();

namespace arduino {
    namespace utils {
        class Switch
        {
            enum State 
            {
                B_HIGH = 2,          //000001
                B_SWITCH_ON = 4,     //000010
                B_SWITCH_OFF = 8,    //000100
                B_LOW = 16           //001000
            };

        public:
            Switch( uint8_t i_pin,
                OnHandler  i_on_handler = &Switch::emptyHandler,
                OffHandler i_off_handler = &Switch::emptyHandler )
                {
                	m_pin = i_pin; 
                	m_on_handler  = i_on_handler;
               	    m_off_handler = i_off_handler ;
                	m_stateChageTime =  millis();
                	m_switchState = B_LOW;
                }

            ~Switch();

            void run()
            {
                doRun();
            }

            bool getState() const  {  return ( m_switchState & ( B_HIGH | B_SWITCH_ON ) ); }

        private :
            void doRun();

            static void emptyHandler() {}

        private:
            uint8_t m_pin;
            unsigned long   m_stateChageTime;
            int   m_switchState;

            OnHandler   m_on_handler;
            OffHandler  m_off_handler;
        };
    }
}

typedef  arduino::utils::Switch  Switch_t;
#endif

