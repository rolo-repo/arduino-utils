#ifndef BufferAndSize_h__
#define BufferAndSize_h__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


namespace arduino {
	namespace utils {
		template< typename T = char, typename S = long>
		struct  BufferAndSize
		{
			BufferAndSize() :m_pData(0), m_size(0) , m_owner(false) {}
			BufferAndSize(T *i_pData, S i_size , bool i_owner = false) :m_pData(i_pData), m_size(i_size) , m_owner (i_owner){}
			BufferAndSize& operator= (const BufferAndSize& i_other)
			{
				if (this == &i_other)
					return *this;

				if (0 != m_pData && m_owner )
				{
					delete[]m_pData;
					m_pData = 0;
					m_size = 0;
				}

				if (0 != i_other.m_pData)
				{
					if( i_other.m_owner )
					{
						m_owner = true;
						m_pData = new T[i_other.m_size];
						memcpy(m_pData, i_other.m_pData, i_other.m_size);
					}
					else
					{
						m_owner = false;
						m_pData = i_other.m_pData;
					}
					m_size = i_other.m_size;	
				}

				return *this;
			}

			void operator=(BufferAndSize&& i_other)
			{
				m_pData = i_other.m_pData;
				m_size = i_other.m_size;
				m_owner = i_other.m_owner;

				i_other.m_pData = 0;
				i_other.m_size = 0;
			}

			~BufferAndSize()
			{
				if(m_owner)
				{
					delete[]m_pData;
					m_pData = 0;
				}
			}
			
			T operator[]( S i_index ) const
			{
				if (i_index < m_size)
					return *(m_pData + i_index);
				else
					return T(0);
			}

			T& operator[](S i_index)
			{
				if ( i_index < m_size )
					return *(m_pData + i_index);
				else
					return *(m_pData + m_size - 1);
			}

			T* operator*()
			{
				return m_pData;
			}

			////////////////////////////
			T *m_pData;
			S  m_size;
			bool m_owner;
		};
	}
}

template< typename T = char, typename S = long >
using BufferAndSize_t = arduino::utils::BufferAndSize<T, S>;
#endif // BufferAndSize_h__
