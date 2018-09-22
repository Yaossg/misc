#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>
#include <initializer_list>

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

namespace yao_temperature
{
	class Temperature
	{
	public:
		typedef char unit_t;
		static const unit_t K='K';
		static const unit_t C='C';
		static const unit_t F='F';

		Temperature():value(0.0){}
		
		Temperature(const Temperature& t):value(t.value){}
		
		#if __cplusplus >= 201103L//c++11
		Temperature(Temperature&& t) = default;
		#endif
		
		explicit Temperature(double t,unit_t unit = K)
		{set(t,unit);}
		
		const Temperature& set(double t,unit_t unit = K)
		{
			switch(unit)
			{
				case K:
					value = t;
					break;
				case C:
					value = t + 273.15;
					break;
				case F:
					value = 5.0 / 9.0 * (t + 459.67);
					break;
				default:
					throw std::runtime_error("yao_temperature::Temperature:wrong unit" + unit);
			}
			return *this;
		}
		
		double get(unit_t unit = K) const
		{
			switch(unit)
			{
				case K:
					return value;
					break;
				case C:
					return value - 273.15;
					break;
				case F:
					return 9.0 / 5.0 * value - 459.67;
					break;
				default:
					throw std::runtime_error("yao_temperature::Temperature:wrong unit:" + unit);
			}
		}

		const Temperature& operator =(const Temperature& t)
		{
			value = t.value;
			return *this;
		}
		#if __cplusplus >= 201103L//c++11
		Temperature& operator =(Temperature&& t) = default;
		#endif
		inline bool operator >(const Temperature& t) const
		{
            return value > t.value;
		}
		inline bool operator <(const Temperature& t) const
		{
			return value < t.value;
		}
		inline bool operator >=(const Temperature& t) const
		{
			return value >= t.value;
		}
		inline bool operator <=(const Temperature& t) const
		{
			return value<=t.value;
		}
		inline bool operator ==(const Temperature& t) const
		{
			return value == t.value;
		}
		inline bool operator !=(const Temperature& t) const
		{
			return value != t.value;
		}
		
		template<typename CharT>
		std::basic_string<CharT> to_basic_string(unit_t unit = K)
		{
			std::basic_stringstream<CharT> ss;
			ss<<get(unit)<<unit;
			return ss.str();
		}
		std::string to_string(unit_t unit = K)
		{
			return to_basic_string<char>(unit);
		}
		std::wstring to_wstring(unit_t unit = K)
		{
			return to_basic_string<wchar_t>(unit);
		}
		
	 	private:
			double value;
	};
}
#endif

