#ifndef _Button_h
#define _Button_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


typedef    void(*LongClickHandler)();
typedef    void(*SingleClickHandler)();
typedef    void(*DoubleClickHandler)();

namespace arduino {
    namespace utils {
        class Button
        {
            enum State{
                B_HIGH = 2,             //000001
                B_PRESSED = 4,          //000010
                B_RELEASED = 8,         //000100
                B_LONG_PRESSED = 16,    //001000
                B_DOUBLE_PRESSED = 32,  //010000
                B_LOW = 64              //100000
            };           
        public:
            Button(uint8_t i_pin,
                SingleClickHandler i_sc_handler = &Button::onSingleClick,
                DoubleClickHandler i_dc_handler = &Button::onDoubleClick,
                LongClickHandler   i_lc_handler = &Button::onLongClick
                ) :
                m_pin(i_pin),
                m_lc_handler(i_lc_handler),
                m_sc_handler(i_sc_handler),
                m_dc_handler(i_dc_handler)
            {
                m_doubleClickTimer = 400;
                m_longClickTimer = 2000;
                m_buttonState = ( digitalRead(m_pin) == HIGH ) ? B_HIGH : B_LOW;
                m_lastPressedTime = millis();
                m_stateChageTime = m_lastPressedTime;
            }

            int getState() const { return m_buttonState; }
            int getClicks() const { return m_numberOfClicks; }
            bool run()
            {
                return doRun();
            }

        private:
            bool doRun();
            static void onLongClick();
            static void onSingleClick();
            static void onDoubleClick();

            //////////////////////////////////////////////
            uint8_t m_pin;
            short m_longClickTimer;
            short m_doubleClickTimer;
            unsigned long   m_lastPressedTime;
            unsigned long   m_stateChageTime;
            int   m_buttonState;
            int   m_numberOfClicks;

            LongClickHandler   m_lc_handler;
            SingleClickHandler m_sc_handler;
            DoubleClickHandler m_dc_handler;
        };     
    }
}

typedef    arduino::utils::Button  Button_t;

#endif