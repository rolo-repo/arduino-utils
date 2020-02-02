#ifndef _EEPROM_Adapter_h
#define _EEPROM_Adapter_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "EEPROM.h"
#include "BufferAndSize.h"

namespace arduino {
    namespace utils {
		class EEPROM_Adapter
		{
		private:
			const static uint8_t POISON;
		public:

			static void begin( size_t size ) { EEPROM.begin( size ); }

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

				return (EEPROM.commit()) ? i_idx : startIdx;
			}

			template <class T >
			static bool load( T &i_data, uint16_t i_idx = 0 )
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

				return (EEPROM.commit()) ? i_idx : startIdx;
			}

			template <class T >
			static bool load( BufferAndSize_t< T, uint16_t> &o_data, uint16_t i_idx = 0 )
			{
				if (POISON == EEPROM.read(i_idx++))
				{
					BufferAndSize_t< T, uint16_t> b;
					b.m_size = EEPROM.read(i_idx++) | EEPROM.read(i_idx++) << 8;
					b.m_pData = new T[b.m_size];

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
		};

		const uint8_t EEPROM_Adapter::POISON = 0b11001101;
    }
}

typedef  arduino::utils::EEPROM_Adapter  EEPROM_Adapter_t;
#endif

