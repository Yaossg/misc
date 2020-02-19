#include <iostream>
#include <sstream>

#include "DAL4.hpp"

using namespace std;
using namespace data_analysis;

void test_mean() {
    auto a = {100., 50.}, b = {1., 2.};
    auto c = {4., 6.};
    cout << weighted_mean(a.begin(), a.end(), b.begin(), b.end()) << endl;
    cout << harmonic_mean(c.begin(), c.end()) << endl;
    cout << weighted_quadratic_mean(a.begin(), a.end(), b.begin(), b.end()) << endl;
    cout << weighted_geometric_mean(a.begin(), a.end(), b.begin(), b.end()) << endl;
}


void test_mode() {
    auto a = {100, 10, 100};
    for (auto e : mode(a.begin(), a.end()))
        cout << e << endl;
}

int main()
{
    test_mean();
	test_mode();
	return 0;
}

