/**
*	DAL_mode.hpp
*	作者：Yaossg 
*	众数 
*	std=C++11/14/17
*/
#ifndef DAL_MODE_HPP
#define DAL_MODE_HPP
#include "OperationSet.hpp"
namespace data_analysis
{
	template<typename InIt, typename T>
	inline std::vector<T> mode(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(std::distance(first, last) <= 2)return {};
		std::map<T, size_t> m;
		for(InIt p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		if(std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return op.equal(*cnt.begin(), v);}))return {};
		std::vector<T> num(m.size());
		std::transform(m.begin(), m.end(), num.begin(), 
				[](std::pair<T, size_t> p){return p.first;});
		size_t max = *std::max_element(cnt.begin(), cnt.end());
		std::vector<T> ret(std::count_if(cnt.begin(), cnt.end(), 
				[&](T v){return equal(v, max);}));
		std::copy_if(num.begin(), num.end(), ret.begin(), 
				[&](T v){return equal(m[v], max);});
		return ret;
	}
	template<typename InIt, typename T>
	inline std::size_t count_mode(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(std::distance(first, last) <= 2)return 0;
		std::map<T, size_t> m;
		for(InIt p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		if(std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return op.equal(*cnt.begin(), v);}))return 0;
		size_t max = *std::max_element(cnt.begin(), cnt.end());
		return std::count_if(cnt.begin(), cnt.end(), 
				[&](T v){return equal(v, max);});
	}
	template<typename InIt, typename T>
	inline bool has_mode(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(std::distance(first, last) <= 2)return false;
		std::map<T, size_t> m;
		for(InIt p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		return !std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return op.equal(*cnt.begin(), v);});
	}
}
#endif
