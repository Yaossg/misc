#include <iostream>
#include <vector>
#include <cstdio>
#include <map>
#include <cstdint>
#include <sstream>
#include <cmath>

class PrimeEngine {
	std::vector<uintmax_t> primes = init_primes;
	uintmax_t current = 50;
	std::size_t tail = primes.size();
	bool compute(uintmax_t limit, std::size_t tail_limit) {
		for(uintmax_t i = current; i <= current * current; ++i) {
			if(i >= limit || tail > tail_limit) return false;
			if(is_prime(i))
				primes[tail++] = i;
		}
		return current *= current;
	}
public:
	constexpr static std::initializer_list<uintmax_t> 
		init_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
	
	bool is_prime(uintmax_t n) {
		if(n < 2) return false;
		for(uintmax_t i = 0; primes[i] * primes[i] <= n; ++i) 
			if(n % primes[i] == 0) 
				return false;
		return true;
	}
	
	void compute(uintmax_t limit) {
		if(limit <= primes.back()) return;
		double ln = std::log(limit);
		int estimate = limit / (ln - 1.1);
		primes.resize(std::max(estimate, primes.size()));
		while(compute(limit, -1));
		primes.resize(tail);
	}
	
	void computeN(size_t tail_limit) {
		if(tail_limit <= primes.size()) return;
		primes.resize(std::max(tail_limit, primes.size()));
		while(compute(-1, tail_limit));
	}
	
	std::map<uintmax_t, size_t> split(uintmax_t value) {
		if(value < 2) 
			throw std::invalid_argument(std::to_string(value) + " is too small to split");
		std::map<uintmax_t, size_t> ret;
		for(uintmax_t each : primes) {
			while(value % each == 0) {
				++ret[each];
				value /= each;
			}
			if(value == 1) break;
		}
		return ret;
	}
	
	std::ostream& print_split(std::ostream& out, uintmax_t value) {
		out << value << " = ";
		bool first = true;
		for(auto each : split(value)) {
			if (first) first = false;
			else out << " * ";
			out << each.first;
			if(each.second != 1) 
				out << "^" << each.second;
		}
		return out;
	}
	
	std::string get_split(uintmax_t value) {
		std::ostringstream get;
		print_split(get, value);
		return get.str();
	}
	
};

int main() {
	PrimeEngine engine;
	uintmax_t n = 1111177;
	engine.compute(n);
	std::cout << std::boolalpha << engine.is_prime(n) << std::endl;
	std::cout << engine.get_split(n) << std::endl;
}
