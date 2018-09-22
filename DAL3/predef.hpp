/**
*	DAL_predef.hpp
*	作者：Yaossg 
*	预定义 
*	std=C++11/14/17/20 
*/
#ifndef DAL_PREDEF_HPP
#define DAL_PREDEF_HPP
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <functional>
#include <numeric>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include <tuple>
namespace data_analysis
{
	template<typename T>
	using valueT = typename std::iterator_traits<T>::value_type;
	template<typename T>
	using predT = std::function<bool(T,T)>;
	template<typename T>
	using biPredT = std::function<bool(T,T)>;
	template<typename T>
	using funcT = std::function<T(T)>;
	template<typename T>
	using biFuncT = std::function<T(T,T)>;
	struct RangeError : public std::invalid_argument
	{
		RangeError(std::string str)
			: invalid_argument(str) {}
		~RangeError() noexcept {}
	};
	struct EmptyRange : public RangeError
	{
		EmptyRange()
			: RangeError("EmptyRange") {}
		~EmptyRange() noexcept {}
	};
	struct DifferentRangeSize : public RangeError
	{
		DifferentRangeSize()
			: RangeError("DifferentRangeSize") {}
		~DifferentRangeSize() noexcept {}
	};
}
#endif
