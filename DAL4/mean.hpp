#ifndef DAL_MEAN_HPP
#define DAL_MEAN_HPP

#include "predef.hpp"
#include "iterator_invoker.hpp"

#define DAL_CHECK_MEAN(size) \
    if(first == last)throw empty_range(); \
    auto size = std::distance(first, last); \
    if(size == 1)return *first;

#define DAL_CHECK_WEIGHTED_MEAN(size) \
    if(first == last || first2 == last2)throw empty_range(); \
    auto size = std::distance(first, last); \
    if(size != std::distance(first2, last2)) \
    throw different_range(); \
    if(size == 1)return *first;

namespace data_analysis
{
	template<typename T>
	using binary_mean_t = std::function<T(T,T)>;
	template<typename It, typename T = valueT<It>>
	using mean_t = std::function<T(It,It)>;
	template<typename It, typename T = valueT<It>>
	using weighted_mean_t = std::function<T(It,It,It,It)>;
	template<typename T>
	inline T binary_mean(T left, T right)
	{
		return (left + right) / T(2);
	}
	template<typename It, typename T = valueT<It>>
	inline T mean(It first, It last)
	{
		DAL_CHECK_MEAN(size)
		return std::accumulate(first, last, T(0)) / size;
	}
	template<typename It, typename T = valueT<It>>
	inline T weighted_mean(It first, It last, It first2, It last2)
	{
		DAL_CHECK_WEIGHTED_MEAN(size)
		return std::inner_product(first, last, first2, T(0)) / std::accumulate(first2, last2, T(0));
	}
	template<typename T>
	inline T binary_harmonic_mean(T left, T right)
	{
		return T(2) * left * right / (left + right);
	}
	template<typename It, typename T = valueT<It>>
	inline T harmonic_mean(It first, It last)
	{
		DAL_CHECK_MEAN(size)
        using namespace std::placeholders;
		return size / std::accumulate(
		        iterator_invoker<It>(first, std::bind(std::divides<T>(), 1., _1)),
		        iterator_invoker<It>(last), 0.);
	}
	template<typename It, typename T = valueT<It>>
	inline T weighted_harmonic_mean(It first, It last, 
			It first2, It last2)
	{
		DAL_CHECK_WEIGHTED_MEAN(size)
		return std::accumulate(first2, last2, T(0)) /
			std::inner_product(first2, last2, first, T(0), std::plus<T>(), std::divides<T>());
	}
	template<typename T>
	inline T binary_quadratic_mean(T left, T right)
	{
		return std::sqrt(left * left + right * right);
	}
	template<typename It, typename T = valueT<It>>
	inline T quadratic_mean(It first, It last)
	{
		DAL_CHECK_MEAN(size)
        using namespace std::placeholders;
		return std::sqrt(std::accumulate(
		        iterator_invoker<It>(first, std::bind(std::multiplies<T>(), _1, _1)),
		        iterator_invoker<It>(last), T(0)) / size);
	}
	template<typename It, typename T = valueT<It>>
	inline T weighted_quadratic_mean(It first, It last, 
			It first2, It last2)
	{
		DAL_CHECK_WEIGHTED_MEAN(size)
        using namespace std::placeholders;
		return std::sqrt(std::inner_product(
		        iterator_invoker<It>(first, std::bind(std::multiplies<T>(), _1, _1)),
		        iterator_invoker<It>(last), first2, T(0)) /
				std::accumulate(first2, last2, T(0)));
	} 
	template<typename T>
	inline T binary_geometric_mean(T left, T right)
	{
		return std::sqrt(left * right);
	}
	template<typename It, typename T = valueT<It>>
	inline T geometric_mean(It first, It last)
	{
		DAL_CHECK_MEAN(size)
		return std::pow(std::accumulate(first, last, T(1), std::multiplies<T>()),
				1. / std::distance(first, last));
	}
	template<typename It, typename T = valueT<It>>
	inline T weighted_geometric_mean(It first, It last, 
			It first2, It last2)
	{
		DAL_CHECK_WEIGHTED_MEAN(size)
        using namespace std::placeholders;
		return std::pow(std::inner_product(first, last, first2, T(1),
		        std::multiplies<T>(), std::pow<T, T>),
				1. / std::accumulate(first2, last2, T(0)));
	}
}

#undef DAL_CHECK_MEAN
#undef DAL_CHECK_WEIGHTED_MEAN
#endif
