#ifndef DAL_VARIANCE_HPP
#define DAL_VARIANCE_HPP

#include "mean.hpp"

namespace data_analysis
{
	template<typename T>
	using binary_varianceT = std::function<T(T,T,binary_mean_t<T>)>;
	template<typename It, typename T = valueT<It>>
	using varianceT = std::function<T(It,It,mean_t<T>)>;
	template<typename It, typename T = valueT<It>>
	using weighted_varianceT = std::function<T(It,It,It,It,weighted_mean_t<T>)>;
	template<typename T>
	inline T variance_core(T v, T m)
	{
		return (m - v) * (m - v);
	}
	template<typename T>
	inline T binary_variance(T a, T b, binary_mean_t<T> mean)
	{
        auto m = mean(a, b);
		return mean(variance_core(a, m), variance_core(b, m));
	}
	template<typename It, typename T=valueT<It>>
	inline T variance(It first, It last, mean_t<T> mean)
	{
		if(first == last)throw empty_range();
		auto size = std::distance(first, last);
		if(size == 1)return 0;
        using namespace std::placeholders;
		return std::accumulate(iterator_invoker<It>(first,
		        std::bind(variance_core, _1, mean(first, last)),
		        iterator_invoker<It>(last), T(0))) / size;
	}
	template<typename It, typename T=valueT<It>>
	inline T weighted_variance(It first, It last, It first2, It last2, mean_t<T> mean)
	{
		if(first == last || first2 == last2)throw empty_range();
        auto size = std::distance(first, last);
		if(size != std::distance(first2, last2))
			throw different_range();
		if(size == 1)return 0;
		using namespace std::placeholders;
		return std::inner_product(first, last, first2, T(0),
			std::bind(std::multiplies<T>(), std::bind(variance_core, _1, mean(first, last)), _2));
	}
	template<typename T>
	inline T binary_standard_deviation(T a, T b, binary_mean_t<T> m)
	{
		return std::sqrt(binary_variance(a, b, m));
	}
	template<typename It, typename T=valueT<It>>
	inline T standard_deviation(It first, It last, mean_t<T> m)
	{
		return std::sqrt(variance(first, last, m));
	}
	template<typename It, typename T=valueT<It>>
	inline T weighted_standard_deviation(It first, It last, 
		It first2, It last2, weighted_mean_t<T> m)
	{
		return std::sqrt(weighted_variance(first, last, first2, last2, m));
	}
}

#endif
