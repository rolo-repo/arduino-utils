#ifndef _Constants_h
#define _Constants_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define LED_ON LOW
#define LED_OFF ~LOW

#define PIN unsigned int

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
#endif

        }
      };
  }
}

#endif