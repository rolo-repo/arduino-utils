#ifndef _EEPROM_Adapter_h
#define _EEPROM_Adapter_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "EEPROM.h"
#include "BufferAndSize.h"
#include "String.h"

#ifndef E2SIZE

#define E2SIZE 512 //default

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#define E2SIZE 4096
#endif

#ifdef  ARDUINO_AVR_NANO
#define E2SIZE 512
#endif 

#endif


namespace arduino {
    namespace utils {
		class EEPROM_Adapter
		{
		private:
			const static uint8_t POISON;
		public:

			static void begin( size_t size = E2SIZE) { 
				#if defined ARDUINO_ARCH_ESP8266  || defined  ARDUINO_ESP8266_WEMOS_D1MINI
					EEPROM.begin(E2SIZE);
				#endif
				
				#ifdef ARDUINO_ARCH_AVR
					EEPROM.begin();
				#endif // ARDUINO_ARCH_AVR

			}
			static void clean(size_t size = E2SIZE) {
				for (auto i = 0; i < size; i++) { EEPROM.put(i, 0x0); } 

#ifdef ARDUINO_ARCH_ESP8266
				EEPROM.commit();
#endif			
			}

			template <class T>
			static bool load( T &data )
			{
				uint16_t indx = 0;
				return load( data, indx);
			}

			template <class T >
			static uint16_t save( const T &i_data, uint16_t i_idx = 0 )
			{
				uint16_t startIdx = i_idx;
				EEPROM.put(i_idx++, POISON);
				EEPROM.put(i_idx++, (uint8_t)sizeof(T));

				const uint8_t *t = (const uint8_t*)&i_data;

				for (auto i = 0; i < sizeof(T); i++)
				{
					EEPROM.put(i_idx++, *(t + i));
				}

#ifdef ARDUINO_ARCH_ESP8266
				return (EEPROM.commit()) ? i_idx : startIdx;
#else
				return i_idx;
#endif		
			}

			template <class T >
			static bool load( T &i_data, uint16_t &i_idx )
			{
				if (POISON == EEPROM.read(i_idx++))
				{
					uint8_t size = EEPROM.read(i_idx++);

					if (size <= sizeof(T))
					{
						memset(&i_data, 0x0, sizeof(T));

						uint8_t *t = (uint8_t*)&i_data;
						for (auto i = 0; i < size; i++)
						{
							*(t + i) = EEPROM.read(i_idx++);
						}
						return true;
					}
				}
				return false;
			}

			template <class T >
			static uint16_t save( const BufferAndSize_t< T, uint16_t> &i_data, uint16_t i_idx = 0 )
			{
				uint16_t startIdx = i_idx;
				EEPROM.put(i_idx++, POISON);
				EEPROM.put(i_idx++, (uint8_t)i_data.m_size);
				EEPROM.put(i_idx++, (uint8_t)(i_data.m_size >> 8));

				const uint8_t *t = (const uint8_t*)i_data.m_pData;

				for (auto i = 0; i < i_data.m_size * sizeof(T); i++)
				{
					EEPROM.put(i_idx++, ((const uint8_t*)i_data.m_pData)[i]);
				}

#ifdef ARDUINO_ARCH_ESP8266
				return (EEPROM.commit()) ? i_idx : startIdx;
#else
				return i_idx;
#endif		
			}

			template <class T >
			static bool load( BufferAndSize_t< T, uint16_t> &o_data, uint16_t &i_idx )
			{
				if (POISON == EEPROM.read(i_idx++))
				{
					BufferAndSize_t< T, uint16_t> b;
					b.m_size = EEPROM.read(i_idx++) | EEPROM.read(i_idx++) << 8;
					b.m_pData = new T[b.m_size];
					b.m_owner = true;

					uint8_t *t = (uint8_t*)b.m_pData;

					for (auto i = 0; i < b.m_size * sizeof(T); i++)
					{
						t[i] = EEPROM.read(i_idx++);
					}

					o_data = b;

					return true;
				}
				
				return false;
			}

			static uint16_t save(const String &i_data, uint16_t i_idx = 0)
			{
				BufferAndSize_t<const char, uint16_t> b( i_data.c_str(), i_data.length() );
				return save(b, i_idx);
			}

			static bool load( String &o_data, uint16_t &i_idx )
			{
				BufferAndSize_t<char, uint16_t> b;
				bool sts = load( b, i_idx );
				char * buff = new char[b.m_size + 1];
				memset(buff, 0x0, b.m_size + 1);
				memcpy(buff, b.m_pData, b.m_size);

				o_data = buff;
				delete[] buff;

				return sts;
			}
		};

		const uint8_t EEPROM_Adapter::POISON = 0b11001101;
    }
}

typedef  arduino::utils::EEPROM_Adapter  EEPROM_Adapter_t;
#endif

