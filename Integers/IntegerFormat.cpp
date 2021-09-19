#include <array>
#include <string>
#include <cstdint>
#include <cinttypes>
#include <cctype>
#include <locale>
#include <cmath>
#include <optional>
#include <type_traits>

#include <iostream>
#include <chrono>

template<typename CharT>
struct Characters {
	CharT pos{'+'};
	CharT neg{'-'};
	CharT zero{'0'};
	CharT a{'a'};
	CharT A{'A'};
	CharT fill{' '};
};

template<typename CharT, 
	typename Traits = std::char_traits<CharT>>
struct Texts {
	using view = std::basic_string_view<CharT, Traits>;
	Characters<CharT> chars;
	view base_name = {}; //进制前缀 
	view separator = {}; //分割字符 
};

struct Options { bool 
	positive_sign : 1, //是否显示正号'+' 
	zero_has_sign : 1, //当value == 0时，0是否有符号(符号取决于positive_sign)
	number_upper_case : 1, //数字(A-Z)是否大写 
	sign_before_base : 1, //符号是否放在进制前面
	sign_take_up_zero : 1, //符号是否算作补位0的一部分 
	base_take_up_zero : 1, //进制是否算作补位0的一部分 
	left_aligned : 1; //是否左对齐 
};

template<typename CharT, 
	typename Traits = std::char_traits<CharT>>
struct Arguments {
	Texts<CharT, Traits> texts = {}; //所需文本 
	Options options = {}; //所需选项 
	int base = 10; //进制 
	std::size_t zerowidth = 0; //补位0的宽度 
	std::size_t spacewidth = 0; //空格补位宽度 
	std::size_t grouping = 0; //分割宽度 
	CharT getA() {
		return options.number_upper_case ? texts.chars.A : texts.chars.a;
	}
};

template<typename Iter, typename CharT>
void raw(Iter last, std::uintmax_t in, int base, CharT zero, CharT A)
{
	if(in == 0)
		*--last = zero;
	else for(size_t bit; in > 0; in /= base) bit = in % base,
		*--last = CharT(bit + (bit < 10 ? zero : A - 10));
}

template<bool Signed>
using choose_type = std::conditional_t<Signed, std::intmax_t, std::uintmax_t>;

template<bool Signed, typename CharT, 
	typename Traits = std::char_traits<CharT>, 
	typename Alloc = std::allocator<CharT>>
std::basic_string<CharT, Traits, Alloc> meta_int_format(
	choose_type<Signed> value,
	Arguments<CharT, Traits> args
) {
	using str_t = std::basic_string<CharT, Traits, Alloc>;
	size_t len = std::log((value < 0 ? -value : value) + 1.0) / std::log(args.base) + 1;
	std::optional<CharT> sign{};
	if(value < 0)
		sign = args.texts.chars.neg;
	else if(value > 0 && args.options.positive_sign) 
		sign = args.texts.chars.pos;
	else if(value == 0 && args.options.zero_has_sign)
		sign = args.options.positive_sign ? args.texts.chars.pos : args.texts.chars.neg;
	if(sign) ++len;
	str_t raw_s(len, sign.value_or(args.texts.chars.zero));
	raw(raw_s.end(), (value < 0 ? -value : value), args.base, args.texts.chars.zero, args.getA());
	size_t count = 0;
	if(args.grouping) for(int index = len - 1; index > (bool)sign; --index) {
		if(++count >= args.grouping) {
			count = 0;
			raw_s.insert(index, args.texts.separator); 
		}
	}
	raw_s.insert(args.options.sign_before_base && sign, args.texts.base_name);
	if(int fillzeros = (args.zerowidth - (sign && args.options.sign_take_up_zero))
		 - (raw_s.length() - (args.options.base_take_up_zero ? 0 : args.texts.base_name.length())
		 - (bool)sign); fillzeros > 0)
		raw_s.insert((bool)sign + args.texts.base_name.length(), str_t(fillzeros, args.texts.chars.zero));
	if(int fillspaces = args.spacewidth - raw_s.length(); fillspaces > 0)
		raw_s.insert(args.options.left_aligned ? 0 : raw_s.length(), str_t(fillspaces, args.texts.chars.fill));
	return raw_s;
}


int main()
{
	using namespace std;
	using namespace std::chrono;
	using namespace std::literals;
	cout<<sizeof(Options)<<endl;
	auto start = high_resolution_clock::now();
	auto finish = high_resolution_clock::now();
	cout<<"$"<<meta_int_format<false, char>(18446744073709551615ull, {})<<endl;
	duration<double, std::milli> take = finish - start;
//	cout<<"It took "<<take.count()<<"ms for 10000times"<<endl;
	
}

