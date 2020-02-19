#ifndef DAL_MEANREFLECTOR_HPP
#define DAL_MEANREFLECTOR_HPP

#include "mean.hpp"

namespace data_analysis
{
	struct mean_reflector
	{
		template<typename T>
		static binary_mean_t<T> get_binary(std::string const& str)
		{
			if(str.empty())return binary_mean<T>;
			if(str=="harmonic")return binary_harmonic_mean<T>;
			if(str=="quadratic")return binary_quadratic_mean<T>;
			if(str=="geometric")return binary_geometric_mean<T>;
			throw std::invalid_argument("invalid binary_mean type");
		}
		template<typename It, typename T>
		static mean_t<It, T> get(std::string const& str)
		{
			if(str.empty())return mean<It, T>;
			if(str=="harmonic")return harmonic_mean<It, T>;
			if(str=="quadratic")return quadratic_mean<It, T>;
			if(str=="geometric")return geometric_mean<It, T>;
			throw std::invalid_argument("invalid mean type");
		}
		template<typename It, typename T>
		static weighted_mean_t<It, T> get_weighted(std::string const& str)
		{
			if(str.empty())return weighted_mean<It, T>;
			if(str=="harmonic")return weighted_harmonic_mean<It, T>;
			if(str=="quadratic")return weighted_quadratic_mean<It, T>;
			if(str=="geometric")return weighted_geometric_mean<It, T>;
			throw std::invalid_argument("invalid weighted_mean type");
		}
		
		template<typename T>
		static T call_binary(std::string const& str, T left, T right)
		{
			if(str.empty())return binary_mean(left, right);
			if(str=="harmonic")return binary_harmonic_mean(left, right);
			if(str=="quadratic")return binary_quadratic_mean(left, right);
			if(str=="geometric")return binary_geometric_mean(left, right);
			throw std::invalid_argument("invalid binary_mean type");
		}
		template<typename It, typename T>
		static T call(std::string const& str, It first, It last)
		{
			if(str.empty())return mean(first, last);
			if(str=="harmonic")return harmonic_mean(first, last);
			if(str=="quadratic")return quadratic_mean(first, last);
			if(str=="geometric")return geometric_mean(first, last);
			throw std::invalid_argument("invalid mean type");
		}
		template<typename It, typename T>
		static T call_weighted(std::string const& str, It first, It last, It first2, It last2)
		{
			if(str.empty())return weighted_mean(first, last, first2, last2);
			if(str=="harmonic")return weighted_harmonic_mean(first, last, first2, last2);
			if(str=="quadratic")return weighted_quadratic_mean(first, last, first2, last2);
			if(str=="geometric")return weighted_geometric_mean(first, last, first2, last2);
			throw std::invalid_argument("invalid weighted_mean type");
		}
		
	};
}

#endif
