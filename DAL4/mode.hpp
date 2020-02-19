#ifndef DAL_MODE_HPP
#define DAL_MODE_HPP

#include "predef.hpp"

namespace data_analysis
{
	template<typename It, typename T=valueT<It>>
	inline std::vector<T> mode(It first, It last, biPredT<T> equal = std::equal_to<T>())
	{
		if(std::distance(first, last) <= 2)return {};
		std::map<T, size_t> m;
		for(It p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		if(std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return equal(*cnt.begin(), v);}))return {};
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
	template<typename It, typename T=valueT<It>>
	inline std::size_t count_mode(It first, It last, biPredT<T> equal = std::equal_to<T>())
	{
		if(std::distance(first, last) <= 2)return 0;
		std::map<T, size_t> m;
		for(It p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		if(std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return equal(*cnt.begin(), v);}))return 0;
		size_t max = *std::max_element(cnt.begin(), cnt.end());
		return std::count_if(cnt.begin(), cnt.end(), 
				[&](T v){return equal(v, max);});
	}
	template<typename It, typename T=valueT<It>>
	inline bool has_mode(It first, It last, biPredT<T> equal = std::equal_to<T>())
	{
		if(std::distance(first, last) <= 2)return false;
		std::map<T, size_t> m;
		for(It p = first; p != last; m[*p++]++);
		std::vector<size_t> cnt(m.size());
		std::transform(m.begin(), m.end(), cnt.begin(), 
				[](std::pair<T, size_t> p){return p.second;});
		return !std::all_of(std::next(cnt.begin()), cnt.end(), 
			[&](size_t v){return equal(*cnt.begin(), v);});
	}
}

#endif
