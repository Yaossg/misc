/**
*	DAL_OperationSet.hpp
*	作者：Yaossg 
*	操作符合集 
*	std=C++11/14/17
*/
#ifndef DAL_OPERATIONSET_HPP
#define DAL_OPERATIONSET_HPP
#include "predef.hpp"
namespace data_analysis
{
	template<typename T>
	class OperationSet
	{
		struct std_pow
		{
			T operator()(T a,T b)const
			{
				return std::pow(a,b);
			}
		};
		struct std_sqrt
		{
			T operator()(T a)const
			{
				return std::sqrt(a);
			}
		};
	public:
		biPredT<T> equal;
		biPredT<T> comp;
		biFuncT<T> plus;
		biFuncT<T> minus;
		biFuncT<T> multi;
		biFuncT<T> divide;
		biFuncT<T> power;
		  funcT<T> sqrt;
		OperationSet(){*this = OperationSet<T>();} 
		OperationSet(OperationSet const&) = default;
		OperationSet<T>& withEqual
		(biPredT<T> p =std::equal_to<T>())	{equal = p;	return *this;}
		OperationSet<T>& withComp
		(biPredT<T> p = std::less	<T>())	{comp = p;	return *this;}
		OperationSet<T>& withPlus
		(biFuncT<T> f = std::plus	<T>())	{plus = f;	return *this;}
		OperationSet<T>& withMinus
		(biFuncT<T> f = std::minus	<T>())	{minus = f;	return *this;}
		OperationSet<T>& withMulti
		(biFuncT<T>f=std::multiplies<T>())	{multi = f;	return *this;}
		OperationSet<T>& withDivide
		(biFuncT<T> f = std::divides<T>())	{divide = f;return *this;}
		OperationSet<T>& withPower
		(biFuncT<T> f = std_pow		  ())	{power = f;	return *this;}
		OperationSet<T>& withSqrt
		(  funcT<T> f = std_sqrt	  ())	{sqrt = f;	return *this;}
		OperationSet<T>& clear(){return *this = OperationSet<T>();}
		OperationSet<T>& bindSqrtOnPower()
		{
			using namespace std::placeholders;
			sqrt = std::bind(power, _1, 0.5);
			return *this;
		}
		static OperationSet<T> getDefault()
		{
			return OperationSet<T>()
					.withEqual()
					.withComp()
					.withPlus()
					.withMinus()
					.withMulti()
					.withDivide()
					.withPower()
					.withSqrt();
		}
	};
}
#endif
