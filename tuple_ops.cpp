#include <type_traits>
#include <tuple>

#ifndef TUPLE_OPS_CPP
#define TUPLE_OPS_CPP

namespace tuple_ops {

template<size_t value>
using sizer = std::integral_constant<size_t, value>;

template<bool value>
using booler = std::integral_constant<bool, value>; 

template<typename Type>
struct typer { using type = Type; }; 

template<typename Type>
struct box : typer<typer<Type>> {};

template<typename Type>
using box_t = typename box<Type>::type;

template<typename Type>
struct unbox : Type {};

template<typename Type>
using unbox_t = typename unbox<Type>::type;

template<size_t Loop, template<typename Type> class Func, typename Type>
struct repeat : repeat<Loop - 1, Func, typename Func<Type>::type> {};

template<template<typename Type> class Func, typename Type>
struct repeat<1, Func, Type> : Func<Type> {};

template<template<typename Type> class Func, typename Type>
struct repeat<0, Func, Type> : Type {};

template<size_t Loop, template<typename Type> class Func, typename Type>
using repeat_t = typename repeat<Loop, Func, Type>::type;

template<typename Type>
struct increase 
  : std::integral_constant<decltype(Type::value), Type::value + 1> {};

template<bool Bool, typename True, typename False>
struct lazy_conditional : False {};

template<typename True, typename False>
struct lazy_conditional<true, True, False> : True {};

template<bool Bool, typename True, typename False>
using lazy_conditional_t = typename lazy_conditional<Bool, True, False>::type;

template<typename Type, typename Tuple> 
struct count {};

template<typename Head, typename... Tail>
struct count<Head, std::tuple<Head, Tail...>>
  : increase<count<Head, std::tuple<Tail...>>> {};

template<typename Head, typename Body, typename... Tail> 
struct count<Head, std::tuple<Body, Tail...>>
  : count<Head, std::tuple<Tail...>> {};

template<typename Head> 
struct count<Head, std::tuple<>> 
  : sizer<0> {};

template<typename Type, typename Tuple> 
constexpr size_t count_v = count<Type, Tuple>::value;

template<template<typename Type> class Pred, typename Tuple>
struct count_if {};

template<template<typename Type> class Pred, typename Head, typename... Tail>
struct count_if<Pred, std::tuple<Head, Tail...>>
  : lazy_conditional<Pred<Head>::value, 
  	  increase<count_if<Pred, std::tuple<Tail...>>>, 
	           count_if<Pred, std::tuple<Tail...>>> {};

template<template<typename Type> class Pred> 
struct count_if<Pred, std::tuple<>> 
  : sizer<0> {};

template<template<typename Type> class Pred, typename Tuple> 
constexpr size_t count_if_v = count_if<Pred, Tuple>::value;

template<size_t Index, typename Type, typename Tuple>
struct find_impl {};

template<size_t Index, typename Head, typename... Tail>
struct find_impl<Index, Head, std::tuple<Head, Tail...>>
  : sizer<Index> {};

template<size_t Index, typename Head, typename Body, typename... Tail>
struct find_impl<Index, Head, std::tuple<Body, Tail...>>
  : find_impl<Index + 1, Head, std::tuple<Tail...>> {};

template<size_t Index, typename Head>
struct find_impl<Index, Head, std::tuple<>>
  : sizer<Index> {};

template<typename Type, typename Tuple>
struct find 
  : find_impl<0, Type, Tuple> {};

template<typename Type, typename Tuple> 
constexpr size_t find_v = find<Type, Tuple>::value;

template<size_t Index, template<typename Type> class Pred, typename Tuple>
struct find_if_impl {};

template<size_t Index, template<typename Type> class Pred, typename Head, typename... Tail>
struct find_if_impl<Index, Pred, std::tuple<Head, Tail...>>
  : lazy_conditional<Pred<Head>::value, sizer<Index>, 
	  find_if_impl<Index + 1, Pred, std::tuple<Tail...>>> {};

template<size_t Index, template<typename Type> class Pred>
struct find_if_impl<Index, Pred, std::tuple<>>
  : sizer<Index> {};

template<template<typename Type> class Pred, typename Tuple>
struct find_if 
  : find_if_impl<0, Pred, Tuple> {};

template<template<typename Type> class Pred, typename Tuple> 
constexpr size_t find_if_v = find_if<Pred, Tuple>::value;

template<typename Tuple>
struct empty {};

template<typename Head, typename... Tail>
struct empty<std::tuple<Head, Tail...>> 
  : std::false_type {};

template<>
struct empty<std::tuple<>> 
  : std::true_type {};

template<typename Tuple> 
constexpr size_t empty_v = empty<Tuple>::value;

template<typename Tuple>
struct front {};

template<typename Head, typename... Tail>
struct front<std::tuple<Head, Tail...>> 
  : typer<Head> {};

template<typename Tuple>
using front_t = typename front<Tuple>::type;

template<typename Tuple>
struct back 
  : std::tuple_element<
	std::tuple_size<Tuple>::value - 1, Tuple> {};

template<typename Tuple>
using back_t = typename back<Tuple>::type;

template<typename Type, typename Tuple>
struct push_front {};

template<typename Head, typename... Tail>
struct push_front<Head, std::tuple<Tail...>> 
  : typer<std::tuple<Head, Tail...>> {};

template<typename Type, typename Tuple>
using push_front_t = typename push_front<Type, Tuple>::type;

template<typename Tuple>
struct pop_front {};

template<typename Head, typename... Tail>
struct pop_front<std::tuple<Head, Tail...>> 
  : typer<std::tuple<Tail...>> {};

template<typename Tuple>
using pop_front_t = typename pop_front<Tuple>::type;

template<typename Type, typename Tuple>
struct push_back {};

template<typename Tail, typename... Head>
struct push_back<Tail, std::tuple<Head...>> 
  : typer<std::tuple<Head..., Tail>> {};

template<typename Type, typename Tuple>
using push_back_t = typename push_back<Type, Tuple>::type;

template<typename Tuple>
struct pop_back {};

template<typename Head, typename Body, typename... Tail>
struct pop_back<std::tuple<Head, Body, Tail...>> 
  : push_front<Head, 
	typename pop_back<std::tuple<Body, Tail...>>::type> {};

template<typename Head>
struct pop_back<std::tuple<Head>> 
  : typer<std::tuple<>> {};

template<typename Tuple>
using pop_back_t = typename pop_back<Tuple>::type;

template<typename Tuple>
using head = front_t<Tuple>;

template<typename Tuple>
using tail = pop_front_t<Tuple>;

template<typename Type>
struct to_tuple {};

template<typename... Types>
struct to_tuple<std::tuple<Types...>> : typer<std::tuple<Types...>> {};

template<typename Head, typename Tail>
struct to_tuple<std::pair<Head, Tail>> : typer<std::tuple<Head, Tail>> {};

template<typename Type, size_t Index>
struct to_tuple<std::array<Type, Index>> 
  : typer<push_front_t<Type, 
      typename to_tuple<std::array<Type, Index - 1>>::type>> {};

template<typename Type>
struct to_tuple<std::array<Type, 1>> 
  : typer<std::tuple<Type>> {};

template<typename Type>
using to_tuple_t = typename to_tuple<Type>::type;

template<typename Tuple>
struct reverse {};

template<typename Head, typename... Tail>
struct reverse<std::tuple<Head, Tail...>> 
  : push_back<Head, 
	typename reverse<std::tuple<Tail...>>::type> {};

template<>
struct reverse<std::tuple<>> 
  : typer<std::tuple<>> {};

template<typename Tuple>
using reverse_t = typename reverse<Tuple>::type;

template<typename Tuple, typename Type>
struct unique_impl {};

template<typename... Types>
struct unique_impl<std::tuple<Types...>, std::tuple<>> : typer<std::tuple<Types...>> {};

template<typename... Types, typename Head, typename... Tail>
struct unique_impl<std::tuple<Types...>, std::tuple<Head, Tail...>> 
  : lazy_conditional<
      find_v<Head, std::tuple<Tail...>> != sizeof...(Tail), 
	    unique_impl<std::tuple<Types...>, std::tuple<Tail...>>, 
	    unique_impl<std::tuple<Head, Types...>, std::tuple<Tail...>>> {};

template<typename Tuple>
struct unique : unique_impl<std::tuple<>, reverse_t<Tuple>> {};

template<typename Tuple>
using unique_t = typename unique<Tuple>::type;

template<template<typename Type> class Func, typename Tuple>
struct transform {};

template<template<typename Type> class Func, typename... Types>
struct transform<Func, std::tuple<Types...>> 
  : typer<std::tuple<typename Func<Types>::type...>> {};

template<template<typename Type> class Func, typename Tuple>
using transform_t = typename transform<Func, Tuple>::type;

template<typename Type, typename Tuple>
struct all_of {};

template<typename Head, typename... Tail>
struct all_of<Head, std::tuple<Head, Tail...>> 
  : all_of<Head, std::tuple<Tail...>> {};

template<typename Head, typename Body, typename... Tail> 
struct all_of<Head, std::tuple<Body, Tail...>>
  : std::false_type {};

template<typename Head> 
struct all_of<Head, std::tuple<>> 
  : std::true_type {};

template<typename Type, typename Tuple>
constexpr bool all_of_v = all_of<Type, Tuple>::value;

template<typename Type, typename Tuple>
struct none_of 
  : booler<find_v<Type, Tuple> == std::tuple_size<Tuple>::value> {};
  
template<typename Type, typename Tuple>
constexpr bool none_of_v = none_of<Type, Tuple>::value;

template<typename Type, typename Tuple>
struct any_of 
  : booler<find_v<Type, Tuple> != std::tuple_size<Tuple>::value> {};

template<typename Type, typename Tuple>
constexpr bool any_of_v = any_of<Type, Tuple>::value;

template<template<typename Type> class Pred, typename Tuple>
struct all_of_if {};

template<template<typename Type> class Pred, typename Head, typename... Tail>
struct all_of_if<Pred, std::tuple<Head, Tail...>>
  : lazy_conditional<Pred<Head>::value, 
  	  all_of_if<Pred, std::tuple<Tail...>>, 
	  std::false_type> {};

template<template<typename Type> class Pred> 
struct all_of_if<Pred, std::tuple<>> 
  : std::true_type {};

template<template<typename Type> class Pred, typename Tuple>
constexpr bool all_of_if_v = all_of_if<Pred, Tuple>::value;

template<template<typename Type> class Pred, typename Tuple>
struct none_of_if 
  : booler<find_if_v<Pred, Tuple> == std::tuple_size<Tuple>::value> {};

template<template<typename Type> class Pred, typename Tuple>
constexpr bool none_of_if_v = none_of_if<Pred, Tuple>::value;

template<template<typename Type> class Pred, typename Tuple>
struct any_of_if 
  : booler<find_if_v<Pred, Tuple> == std::tuple_size<Tuple>::value> {};

template<template<typename Type> class Pred, typename Tuple>
constexpr bool any_of_if_v = any_of_if<Pred, Tuple>::value;

template<typename Src, typename Dst, typename Tuple>
struct replace {};

template<typename Src, typename Dst, typename... Tail>
struct replace<Src, Dst, std::tuple<Src, Tail...>> 
  : push_front<Dst, 
    typename replace<Src, Dst, std::tuple<Tail...>>::type> {};
    
template<typename Src, typename Dst, typename Body, typename... Tail>
struct replace<Src, Dst, std::tuple<Body, Tail...>> 
  : replace<Src, Dst, std::tuple<Tail...>> {};
    
template<typename Src, typename Dst>
struct replace<Src, Dst, std::tuple<>> 
  : typer<std::tuple<>> {};

struct replace_if {};
struct process {};
struct process_if {};

//*未完成* *[已完成]*
//*<未完成>* 
//[replace] replace_if
//process process_if
//<subreplace> <subreplace_if>
//<subprocess> <subprocess_if>
//[transform] <flat> <subtuple>


using T = std::tuple<int,wchar_t,double,wchar_t,int,
	float,wchar_t,char,int,char,wchar_t,int,wchar_t,
	double,wchar_t,int,float,wchar_t,char,int,char,wchar_t>;
static_assert( std::is_same<unique_t<T>, std::tuple<int, wchar_t, double, float, char>>::value , ""); 

using T2 = std::tuple<double, T, unsigned long, float>;
static_assert( find_if_v<std::is_integral, T2> == 2, "");

using T3 = std::tuple<double, unsigned long, float>;
using T4 = std::tuple<double*, unsigned long*, float*>;
static_assert( std::is_same<transform_t<std::add_pointer, T3>, T4>::value, "");

using Tarray = std::array<double, 4>;
using Ttuple = std::tuple<int, int, int, int>;
static_assert( std::is_same<Ttuple, typename replace<double, int, to_tuple_t<Tarray>>::type >::value, "");
static_assert( all_of_if_v<std::is_integral, Ttuple>, "");

using Ttyper = int; 
using Tint = repeat_t<3, box, Ttyper>;
using Tint2 = repeat_t<3, unbox, Tint>;
static_assert( std::is_same<Tint2, int>::value, "");

}

#endif
