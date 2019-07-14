#ifndef RFcom_h__
#define RFcom_h__
#include "SerialOutput.h"
namespace arduino {
    namespace utils {

        static const short RF_TIMEOUT_MS = 1000;

        unsigned int hash32(unsigned int i_key32, int i_initValue = 0)
        {
            #define ROT32(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

            #define FINAL32(a, b, c) \
            { \
                c ^= b; c -= ROT32 (b, 14); \
                a ^= c; a -= ROT32 (c, 11); \
                b ^= a; b -= ROT32 (a, 25); \
                c ^= b; c -= ROT32 (b, 16); \
                a ^= c; a -= ROT32 (c, 4);  \
                b ^= a; b -= ROT32 (a, 14); \
                c ^= b; c -= ROT32 (b, 24); \
            }

            unsigned int a, b, c;

            a = b = c = 0xdeadbeef + (static_cast<int>(1 /* because of Fixed1 */) << 2) + i_initValue;
            a += i_key32;

            FINAL32(a, b, c);

            return c;
        }

        struct Payload
        {
			unsigned int m_cksum = 0;
			
			union {
				unsigned int m_data;
				unsigned char m_j[4] = { 0,0,0,0 };
				struct { unsigned char m_speed : 8, m_steering : 8, m_j3 : 8, m_j4 : 8; };
			};

			union {
				unsigned char m_bits = 0;//8 bits
				struct { unsigned char m_b1 : 1, m_b2 : 1, m_b3 : 1, m_b4 : 1; };
			};

            bool isValid() const
            {
                return ( m_cksum == hash32( hashify() ) );
            }

            const Payload* finalize()
            {
                m_cksum = hash32( hashify() );
                return this;
            }

		private:
			unsigned int hashify() const
			{
				return ( ( m_data & 0x00ffffff ) | m_bits << 24 );
				//return   ( m_j[0] | ( m_j[1] << 8) | ( m_j[2] << 16 ) | ( m_bits << 24 ) ) ;
			}
        };


        bool operator==( const Payload& i_left, const Payload& i_right )
        {
			if (i_left.m_bits != i_right.m_bits)
			{
				LOG_MSG("Bits " << i_left.m_bits << " " << i_right.m_bits);
				return false;
			}

			if (i_left.m_data != i_right.m_data)
			{
				for ( char i = 0; i < sizeof(Payload::m_j); i++ )
				{
					if (abs(i_left.m_j[i] - i_right.m_j[i]) > 10)
					{
						LOG_MSG("Data " << i_left.m_j[i] << " " << i_right.m_j[i]);

						return false;
			
					}
				}
			}

			/*
			if (abs(i_left.m_speed - i_right.m_speed) > 5)
            {
                return false;
            }

            if (abs(i_left.m_steering - i_right.m_steering) > 5)
            {
                return false;
            }*/

         

            return true;
        }

        struct PayloadAck
        {
            char speed;
            char batteryLevel;
        } ack;

    }
}
#endif // RFcom_h__