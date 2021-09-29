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
	size_t count = 0;
	/* count	current
	 * n		50^(2^n)
	 */
	size_t tail = primes.size();
	bool compute(uintmax_t below, size_t capcity) {
		// here we assume a uintmax_t has 64 bits
		uintmax_t limit = count == 3 ? UINTMAX_MAX : current * current;
		for(uintmax_t i = current; i <= limit; ++i) {
			if(i >= below || tail > capcity) return false;
			if(is_prime(i))
				primes[tail++] = i;
		}
		if (count < 3) {
			++count;
			current *= current;
		}
		return true;
	}
public:
	constexpr static std::initializer_list<uintmax_t> 
		init_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
	
	bool is_prime(uintmax_t n) {
		switch(n) {
			case 0: case 1: return false;
			case 2: case 3: case 5: case 7: return true;
			default: if(n & 1 == 0) return false; // shortcut for even numbers
		}
		for(uintmax_t i = 1 /*skipping 2*/ ; primes[i] * primes[i] <= n; ++i) 
			if(n % primes[i] == 0) 
				return false;
		return true;
	}
	
	void computeBelow(uintmax_t below) {
		if(below <= primes.back()) return;
		double ln = std::log(below);
		size_t estimate = below / (ln - 1.1);
		primes.resize(std::max(estimate, primes.size()));
		while(compute(below, -1));
		primes.resize(tail);
	}
	
	void computeCapcity(size_t capcity) {
		if(capcity <= primes.size()) return;
		primes.resize(std::max(capcity, primes.size()));
		while(compute(-1, capcity));
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
	engine.computeBelow(n);
	std::cout << std::boolalpha << engine.is_prime(n) << std::endl;
	std::cout << engine.get_split(n) << std::endl;
}
