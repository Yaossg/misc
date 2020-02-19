#ifndef DAL_iterator_invoker_HPP
#define DAL_iterator_invoker_HPP

#include "predef.hpp"

namespace data_analysis
{
	template<typename It>
	struct iterator_invoker
	{
		using difference_type = typename std::iterator_traits<It>::difference_type;
		using value_type = typename std::iterator_traits<It>::value_type;
		using pointer = typename std::iterator_traits<It>::pointer;
		using reference = typename std::iterator_traits<It>::reference;
		using iterator_category = typename std::iterator_traits<It>::iterator_category;
		iterator_invoker() = default;
		explicit iterator_invoker(It it, funcT<value_type> func = nullptr)
			:it(it), func(func) {}
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
