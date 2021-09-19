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
constexpr bool empty_v = empty<Tuple>::value;

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

// following two templates are provided, however, never used in the library,
// because I prefer specialization to these.

template<typename Tuple>
using head = front_t<Tuple>;

template<typename Tuple>
using tail = pop_front_t<Tuple>;

template<size_t Index, typename Type, size_t Pos, typename... Types>
struct insert_impl {};

template<size_t Index, typename Type, size_t Pos, typename Head, typename... Tail>
struct insert_impl<Index, Type, Pos, Head, Tail...>
  : lazy_conditional<Pos != Index,
  push_front<Head, typename insert_impl<Index, Type, Pos + 1, Tail...>::type>,
  push_front<Type, push_front_t<Head, typename insert_impl<Index, Type, Pos + 1, Tail...>::type>>> {};

template<size_t Index, typename Type, size_t Pos>
struct insert_impl<Index, Type, Pos>
  : std::conditional<Pos != Index, std::tuple<>, std::tuple<Type>> {};

template<size_t Index, typename Type, typename Tuple>
struct insert {};

template<size_t Index, typename Type, typename... Types>
struct insert<Index, Type, std::tuple<Types...>> 
  : insert_impl<Index, Type, 0, Types...> {};

template<size_t Index, typename Type, typename Tuple>
using insert_t = typename insert<Index, Type, Tuple>::type;

template<size_t Index, size_t Pos, typename... Types>
struct remove_impl {};

template<size_t Index, size_t Pos, typename Head, typename... Tail>
struct remove_impl<Index, Pos, Head, Tail...> 
  : lazy_conditional<Pos != Index, 
  push_front<Head, typename remove_impl<Index, Pos + 1, Tail...>::type>,
  remove_impl<Index, Pos + 1, Tail...>> {};

template<size_t Index, size_t Pos>
struct remove_impl<Index, Pos> : typer<std::tuple<>> {};

template<size_t Index, typename Tuple>
struct remove {};

template<size_t Index, typename... Types>
struct remove<Index, std::tuple<Types...>> 
  : remove_impl<Index, 0, Types...> {};
  
template<size_t Index, typename Tuple>
using remove_t = typename remove<Index, Tuple>::type;

template<typename Type, typename... Types>
struct remove_all_impl {};

template<typename Type, typename... Tail>
struct remove_all_impl<Type, Type, Tail...>
  : remove_all_impl<Type, Tail...> {};

template<typename Type, typename Head, typename... Tail>
struct remove_all_impl<Type, Head, Tail...>
  : push_front<Head, typename remove_all_impl<Type, Tail...>::type> {};

template<typename Type>
struct remove_all_impl<Type> : typer<std::tuple<>> {};

template<typename Type, typename Tuple>
struct remove_all {};

template<typename Type, typename... Types>
struct remove_all<Type, std::tuple<Types...>> 
  : remove_all_impl<Type, Types...> {};

template<typename Type, typename Tuple>
using remove_all_t = typename remove_all<Type, Tuple>::type;

template<template<typename Type> class Pred, typename... Types>
struct remove_if_impl {};

template<template<typename Type> class Pred, typename Head, typename... Tail>
struct remove_if_impl<Pred, Head, Tail...>
  : lazy_conditional<Pred<Head>::value, 
      remove_if_impl<Pred, Tail...>
      push_front<Head, typename remove_if_impl<Pred, Tail...>::type>> {};

template<template<typename Type> class Pred>
struct remove_if_impl<Pred> : typer<std::tuple<>> {};

template<template<typename Type> class Pred, typename Tuple>
struct remove_if {};

template<template<typename Type> class Pred, typename... Types>
struct remove_if<Pred, std::tuple<Types...>> 
  : remove_if_impl<Pred, Types...> {};

template<template<typename Type> class Pred, typename Tuple>
using remove_if_t = typename remove_all<Pred, Tuple>::type;

template<typename Tuple>
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
  : push_front<Body, 
    typename replace<Src, Dst, std::tuple<Tail...>>::type> {};
    
template<typename Src, typename Dst>
struct replace<Src, Dst, std::tuple<>> 
  : typer<std::tuple<>> {};

template<typename Src, typename Dst, typename Tuple>
using replace_t = typename replace<Src, Dst, Tuple>::type;

template<template<typename Type> class Pred, typename Dst, typename Tuple>
struct replace_if {};

