/**
*	DAL_MeanReflector.hpp
*	作者：Yaossg 
*	平均数反射 
*	std=C++11/14/17
*/
#ifndef DAL_MEANREFLECTOR_HPP
#define DAL_MEANREFLECTOR_HPP
#include "mean.hpp" 
namespace data_analysis
{
	struct MeanReflector
	{
		template<typename T>
		static binary_mean_t<T> get_binary(std::string str, 
			T left, T right, OperationSet<T>const& op)
		{
			if(str=="")return binary_mean<T>;
			if(str=="harmonic")return binary_harmonic_mean<T>;
			if(str=="quadratic")return binary_quadratic_mean<T>;
			if(str=="geometric")return binary_geometric_mean<T>;
			throw std::invalid_argument("invalid binary_mean type");
		}
		template<typename InIt, typename T>
		static mean_t<InIt, T> get(std::string str, 
			InIt first, InIt last, OperationSet<T>const& op)
		{
			if(str=="")return mean<InIt, T>;
			if(str=="harmonic")return harmonic_mean<InIt, T>;
			if(str=="quadratic")return quadratic_mean<InIt, T>;
			if(str=="geometric")return geometric_mean<InIt, T>;
			throw std::invalid_argument("invalid mean type");
		}
		template<typename InIt, typename T>
		static weighted_mean_t<InIt, T> get_weighted(std::string str, 
			InIt first, InIt last, InIt first2, InIt last2, OperationSet<T>const& op)
		{
			if(str=="")return weighted_mean<InIt, T>;
			if(str=="harmonic")return weighted_harmonic_mean<InIt, T>;
		//	if(str=="quadratic")return weighted_quadratic_mean<InIt, T>;
			if(str=="geometric")return weighted_geometric_mean<InIt, T>;
			throw std::invalid_argument("invalid weighted_mean type");
		}
		
		template<typename T>
		static T call_binary(std::string str, 
			T left, T right, OperationSet<T>const& op)
		{
			if(str=="")return binary_mean(left, right, op);
			if(str=="harmonic")return binary_harmonic_mean(left, right, op);
			if(str=="quadratic")return binary_quadratic_mean(left, right, op);
			if(str=="geometric")return binary_geometric_mean(left, right, op);
			throw std::invalid_argument("invalid binary_mean type");
		}
		template<typename InIt, typename T>
		static T call(std::string str, 
			InIt first, InIt last, OperationSet<T>const& op)
		{
			if(str=="")return mean(first, last, op);
			if(str=="harmonic")return harmonic_mean(first, last, op);
			if(str=="quadratic")return quadratic_mean(first, last, op);
			if(str=="geometric")return geometric_mean(first, last, op);
			throw std::invalid_argument("invalid mean type");
		}
		template<typename InIt, typename T>
		static T call_weighted(std::string str, 
			InIt first, InIt last, InIt first2, InIt last2, OperationSet<T>const& op)
		{
			if(str=="")return weighted_mean(first, last, first2, last2, op);
			if(str=="harmonic")return weighted_harmonic_mean(first, last, first2, last2, op);
		//	if(str=="quadratic")return weighted_quadratic_mean(first, last, first2, last2, op);
			if(str=="geometric")return weighted_geometric_mean(first, last, first2, last2, op);
			throw std::invalid_argument("invalid weighted_mean type");
		}
		
	};
}
#endif
