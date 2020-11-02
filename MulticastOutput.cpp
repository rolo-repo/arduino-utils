#include "MulticastOutput.h"

namespace arduino
{
    namespace utils
    {   
#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
        void MulticastOutput::write ( const arduino::utils::LogItem& i_data )
        {
             static MulticastOutput output;
             output.sendMetric(i_data);
        }
#endif
    }
}
