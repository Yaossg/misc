#include <map>
#include <set>
#include <string>
#include <iterator>
#include <numeric>
#include <cmath>

namespace yao_math { 

template<typename Int>
Int fastIntPow(Int a, Int n) {
	Int r = 1;
	while(n & 1 && (r *= a, 0), n && (a *= a, 0), n >>= 1);
	return r;
}

template<typename Int>
Int gcd(Int x, Int y) {
	return y == 0 ? x : gcd(y, x % y);
}

template<typename Int>
class Monomial {
	std::map<std::string, Int> core;
public:
	Monomial(): core{} {}
	Monomial(std::map<std::string, Int> core): core{core} {}
	friend bool operator <(Monomial const& a, Monomial const& b) {
		if (auto cmp = a.degree() - b.degree()) return cmp > 0;
		return a.core < b.core;	
	}
	friend Monomial operator *(Monomial const& a, Monomial const& b) {
		Monomial r = a;
		for (auto const& [a, n] : b.core)
			r.core[a] += n;
		return r;
	}
	std::string toString() const {
		std::string r;
		for(auto const& [a, n] : core) {
			r += a;
			if (n > 1) {
				r += "^";
				auto exp = std::to_string(n);
				if (exp.length() > 1) exp = "{" + exp + "}";
				r += exp;
			}
		}
		return r;
	}
	bool empty() const { return core.empty(); }
	void erase(std::string name) { core.erase(name); }
	bool contains(std::string name) const {
		return core.find(name) != core.end();
	}
	Int at(std::string name) const {
		return core.at(name);
	}
	Int degree() const {
		Int m = 0;
		for (auto const& [a, n] : core)
			m = std::max(m, n);
		return m;
	}
};

template<typename Int>
class RatioExpr;

template<typename Int>
class IntExpr {
	friend class RatioExpr<Int>;
	using mono = Monomial<Int>;
	using core_t = std::map<mono, Int>;
	core_t core;
	IntExpr(core_t core): core{core} {}
	
	void div(Int g) {
		for(auto& [x1, k1] : core) {
			k1 /= g;
		}
	}
public:
	IntExpr(): core{{ {}, 0 }} {}
	IntExpr(Int C): core{{ {}, C }} {}
	IntExpr(std::string a, Int n = 1): core{{ { {{a, n}} }, 1 }} {}
	IntExpr(mono mono, Int C): core{{ mono, C }} {}
	
	IntExpr operator+() const {
		return *this;
	}
	
	IntExpr operator-() const {
		return *this * -1;
	}
	
	IntExpr& operator+=(IntExpr const& other) {
		for(auto const& [x1, k1] : other.core) {
			core[x1] += k1;
			if(core[x1] == 0)
				core.erase(x1);
		}
		return *this;
	}
	
	friend IntExpr operator+(IntExpr const& a, IntExpr const& b) {
		IntExpr r(a);
		return r += b;
	}
	
	IntExpr& operator-=(IntExpr const& other) {
		return *this += -other;
	}
	
	friend IntExpr operator-(IntExpr const& a, IntExpr const& b) {
		return a + -b;
	}
	
	IntExpr& operator*=(IntExpr const& other) {
		return *this = *this * other;
	}	
	
	friend IntExpr operator*(IntExpr const& a, IntExpr const& b) {
		IntExpr r;
		for(auto const& [x1, k1] : a.core) {
			for(auto const& [x2, k2] : b.core) {
				auto k = k1 * k2;
				if (k)
					r += {x1 * x2, k};
			}
		}
		return r;
	}

	std::string toString() const {
		std::string r;
		Int C;
		for(auto const& [x1, k1] : core) {
			if (x1.empty()) {
				C = k1;
				continue;
			}
			if (!r.empty() && k1 >= 0) 
				r += "+";
			
			if (k1 != 1) {
				if (k1 == -1)
					r += "-";
				else 
					r += std::to_string(k1);
			}
			r += x1.toString();	
		}
		if (C != 0) {
			if (!r.empty() && C >= 0) 
				r += "+";
			r += std::to_string(C);
		}
		return r;
	}
	
	IntExpr eval(std::string name, Int value) const {
		return eval(name, IntExpr{value});
	}
	
