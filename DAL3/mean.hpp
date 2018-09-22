/**
*	DAL_mean.hpp
*	作者：Yaossg 
*	平均数 
*	std=C++11/14/17
*/
#ifndef DAL_MEAN_HPP
#define DAL_MEAN_HPP
#include "OperationSet.hpp" 
#include "iterator_invoker.hpp"
namespace data_analysis
{
	template<typename T>
	using binary_mean_t = std::function<T(T,T,OperationSet<T>const&)>;
	template<typename InIt, typename T = valueT<InIt>>
	using mean_t = std::function<T(InIt,InIt,OperationSet<T>const&)>;
	template<typename InIt, typename T = valueT<InIt>>
	using weighted_mean_t = std::function<T(InIt,InIt,InIt,InIt,OperationSet<T>const&)>;
	template<typename T>
	inline T binary_mean(T left, T right, OperationSet<T>const& op)
	{
		return op.divide(op.plus(left, right), T(2));
	}
	template<typename InIt, typename T>
	inline T mean(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyRange();
		return op.divide(std::accumulate(first, last, T(0), op.plus), 
		std::distance(first, last));
	}
	template<typename InIt, typename T>
	inline T weighted_mean(InIt first, InIt last, 
			InIt first2, InIt last2, OperationSet<T>const& op)
	{
		if(first == last || first2 == last2)throw EmptyRange();
		if(std::distance(first, last) != std::distance(first2, last2))
			throw DifferentRangeSize();
		if(std::distance(first, last) == 1)return *first;
		return op.divide(std::inner_product(first, last, first2, T(0), op.plus, op.multi), 
				std::accumulate(first2, last2, T(0), op.plus));
	}
	template<typename T>
	inline T binary_harmonic_mean(T left, T right, OperationSet<T>const& op)
	{
		return op.divide(op.multi(T(2), op.multi(left, right)), op.plus(left, right));
	}
	template<typename InIt, typename T>
	inline T harmonic_mean(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyRange();
		if(std::distance(first, last) == 1)return *first;
		return op.divide(std::distance(first, last), 
				std::accumulate(iterator_invoker<InIt>(first, 
				[&](T left){return op.divide(T(1), left);}), 
				last, T(0), op.plus));
	}
	template<typename InIt, typename T>
	inline T weighted_harmonic_mean(InIt first, InIt last, 
			InIt first2, InIt last2, OperationSet<T>const& op)
	{
		if(first == last || first2 == last2)throw EmptyRange();
		if(std::distance(first, last) != std::distance(first2, last2))
			throw DifferentRangeSize();
		if(std::distance(first, last) == 1)return *first;
		return op.divide(std::accumulate(first2, last2, T(0), op.plus), 
			std::inner_product(first2, last2, first, T(0), op.plus, op.divide));
	}
	template<typename T>
	inline T binary_quadratic_mean(T left, T right, OperationSet<T>const& op)
	{
		return op.sqrt(op.plus(op.multi(left, left), op.multi(right, right)));
	}
	template<typename InIt, typename T>
	inline T quadratic_mean(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyRange();
		if(std::distance(first, last) == 1)return *first;
		return op.sqrt(op.divide(
				std::accumulate(iterator_invoker<InIt>(first, 
				[&](T left){return op.multi(left, left);}), last, T(0), op.plus),
				std::distance(first, last)));
	}
	template<typename InIt, typename InIt2, typename T, typename T2>
	inline T weighted_quadratic_mean(InIt first, InIt last, 
			InIt2 first2, InIt2 last2, OperationSet<T>const& op) = delete;
	template<typename T>
	inline T binary_geometric_mean(T left, T right, OperationSet<T>const& op)
	{
		return op.sqrt(op.multi(left, right));
	}
	template<typename InIt, typename T>
	inline T geometric_mean(InIt first, InIt last, OperationSet<T>const& op)
	{
		if(first == last)throw EmptyRange();
		return op.power(std::accumulate(first, last, T(1), op.multi), 
				op.divide(T(1), std::distance(first, last)));
	}
	template<typename InIt, typename T>
	inline T weighted_geometric_mean(InIt first, InIt last, 
			InIt first2, InIt last2, OperationSet<T>const& op)
	{
		if(first == last || first2 == last2)throw EmptyRange();
		if(std::distance(first, last) != std::distance(first2, last2))
			throw DifferentRangeSize();
		if(std::distance(first, last) == 1)return *first;
		return op.power(std::inner_product(first, last, first2, T(0), op.multi, op.pow), 
				op.divide(T(1), std::accumulate(first2, last2, T(0), op.plus)));	
	}
}
#endif
