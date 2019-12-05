#ifndef _InteruptEncoder_h
#define _InteruptEncoder_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Constants.h"

namespace arduino {
	namespace utils {
		class InteruptEncoder
		{
		public:
			InteruptEncoder(PIN i_p1, PIN i_p2) :m_p1(i_p1), m_p2(i_p2) {}

			void begin() volatile
			{
				m_val = 0;
				m_prevNextCode = 3;
				//attachInterrupt(digitalPinToInterrupt(m_p1), handle_interupt, CHANGE);
				//	attachInterrupt(digitalPinToInterrupt(m_p2), handle_interupt, CHANGE);

			}

			void stop() volatile
			{
				//detachInterrupt(digitalPinToInterrupt(m_p1));
			}

			void run() volatile
			{
				static char rot_enc_table[] = { 0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0 };

				m_prevNextCode <<= 2;
				if (digitalRead(m_p1)) m_prevNextCode |= 0x02;
				if (digitalRead(m_p2)) m_prevNextCode |= 0x01;
				m_prevNextCode &= 0x0f;

				// If valid then store as 16 bit data.
				if (rot_enc_table[m_prevNextCode]) {
					m_fence <<= 4;
					m_fence |= m_prevNextCode;

					if ((m_fence & 0xff) == 0x2b) m_val--;
					if ((m_fence & 0xff) == 0x17) m_val++;
				}
			}

			short val() volatile const
			{
				return abs(m_val);
			}

			InteruptEncoder(const InteruptEncoder&) = delete;
			InteruptEncoder& operator=(const InteruptEncoder&) = delete;
		private:

			PIN m_p1;
			PIN m_p2;
			short m_val = 0;
			unsigned char m_prevNextCode = 0;
			unsigned short m_fence = 0;
		};
	}
}

typedef   arduino::utils::InteruptEncoder  InteruptEncoder_t;

#endif