#include <stdexcept>
#include <limits>
#include <cstdint>
#include <cmath>

namespace yao_math {
class BadRational : public std::domain_error {
public:
	explicit BadRational() : std::domain_error("bad rational: zero denominator") {}
	explicit BadRational( char const *what ) : std::domain_error( what ) {}
};
template<typename IntType>
inline constexpr IntType gcd(IntType x,IntType y) {
	static_assert(::std::numeric_limits<IntType>::is_integer,
				  "gcd<IntType>(),IntType must be integral");
	return y == 0 ? x : gcd(y, x % y);
}
template<typename IntType>
inline constexpr IntType lcm(IntType x,IntType y) {
	static_assert(::std::numeric_limits<IntType>::is_integer,
				  "lcm<IntType>(),IntType must be integral");
	return x * y / gcd(x, y);
}

template<typename IntType>
class Rational {
	static_assert(::std::numeric_limits<IntType>::is_integer,
				  "Rational<IntType>,IntType must be integral");
public:
	typedef IntType int_type;
	constexpr Rational():num(0),den(1) {}
	constexpr Rational(IntType n):num(n),den(1) {}
	constexpr Rational(IntType n,IntType d):num(n),den(d) { normalize(); }
	constexpr Rational(Rational<IntType> const&) = default;
	template<typename NewType>
	constexpr Rational(Rational<NewType> const&r):num(r.num), den(r.den) {}
	constexpr Rational<IntType>& operator =(IntType n) {
		num = n;
		den = 1;
		return *this;
	}
	constexpr void numerator(IntType n) {
		assgin(n, den);
	}
	constexpr IntType numerator()const {
		return num;
	}
	constexpr void denominator(IntType d) {
		assgin(num, d);
	}
	constexpr IntType denominator()const {
		return den;
	}
	constexpr void assign(IntType n) {
		num = n;
		den = 1;
	}
	constexpr void assign(IntType n, IntType d) {
		num = n;
		den = d;
		normalize();
	}
	constexpr Rational<IntType> reciprocal()const {
		return den == IntType(0) ? (throw
									BadRational("bad rational: zero has no reciprocal"), 0)
			   : Rational<IntType>(den,num);
	}
	constexpr bool isNegative()const {
		return num < 0;
	}
	constexpr Rational<IntType> operator+()const {
		return *this;
	}
	constexpr Rational<IntType> operator-()const {
		return Rational<IntType>(-num,den);
	}
	friend constexpr Rational<IntType> operator+(Rational<IntType> const& x,
			Rational<IntType> const& y) {
		return Rational<IntType>(x.num * lcm(x.den,y.den) / x.den + y.num * lcm(x.den,
								 y.den) / y.den,lcm(x.den,y.den));
	}
	friend constexpr Rational<IntType> operator-(Rational<IntType> const& x,
			Rational<IntType> const& y) {
		return Rational<IntType>(x.num * lcm(x.den,y.den) / x.den - y.num * lcm(x.den,
								 y.den) / y.den,lcm(x.den,y.den));
	}
	friend constexpr Rational<IntType> operator*(Rational<IntType> const& x,
			Rational<IntType> const& y) {
		return Rational<IntType>(x.num * y.num, x.den * y.den);
	}
	friend constexpr Rational<IntType> operator/(Rational<IntType> const& x,
			Rational<IntType> const& y) {
		return x * y.reciprocal();
	}
	constexpr Rational<IntType> operator+=(Rational<IntType> const& y) {
		return *this = *this + y;
	}
	constexpr Rational<IntType> operator-=(Rational<IntType> const& y) {
		return *this = *this - y;
	}
	constexpr Rational<IntType> operator*=(Rational<IntType> const& y) {
		return *this = *this * y;
	}
	constexpr Rational<IntType> operator/=(Rational<IntType> const& y) {
		return *this = *this / y;
	}
	friend constexpr bool operator==(Rational<IntType> const& x,
									 Rational<IntType> const& y) {
		return x.num * lcm(x.den,y.den) / x.den == y.num * lcm(x.den,y.den) / y.den;
	}
	friend constexpr bool operator!=(Rational<IntType> const& x,
									 Rational<IntType> const& y) {
		return !(x == y);
	}
	friend constexpr bool operator>(Rational<IntType> const& x,
									Rational<IntType> const& y) {
		return x.num * lcm(x.den,y.den) / x.den > y.num * lcm(x.den,y.den) / y.den;
	}
	friend constexpr bool operator>=(Rational<IntType> const& x,
									 Rational<IntType> const& y) {
		return x.num * lcm(x.den,y.den) / x.den >= y.num * lcm(x.den,y.den) / y.den;
	}
	friend constexpr bool operator<(Rational<IntType> const& x,
									Rational<IntType> const& y) {
		return x.num * lcm(x.den,y.den) / x.den < y.num * lcm(x.den,y.den) / y.den;
	}
	friend constexpr bool operator<=(Rational<IntType> const& x,
									 Rational<IntType> const& y) {
		return x.num * lcm(x.den,y.den) / x.den <= y.num * lcm(x.den,y.den) / y.den;
	}
	friend constexpr Rational<IntType> abs(Rational<IntType> const& x) {
		return Rational<IntType>(std::abs(x.num),x.den);
	}
	constexpr void normalize() {
		IntType zero(0);
		if (den == zero)
			throw BadRational();
		if (num == zero) {
			den = IntType(1);
			return;
		}
		IntType g = gcd(num, den);
		num /= g;
		den /= g;
		if (den < zero) {
			num = -num;
			den = -den;
		}
	}
private:
	IntType num,den;
};

}
