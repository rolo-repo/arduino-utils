#ifndef RFcom_h__
#define RFcom_h__

namespace arduino {
    namespace utils {

        static const short RF_TIMEOUT_MS = 1000;

        unsigned int hash32(unsigned long i_key32, long i_initValue = 0)
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

            unsigned long a, b, c;

            a = b = c = 0xdeadbeef + (static_cast<long>(1 /* because of Fixed1 */) << 2) + i_initValue;
            a += i_key32;

            FINAL32(a, b, c);

            return c;
        }
//////////////////////////////////////////////////////////////////////
        struct Payload
        {
			unsigned long m_cksum = 0;
			
			union {
				struct { char m_speed, m_steering, m_j3, m_j4; };
				unsigned long m_data;
			    char m_j[4];
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

			Payload()
			{
				//The struct holds POD only , i can do it
				memset(this, 0x0, sizeof(Payload));
			}
		private:
			unsigned int hashify() const
			{
				return ( ( m_data & 0x00ffffff ) | m_bits << 24 );
				//return   ( m_j[0] | ( m_j[1] << 8) | ( m_j[2] << 16 ) | ( m_bits << 24 ) ) ;
			}
        };
//////////////////////////////////////////////////////////////////////
        bool operator==( const Payload& i_left, const Payload& i_right )
        {
			if ( i_left.m_bits != i_right.m_bits )
			{
				return false;
			}

			if ( i_left.m_data != i_right.m_data )
			{
				for ( char i = 0; i < sizeof(Payload::m_j); i++ )
				{
					if (abs(i_left.m_j[i] - i_right.m_j[i]) > 10)
					{
						return false;
					}
				}
			}

            return true;
        }
//////////////////////////////////////////////////////////////////////
        struct PayloadAck
        {
            unsigned char speed;
            unsigned char batteryLevel;
        } payLoadAck;

    }
}
#endif // RFcom_h__