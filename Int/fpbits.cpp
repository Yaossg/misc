#include <cstring>
#include <limits>
#include <type_traits>

namespace yao_math {

typedef unsigned long long bitfield;

template<typename F>
struct FPBits {
    static_assert(std::is_same_v<F, float> || std::is_same_v<F, double>, "only float and double is supported");
    constexpr static int FRACTION = std::numeric_limits<F>::digits - 1;
    constexpr static int EXPONENT = (sizeof(F) << 3) - FRACTION - 1;
    constexpr static int EXPONENT_OFFSET = std::numeric_limits<F>::max_exponent - 1;
    bitfield fraction: FRACTION;
    bitfield exponent: EXPONENT;
    bitfield sign: 1;
    FPBits(F value = 0) {
        *(F*)this = value;
    }
    operator F() const {
        return *(F*)this;
    }
    
    // impl note: log2 is inappropriate for subnormal floating point
    int log2() const {
        return exponent - EXPONENT_OFFSET;
    }
    void exp2(int n) {
        exponent = EXPONENT_OFFSET + n;
    }

    // returns the fraction with leading one
    bitfield full_fraction() const {
        return fraction | (bitfield(1) << FRACTION);
    }
    // returns the exponent without fraction
    int partial_log2() const {
        return log2() - FRACTION;
    }
    
    // based on the two functions above, 
    // the value of the floating point is equivalent to
    //       full_fraction() << +partial_log2()
    //   or  full_fraction() >> -partial_log2()
    // in integral semantics
};

}