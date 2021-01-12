#ifndef _Constants_h
#define _Constants_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define LED_ON LOW
#define LED_OFF ~LOW

typedef unsigned int PIN;
typedef uint32_t uint32;

#ifndef  LED_BUILTIN
#define LED_BUILTIN 4
#endif // ! LED_BUILTIN


#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32 || defined ESP8266
#define RESET() ESP.restart();
#else
//void(*RESET) (void) = 0;
#endif

namespace arduino {
    namespace utils {
      class Constants
      {
      public:
        static constexpr char* const DATE_TIME_14 = "%02d/%02d/%02d %02d:%02d";
        static constexpr char* const DATE_TIME_12 = "%02d/%02d %02d:%02d";
        static const short RF_TIMEOUT_MS = 1000;
        static String ID()
        {
#if   defined APP_ID
            return String(APP_ID);
#elif defined ARDUINO_ARCH_ESP8266
            return String( ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
			return String((unsigned long) ESP.getEfuseMac());
#else
			return "NO_ID";
#endif

        }
      };

	  template<typename ...Args>
	  constexpr size_t va_count(Args&&...) { return sizeof...(Args); }
    
	
	 template <typename T, uint32 N> char( &size_of_array_helper(T(&)[N]) )[N];
	}
}

#define NON_DERIVABLE_BASE(ACLASS)\
    class ACLASS;\
    class DerivedBlocker##ACLASS\
    {\
    private:\
    friend class ACLASS;\
    DerivedBlocker##ACLASS() {}\
};
#define NON_DERIVABLE(ACLASS) virtual public DerivedBlocker##ACLASS

#define VA_LIST(...) arduino::utils::va_count(__VA_ARGS__),__VA_ARGS__

//#define SIZE_OF_ARR(x) (sizeof(x) / sizeof((x)[0])) //this is not type safe

#define SIZE_OF_ARR(x) (sizeof(arduino::utils::size_of_array_helper(x)))

#endif