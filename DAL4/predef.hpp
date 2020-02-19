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
	using biPredT = std::function<bool(T,T)>;
	template<typename T>
	using funcT = std::function<T(T)>;
	struct range_error : public std::invalid_argument
	{
		explicit range_error(std::string const& str): invalid_argument(str) {}
	};
	struct empty_range : public range_error
	{
		explicit empty_range(): range_error("empty_range") {}
	};
	struct different_range : public range_error
	{
		explicit different_range(): range_error("different_range") {}
	};
}

#endif
