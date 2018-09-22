/**
*	DAL_variance.hpp
*	×÷Õß£ºYaossg 
*	·½²î 
*	std=C++11/14/17
*/
#ifndef DAL_VARIANCE_HPP
#define DAL_VARIANCE_HPP
#include "mean.hpp"
namespace data_analysis
{
	template<typename T>
	using binary_varianceT = std::function<T(T,T,binary_mean_t<T>,OperationSet<T>const&)>;
	template<typename InIt, typename T = valueT<InIt>>
	using varianceT = std::function<T(InIt,InIt,mean_t<T>,OperationSet<T>const&)>;
	template<typename InIt, typename T = valueT<InIt>>
	using weighted_varianceT = std::function<T(InIt,InIt,InIt,InIt,weighted_mean_t<T>,OperationSet<T>const&)>;
	template<typename T>
	inline T variance_core(T v, T m, OperationSet<T>const& op)
	{
		return op.multi(op.minus(m, v), op.minus(m, v));
	}
	template<typename T>
	inline T binary_variance(T a, T b, binary_mean_t<T> m, OperationSet<T>const& op)
	{
		return m(variance_core(a, m(a, b, op), op), variance_core(b, m(a, b, op), op), op);
	}
	template<typename InIt, typename T>
	inline T variance(InIt first, InIt last, mean_t<T> m, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyIteratorDistance();
		if(std::distance(first, last) == 1)return 0;
		auto r_m = m(first, last, op);
		return op.divide(std::accumulate(IterCaller<InIt>(first, 
				[&](T left){return variance_core(left, r_m);}), 
				IterCaller<InIt>(last), T(0), op.plus), 
				std::distance(first, last));
	}
	template<typename InIt, typename T>
	inline T weighted_variance(InIt first, InIt last, InIt first2, InIt last2, 
		mean_t<T> m, OperationSet<T>const& op)
	{
		if(first == last || first2 == last2)throw EmptyIteratorDistance();
		if(std::distance(first, last) != std::distance(first2, last2))
			throw DifferentIteratorDistance();
		if(std::distance(first, last) == 1)return 0;
		auto r_m = m(first, last, op);
		using namespace std::placeholders;
		return std::inner_product(first, last, first2, T(0), op.plus, 
			std::bind(op.multi, std::bind(variance_core, _1, r_m), _2));
	}
	template<typename T>
	inline T binary_standard_deviation(T a, T b, binary_mean_t<T> m, OperationSet<T>const& op)
	{
		return op.sqrt(binary_variance(a, b, m, op));
	}
	template<typename InIt, typename T>
	inline T standard_deviation(InIt first, InIt last, T m, OperationSet<T>const& op)
	{
		return op.sqrt(variance(first, last, m, op));
	}
	template<typename InIt, typename T>
	inline T weighted_standard_deviation(InIt first, InIt last, 
		InIt first2, InIt last2, T m, OperationSet<T>const& op)
	{
		return op.sqrt(weighted_variance(first, last, first2, last2, m, op));
	}
}
#endif
