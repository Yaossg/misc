/**
*	DAL_other.hpp
*	作者：Yaossg 
*	其他 
*	std=C++11/14/17
*/
#ifndef DAL_OTHER_HPP
#define DAL_OTHER_HPP
#include "mean.hpp"
namespace data_analysis
{
	template<typename InIt, typename T, class M>
	inline T median(InIt first, InIt last, M m, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyRange();
		using diff_t = typename std::iterator_traits<InIt>::difference_type;
		diff_t dist = std::distance(first, last);
		std::vector<T> vec(dist);
		auto vlast = vec.end();
		std::make_heap(vec.begin(), vlast);
		for(int i = 0; i < (dist + 1) / 2; i++) std::pop_heap(vec.begin(), vlast--);
		return dist % 2 ? *std::next(vlast) : m(*vlast, *std::next(vlast), op); 
	}
}
#endif