	IntExpr eval(std::string name, IntExpr expr) const {
		core_t ret = core, buf;
		for (auto it = ret.begin(); it != ret.end(); ) {
			if (it->first.contains(name)) {
				buf.insert(*it);
				it = ret.erase(it);
			} else {
				++it;
			}
		}
		IntExpr ans{ret};
		for (auto [x0, k] : buf) {
			mono x = x0;
			Int n = x.at(name);
			x.erase(name);
			ans += IntExpr(x, k) * pow(expr, n);
		}
		return ans;
	}
	
	
	RatioExpr<Int> eval(std::string name, RatioExpr<Int> expr) const {
		core_t ret = core, buf;
		for (auto it = ret.begin(); it != ret.end();) {
			if (it->first.contains(name)) {
				buf.insert(*it);
				it = ret.erase(it);
			} else {
				++it;
			}
		}
		RatioExpr<Int> ans{IntExpr<Int>{ret}, 1};
		for (auto [x0, k] : buf) {
			mono x = x0;
			Int n = x.at(name);
			x.erase(name);
			ans += IntExpr(x, k) * pow(expr, n);
		}
		return ans;
	}
	
	Int gcd() const {
		Int ret = 0;
		for(auto const& [x1, k1] : core) {
			ret = gcd(k1, ret);
		}
		return ret;
	}
};

template<typename Int>
IntExpr<Int> pow(IntExpr<Int> a, Int n) {
	IntExpr<Int> r = 1;
	while(n & 1 && (r *= a, 0), n && (a *= a, 0), n >>= 1);
	return r;
}

template<typename Int>
class RatioExpr {
	IntExpr<Int> num, den;
	void normalize() {
		Int g = gcd(num.gcd(), den.gcd());
		num.div(g); den.div(g);
	}
public:
	RatioExpr(): num{0}, den{1} {}
	RatioExpr(Int C): num{C}, den{1} {}
	RatioExpr(IntExpr<Int> num, IntExpr<Int> den = 1)
		: num{num}, den{den} { normalize(); }
	
	RatioExpr operator+() const {
		return *this;
	}
	
	RatioExpr operator-() const {
		return operator*(operator+(), {-1, 1});
	}
	
	friend RatioExpr operator+(RatioExpr const& a, RatioExpr const& b) {
		return {a.num * b.den + b.num * a.den, a.den * b.den};
	}
	
	RatioExpr& operator+=(RatioExpr const& other) {
		return *this = *this + other;
	}	
	
	RatioExpr& operator-=(RatioExpr const& other) {
		return *this += -other;
	}
	
	friend RatioExpr operator-(RatioExpr const& a, RatioExpr const& b) {
		return a +- b;
	}
	
	RatioExpr& operator*=(RatioExpr const& other) {
		return *this = *this * other;
	}
	
	friend RatioExpr operator*(RatioExpr const& a, RatioExpr const& b) {
		return {a.num * b.num, a.den * b.den};
	}
	
	RatioExpr inverse() const {
		return {den, num};
	}
	
	RatioExpr& operator/=(RatioExpr const& other) {
		return *this *= other.inverse();
	}
	
	friend RatioExpr operator/(RatioExpr const& a, RatioExpr const& b) {
		return a * b.inverse();
	}
	
	RatioExpr eval(std::string name, RatioExpr value) const {
		return num.eval(name, value) / den.eval(name, value);
	}
	
	std::string toString() const {
		return "\\frac{" + num.toString() + "}{" + den.toString() + "}";
	}
	
};

template<typename Int>
RatioExpr<Int> operator/(IntExpr<Int> const& a, IntExpr<Int> const& b) {
	return {a, b};
}

template<typename Int>
RatioExpr<Int> pow(RatioExpr<Int> a, int n) {
	RatioExpr<Int> r = 1;
	while(n & 1 && (r *= a, 0), n && (a *= a, 0), n >>= 1);
	return r;
}

inline namespace literals { 
inline namespace expr_literals {
IntExpr<int> operator"" _e(unsigned long long n) {
	return n;
}
IntExpr<long> operator"" _eL(unsigned long long n) {
	return n;
}
IntExpr<long long> operator"" _eLL(unsigned long long n) {
	return n;
}
IntExpr<int> operator"" _e(const char* chars, size_t) {
	return {chars};
}
IntExpr<long> operator"" _eL(const char* chars, size_t) {
	return {chars};
}
IntExpr<long long> operator"" _eLL(const char* chars, size_t) {
	return {chars};
}
}
}
}