template<template<typename Type> class Pred, typename Dst, typename Body, typename... Tail>
struct replace_if<Pred, Dst, std::tuple<Body, Tail...>> 
  : push_front<lazy_conditional_t<Pred<Body>::value, typer<Dst>, typer<Body>>, 
    typename replace_if<Pred, Dst, std::tuple<Tail...>>::type> {};
	    
template<template<typename Type> class Pred, typename Dst>
struct replace_if<Pred, Dst, std::tuple<>> 
  : typer<std::tuple<>> {};

template<template<typename Type> class Pred, typename Dst, typename Tuple>
using replace_if_t = typename replace_if<Pred, Dst, Tuple>::type;

template<typename Src, template<typename Type> class Func, typename Tuple>
struct process {};

template<typename Src, template<typename Type> class Func, typename... Tail>
struct process<Src, Func, std::tuple<Src, Tail...>> 
  : push_front<typename Func<Src>::type, 
    typename process<Src, Func, std::tuple<Tail...>>::type> {};
    
template<typename Src, template<typename Type> class Func, typename Body, typename... Tail>
struct process<Src, Func, std::tuple<Body, Tail...>> 
  : push_front<Body, 
    typename process<Src, Func, std::tuple<Tail...>>::type> {};
    
template<typename Src, template<typename Type> class Func>
struct process<Src, Func, std::tuple<>> 
  : typer<std::tuple<>> {};

template<typename Src, template<typename Type> class Func, typename Tuple>
using process_t = typename process<Src, Func, Tuple>::type;

template<template<typename Type> class Pred, template<typename Type> class Func, typename Tuple>
struct process_if {};
    
template<template<typename Type> class Pred, template<typename Type> class Func, typename Body, typename... Tail>
struct process_if<Pred, Func, std::tuple<Body, Tail...>> 
  : push_front<lazy_conditional_t<Pred<Body>::value, Func<Body>, typer<Body>>, 
    typename process_if<Pred, Func, std::tuple<Tail...>>::type> {};
  
template<template<typename Type> class Pred, template<typename Type> class Func>
struct process_if<Pred, Func, std::tuple<>> 
  : typer<std::tuple<>> {};
  
template<template<typename Type> class Pred, template<typename Type> class Func, typename Tuple>
using process_if_t = typename process_if<Pred, Func, Tuple>::type;

template<typename Head, typename Body, typename... Tail>
struct connect {};

template<typename... Head, typename Body, typename... Tail>
struct connect<std::tuple<Head...>, Body, Tail...>
  : connect<std::tuple<Head...>, typename connect<Body, Tail...>::type> {};

template<typename... Head, typename... Tail>
struct connect<std::tuple<Head...>, std::tuple<Tail...>>
  : typer<std::tuple<Head..., Tail...>> {};

template<typename Head, typename Body, typename... Tail>
using connect_t = typename connect<Head, Body, Tail...>::type;

template<typename Tuple>
struct flat {};

template<typename Head, typename... Tail>
struct flat<std::tuple<Head, Tail...>>
  : push_front<Head, typename flat<std::tuple<Tail...>>::type> {};

template<typename Head, typename... Body, typename... Tail>
struct flat<std::tuple<std::tuple<Head, Body...>, Tail...>>
  : connect<std::tuple<Head, Body...>, typename flat<std::tuple<Tail...>>::type> {};

template<typename... Tail>
struct flat<std::tuple<std::tuple<>, Tail...>>
  : flat<std::tuple<Tail...>> {};
  
template<>
struct flat<std::tuple<>>
  : typer<std::tuple<>> {};

template<typename Tuple>
using flat_t = typename flat<Tuple>::type;

template<typename Tuple>
struct deep_flat {};

template<typename Head, typename... Tail>
struct deep_flat<std::tuple<Head, Tail...>>
  : push_front<Head, typename deep_flat<std::tuple<Tail...>>::type> {};
  
template<typename Head, typename... Body, typename... Tail>
struct deep_flat<std::tuple<std::tuple<Head, Body...>, Tail...>>
  : connect<typename deep_flat<std::tuple<Head, Body...>>::type, typename deep_flat<std::tuple<Tail...>>::type> {};

template<typename... Tail>
struct deep_flat<std::tuple<std::tuple<>, Tail...>>
  : flat<std::tuple<Tail...>> {};
  
template<>
struct deep_flat<std::tuple<>>
  : typer<std::tuple<>> {};
  
template<typename Tuple>
using deep_flat_t = typename deep_flat<Tuple>::type;

}

#endif
