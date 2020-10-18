#ifndef PlaceHolder_h__
#define PlaceHolder_h__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace arduino {
	namespace utils {
		//DataDictionary.h

		class PlaceHolder {
		public:
			String value;

		private:
			 PlaceHolder* next;
			unsigned long hash;
		public:
			PlaceHolder(const String& i_key, const String& i_value, PlaceHolder *i_next = 0) : hash(hashify(i_key)), value(i_value), next(i_next) {}
			PlaceHolder() {}

			static PlaceHolder* makeList(int count, ...)
			{
				PlaceHolder *s, *s_p = 0;
				va_list ap;
				va_start(ap, count);
				while (count--) {
					s = va_arg(ap, PlaceHolder*);
					s->next = s_p;
					s_p = s;
				}
				va_end(ap);
				return s;
			}


			static String tr( const PlaceHolder * const  i_pDict, const String& i_buffer )
			{
				using namespace arduino::utils;
				static const String prefix("${"), suffix('}');

				String data;
				data.reserve( 1024 );

				data += i_buffer;
				
				String placeHolderData;
				
				bool potentialPlaceHolder = false, placeHolder = false;
				unsigned long hash = 0;
				for (uint32_t i = 0; i < i_buffer.length(); i++) {
					if (i_buffer[i] == prefix[0]) {
						potentialPlaceHolder = true;
						continue;
					}

					if (i_buffer[i] == prefix[1] && potentialPlaceHolder) {
						potentialPlaceHolder = false;
						placeHolder = true;
						continue;
					}

					if (i_buffer[i] == suffix[0] && placeHolder) {
						placeHolder = false;

						for (const PlaceHolder *pd = i_pDict; ; pd = pd->next) {
							if (pd->hash == hash) {
								data.replace(prefix + placeHolderData + suffix, pd->value);
								break;
							}

							if (pd->next == 0) {
								break;
							}
						}

						placeHolderData.clear();
						continue;
					}

					if (placeHolder) {
						placeHolderData.concat(i_buffer[i]);
						hash = hashify(placeHolderData);
					}
				}

				return data;
			}

		private:
			static unsigned long hashify(const String& i_value)
			{
				unsigned long result = 0;
				for (unsigned int i = 0; i < i_value.length(); i++) {
					result += ((i + 1)  * i_value[i]);
				}

				return result;
			}
		};
	}
}

#endif // PlaceHolder_h__