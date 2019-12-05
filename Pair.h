// Pair.h

#ifndef _PAIR_h
#define _PAIR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

template < typename K, typename V > class Pair
{
public:
	K first;
	V second;

private:
	Pair(const K &i_first, const V &i_second) : first(i_first), second(i_second) {}

public:
	Pair() {}

	Pair(const Pair& i_other)
	{
		if (this == &i_other)
			return;

		this->first = i_other.first;
		this->second = i_other.second;
	}

	template <typename _K, typename _V>
	friend Pair<_K, _V> make_Pair(const _K&, const _V&);
};

template < typename K, typename V >
Pair<K, V> make_Pair(const K &i_first, const V &i_second)
{
	return Pair<K, V>(i_first, i_second);
}

#endif

