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
			BufferAndSize() :m_pData(0), m_size(0) {}
			BufferAndSize(T *i_pData, S i_size) :m_pData(i_pData), m_size(i_size) {}
			BufferAndSize& operator= (const BufferAndSize& i_other)
			{
				if (this == &i_other)
					return *this;

				if (0 != m_pData)
				{
					delete[]m_pData;
					m_pData = 0;
					m_size = 0;
				}

				if (0 != i_other.m_pData)
				{
					m_pData = new T[i_other.m_size];
					memcpy(m_pData, i_other.m_pData, i_other.m_size);
					m_size = i_other.m_size;
				}

				return *this;
			}

			void operator=(BufferAndSize&& i_other)
			{
				m_pData = i_other.m_pData;
				m_size = i_other.m_size;
			}
			
			////////////////////////////
			T *m_pData;
			S  m_size;
		};
	}
}

template< typename T = char, typename S = long >
using BufferAndSize_t = arduino::utils::BufferAndSize<T, S>;
#endif // BufferAndSize_h__
