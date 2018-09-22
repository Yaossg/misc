#include <string>
#include <locale>
#include <stdexcept>
#include <cstdint>

namespace yao_integer_toolkit
{
	struct IntBase
	{
		static const int min = 2,bin = 2,oct = 8,dec = 10,hex = 16,max = 36;
		inline static bool check(int base)
		{
			return base < min || base > max;
		}
		template<typename CharT>
		inline static bool is_num(CharT ch,int base = 10)
		{
			CharT const zero('0'),nine('9'),A('A');//cast to CharT
			if(IntBase::check(base))throw std::invalid_argument("invalid integer base");
			if(base > 10) {
				ch = toupper(ch, std::locale());
				return (zero <= ch && ch <= nine) || (A <= ch && ch <= A + base - 10);
			} else {//base <= 10
				return zero <= ch && ch <= zero + base;
			}
		}
	};
	
	namespace basic_cast
	{
		template<typename CharT,typename UIntT>
		std::basic_string<CharT> uint_to_bstr(UIntT integer,int base = 10)
		{
			if(IntBase::check(base))throw std::invalid_argument("invalid integer base");
			if(integer == 0)return std::basic_string<CharT>(1,CharT('0'));
			std::basic_string<CharT> ret;
			while(integer > 0)
			{
				int this_bit = integer % base;
				if(this_bit < 10)
					ret = CharT(this_bit + '0') + ret;
				else//this_bit >= 10
					ret = CharT(this_bit + 'A' - 10) + ret;
				integer /= base;
			}
			return ret;
		}
		
		template<typename UIntT>inline std::string uint_to_str(UIntT integer,int base = 10)
		{return uint_to_bstr<char>(integer,base);}
		template<typename UIntT>inline std::wstring uint_to_wstr(UIntT integer,int base = 10)
		{return uint_to_bstr<wchar_t>(integer,base);}
			
		template<typename UIntT,typename CharT>
		UIntT str_to_uint(CharT const * cp,int base = 10)
		{
			if(IntBase::check(base))throw std::invalid_argument("invalid integer base");
			if(*cp == CharT('\0'))return 0;
			UIntT ret = 0;
			for(;*cp != CharT('\0');++cp)
			{
				CharT this_bit = *cp;
				if (!IntBase::is_num(this_bit,base))throw std::invalid_argument("invalid string");
				ret *= base;
				if(IntBase::is_num(this_bit,10))
					ret += (this_bit - '0');
				else//this_bit >= 10
					ret += (toupper(this_bit, std::locale()) - 'A' + 10);
			}
			return ret;
		}
		
		template<typename CharT>inline uint32_t str_to_uint32(CharT const * cp,int base = 10)
		{return str_to_uint<uint32_t>(cp,base);}
		template<typename CharT>inline uint64_t str_to_uint64(CharT const * cp,int base = 10)
		{return str_to_uint<uint64_t>(cp,base);}
		
		template<typename IntT,typename CharT>
		inline IntT str_to_int(CharT const * cp,int base = 10)
		{
			if(cp[0] == '-')return -str_to_uint<IntT>(++cp,base);
			else 			return str_to_uint<IntT>(cp,base);
		}
		
		template<typename CharT>inline int32_t str_to_int32(CharT const * cp,int base = 10)
		{return str_to_int<int32_t>(cp,base);}
		template<typename CharT>inline int64_t str_to_int64(CharT const * cp,int base = 10)
		{return str_to_int<int64_t>(cp,base);}
	}
} 
