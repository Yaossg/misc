#include <iostream>
using namespace std;
#include "expr.cpp"
#include <fstream>

using namespace yao_math;
using namespace yao_math::expr_literals;

int main() {
	auto a = "a"_e, b = "b"_e, c = "c"_e, x = "x"_e;
	
	auto e1 = (x+1)*(x+2)*(x+3);
	cout << e1.toString() << endl;
	
	return 0;
}
