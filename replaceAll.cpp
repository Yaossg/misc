#include <string>
#include <stdexcept>
/**	
*	replaceAll(str, src, dst, pos = 0, n = npos) -> ret
*	将字符串str内src全部替换成dst并返回 
*	src, dst, ret的类型由str决定 
*	pos为替换的起始位置 pos + n为终止位置 
*/	
template<typename CharType, typename CharTraits, typename Allocator> 
inline std::basic_string<CharType, CharTraits, Allocator> 
replaceAll(std::basic_string<CharType, CharTraits, Allocator> str, 
		std::basic_string<CharType, CharTraits, Allocator> const& src, 
		std::basic_string<CharType, CharTraits, Allocator> const& dst, 
		typename std::basic_string<CharType, CharTraits, Allocator>::size_type pos = 0, 
		typename std::basic_string<CharType, CharTraits, Allocator>::size_type 
			n = std::basic_string<CharType, CharTraits, Allocator>::npos) 
{
	if(str.empty())return str;
	typename std::basic_string<CharType, CharTraits, Allocator>::size_type 
		len = src.length();
	if(len == 0)
		throw std::length_error("src.length() == 0");
	if(n == std::basic_string<CharType, CharTraits, Allocator>::npos)
		n = str.length() - pos;
	for(typename std::basic_string<CharType, CharTraits, Allocator>::size_type 
		 _pos  = str.find(src,  pos);
		 _pos != std::basic_string<CharType, CharTraits, Allocator>::npos
		&& _pos + len - 1 < pos + n;
		 _pos  = str.find(src, _pos))
			  str.replace(_pos, len, dst);
	return str;
}
