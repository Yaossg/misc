/**
*	DAL_iterator_invoker.hpp
*	作者：Yaossg 
*	迭代器调用器 
*	std=C++11/14/17
*/
#ifndef DAL_iterator_invoker_HPP
#define DAL_iterator_invoker_HPP
#include "predef.hpp"
namespace data_analysis
{
	template<typename It>
	struct iterator_invoker
	{
		using difference_type = typename It::difference_type;
		using value_type = typename It::value_type;
		using pointer = typename It::pointer;
		using reference = typename It::reference;
		using iterator_category = typename It::iterator_category;
		iterator_invoker() {}
		iterator_invoker(It init, funcT<value_type> f = nullptr)
			:it(init), func(f) {}
		It it;
		funcT<value_type> func;
		value_type operator*()const
		{
			return func(*it);
		}
		iterator_invoker operator++()
		{
			return {++it,func};
		}
		iterator_invoker operator++(int)
		{
			return {it++,func};
		}
		bool operator==(iterator_invoker const& r)const 
		{
			return it == r.it;
		}
		bool operator!=(iterator_invoker const& r)const
		{
			return it != r.it;
		}
		
	};
}
#endif
