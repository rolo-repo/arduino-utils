#ifndef RFcom_h__
#define RFcom_h__

namespace arduino {
    namespace utils {

        static const uint16_t RF_TIMEOUT_MS = 1000;

		uint16_t hash32( uint32_t i_key32, int32_t i_initValue = 0 )
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

			uint32_t a, b, c;

            a = b = c = 0xdeadbeef + ( static_cast<int32_t>( 1 /* because of Fixed1 */) << 2 ) + i_initValue;
            a += i_key32;

            FINAL32(a, b, c);

            return c;
        }
//////////////////////////////////////////////////////////////////////
        struct Payload
        {
			uint32_t m_cksum = 0;
			
			union {
				struct { int8_t m_speed, m_steering, m_j3, m_j4; };
				uint32_t m_data;
			    int8_t m_j[4];
			};

			union {
				uint8_t m_bits = 0;//8 bits
				struct { uint8_t m_b1 : 1, m_b2 : 1, m_b3 : 1, m_b4 : 1; };
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
			uint16_t hashify() const
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
					if ( abs( i_left.m_j[i] - i_right.m_j[i] ) > 1 )
					{
						return false;
					}

					if ( 0 != i_left.m_j[i] &&  0 == i_right.m_j[i] || 0 == i_left.m_j[i] && 0 != i_right.m_j[i])
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
            uint8_t speed;
			uint8_t batteryLevel;
        } payLoadAck;

    }
}
#endif // RFcom_h__