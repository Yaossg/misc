#ifndef DAL_OTHER_HPP
#define DAL_OTHER_HPP

#include "predef.hpp"

namespace data_analysis
{
	template<typename It, typename T=valueT<It>>
	inline T median(It first, It last, binary_mean_t<T> m)
	{
		if(first == last)throw empty_range();
		auto size = std::distance(first, last);
		std::vector<T> vec(size);
		auto vl = vec.end();
		std::make_heap(vec.begin(), vl);
		for(size_t i = 0; i < (size + 1) >> 2; i++) std::pop_heap(vec.begin(), vl--);
		return size & 1 ? *std::next(vl) : m(*vl, *std::next(vl));
	}
}

#endif
