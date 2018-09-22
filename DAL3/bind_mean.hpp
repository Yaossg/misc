/**
*	DAL_bind_mean.hpp
*	作者：Yaossg 
*	平均数绑定 
*	std=C++11/14/17
*/
#ifndef DAL_BIND_MEAN_HPP
#define DAL_BIND_MEAN_HPP
#include "mean.hpp"
namespace data_analysis
{
	template<typename T>
	auto bind_binary_mean(binary_mean_t<T> func_mean, OperationSet<T>const& op)
			->std::function<T(T,T)> 
	{
		using namespace std::placeholders;
		return std::bind(func_mean, _1, _2, op);
	}
	template<typename InIt, typename T>
	auto bind_mean(mean_t<InIt, T> func_mean, OperationSet<T>const& op)
			->std::function<T(InIt,InIt)> 
	{
		using namespace std::placeholders;
		return std::bind(func_mean, _1, _2, op);
	}
	template<typename InIt, typename T>
	auto bind_weighted_mean(weighted_mean_t<InIt, T> func_mean, OperationSet<T>const& op)
			->std::function<T(InIt,InIt,InIt,InIt)> 
	{
		using namespace std::placeholders;
		return std::bind(func_mean, _1, _2, _3, _4, op);
	}
}
#endif
