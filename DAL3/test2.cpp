#include <iostream>
#include <sstream>
#include <tuple>
#include <cxxabi.h>
using namespace std;

struct empty { };

template <typename T, int N>
  struct bar { };
int main()
{
  int     status;
  char   *realname;

  std::bad_exception  e;
  const std::type_info  &te = typeid(e);
  realname = abi::__cxa_demangle(te.name(), 0, 0, &status);
  std::cout << te.name() << "\t=> " << realname << "\t: " << status << '\n';
  free(realname);

  bar<empty,17>          u;
  const std::type_info  &ti = typeid(u);

  realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
  std::cout << ti.name() << "\t=> " << realname << "\t: " << status << '\n';
  free(realname);
  
  std::tuple<std::bad_exception,bar<std::tuple<bar<std::bad_exception,17>>,17>>          t;
  const std::type_info  &tt = typeid(t);

  realname = abi::__cxa_demangle(tt.name(), 0, 0, &status);
  std::cout << tt.name() << "\t=> " << realname << "\t: " << status << '\n';
  free(realname);
  
	const std::type_info* _233 = &typeid(e);
	
  return 0;
}

