#ifndef OTA_h__
#define OTA_h__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h> 
#endif

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#endif

#include "ArduinoOTA.h"

#include "Constants.h"
#include "PlaceHolder.h"
#include "MulticastOutput.h"
#include "TimeManager.h"

namespace arduino {
	namespace utils {

		const char __ota_success_html[] PROGMEM = R"rawliteral(
<br>
<h1 style="text-align:center;">Ready for update : ${buildId} date ${date}</h1>
<br>
)rawliteral";

#ifdef ARDUINO_ARCH_ESP8266
		ESP8266WebServer
#else
		WebServer
#endif 
		oTAWebServer(8080);

		arduino::utils::Timer oTAtimer("TimeOutTimer");
		volatile bool __updateSketch__ = false;

		void OTA_Init(unsigned long i_version = 0)
		{
			ArduinoOTA.onStart([]() {LOG_MSG("Update started"); });
			ArduinoOTA.onEnd([]() {
				LOG_MSG("Update ended");
				__updateSketch__ = false;
				oTAtimer = arduino::utils::Timer("TimeOutTimer");
			});
			ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
				LOG_MSG("Progress:" << (progress / (total / 100)));
			});

			ArduinoOTA.onError([](ota_error_t error) {
				__updateSketch__ = false;
				LOG_MSG("Error" << error);
				switch (error)
				{
				case OTA_AUTH_ERROR:
					LOG_MSG("Auth Failed");
					break;
				case OTA_BEGIN_ERROR:
					LOG_MSG("Begin Failed");
					break;
				case OTA_RECEIVE_ERROR:
					LOG_MSG("Receive Failed");
					break;
				case OTA_END_ERROR:
					LOG_MSG("End Failed");
					break;
				case OTA_CONNECT_ERROR:
					LOG_MSG("Connect Failed");
					break;
				default:
					LOG_MSG("Uknown error");
					break;
				}
			});
			// Port defaults to 8266
			//ArduinoOTA.setPort(8266);
			// Hostname defaults to esp8266-[ChipID]
			//ArduinoOTA.setHostname("myesp8266");
			// No authentication by default
			//ArduinoOTA.setPassword((const char *)"xxxxx");
			
			ArduinoOTA.setPort(8000); //need to defiend in board.txt
			
			ArduinoOTA.begin();
			oTAWebServer.begin();

			oTAWebServer.on("/update", [i_version]() {
				using namespace arduino::utils;
				static PlaceHolder a("buildId", String(i_version));
				static PlaceHolder b("date", String(__DATE__));

				LOG_MSG("Got update request");
				oTAWebServer.send(200, "text/html", PlaceHolder::tr(PlaceHolder::makeList(VA_LIST(&a, &b)), FPSTR(__ota_success_html)));
				__updateSketch__ = true;

				oTAtimer.addTask(TIME.getEpochTime() + 20,
					[](long&) {
					__updateSketch__ = false;
					LOG_MSG("Update failed due to timeout");
				});

			});

			oTAWebServer.on("/version", [i_version]() {
				oTAWebServer.send(200, "text/html", String(i_version) + '_' + TIME.getShortTimeDateStr(i_version));
			});
		}

	}
}


#define OTA_CHECK_AND_UPDATE { if ( arduino::utils::__updateSketch__ ) { ArduinoOTA.handle(); TIME.run(); arduino::utils::oTAtimer.run(); return; } else {  arduino::utils::oTAWebServer.handleClient(); }}


#endif // OTA_h__