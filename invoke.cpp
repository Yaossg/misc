#include <type_traits>
#include <functional>
#include <tuple>
namespace cxx17std {
	namespace detail {
		template <typename T>
		struct is_reference_wrapper : std::false_type {};
		template <typename U>
		struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
		 
		template <typename Base, typename T, typename Derived, typename... Args>
		auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
		noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
		 -> typename std::enable_if<std::is_function<T>::value &&
				std::is_base_of<Base, typename std::decay<Derived>::type>::value,
		    decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>::type {
			return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
		}
		template <typename Base, typename T, typename RefWrap, typename... Args>
		auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
		noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
		 -> typename std::enable_if<std::is_function<T>::value &&
			    is_reference_wrapper<typename std::decay<RefWrap>::type>::value,
		    decltype((ref.get().*pmf)(std::forward<Args>(args)...))>::type {
			return (ref.get().*pmf)(std::forward<Args>(args)...);
		}
		template <typename Base, typename T, typename Pointer, typename... Args>
		auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
		noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
		 -> typename std::enable_if<std::is_function<T>::value &&
				!is_reference_wrapper<typename std::decay<Pointer>::type>::value &&
				!std::is_base_of<Base, typename std::decay<Pointer>::type>::value,
		    decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>::type {	
			return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
		}
		template <typename Base, typename T, typename Derived>
		auto INVOKE(T Base::*pmd, Derived&& ref)
		noexcept(noexcept(std::forward<Derived>(ref).*pmd))
		 -> typename std::enable_if<!std::is_function<T>::value &&
				std::is_base_of<Base, typename std::decay<Derived>::type>::value,
		    decltype(std::forward<Derived>(ref).*pmd)>::type {
			return std::forward<Derived>(ref).*pmd;
		}
		template <typename Base, typename T, typename RefWrap>
		auto INVOKE(T Base::*pmd, RefWrap&& ref)
		noexcept(noexcept(ref.get().*pmd))
		 -> typename std::enable_if<!std::is_function<T>::value &&
				is_reference_wrapper<typename std::decay<RefWrap>::type>::value,
		    decltype(ref.get().*pmd)>::type {
			return ref.get().*pmd;
		}
		template <typename Base, typename T, typename Pointer>
		auto INVOKE(T Base::*pmd, Pointer&& ptr)
		noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
		 -> typename std::enable_if<!std::is_function<T>::value &&
				!is_reference_wrapper<typename std::decay<Pointer>::type>::value &&
				!std::is_base_of<Base, typename std::decay<Pointer>::type>::value,
		    decltype((*std::forward<Pointer>(ptr)).*pmd)>::type {
			return (*std::forward<Pointer>(ptr)).*pmd;
		}
		template <typename F, typename... Args>
		auto INVOKE(F&& f, Args&&... args)
		noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
		 -> typename std::enable_if<!std::is_member_pointer<typename std::decay<F>::type>::value,
		    decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type {
			return std::forward<F>(f)(std::forward<Args>(args)...);
		}
		
		template <typename AlwaysVoid, typename, typename...>
		struct invoke_result {};
		template <typename F, typename... Args>
		struct invoke_result<decltype(void(/*detail::*/INVOKE(std::declval<F>(), 
				std::declval<Args>()...))), F, Args...> {
		    using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
		};
		
		template <typename AlwaysVoid, typename, typename...>
		struct is_invocable : std::false_type {};
		template <typename F, typename... Args>
		struct is_invocable<decltype(void(INVOKE(std::declval<F>(), 
				std::declval<Args>()...))), F, Args...> : std::true_type {};
		
		template <bool cond, typename F, typename... Args>
		struct is_nothrow_invocable : std::false_type {};
		template <typename F, typename... Args>
		struct is_nothrow_invocable<true, F, Args...> : std::integral_constant<bool, 
				noexcept(INVOKE(std::declval<F>(), std::declval<Args>()...))> {};
			
	}  // namespace detail
	template <typename F, typename... Args>
	struct invoke_result : detail::invoke_result<void, F, Args...> {};
	template<typename F, typename... Args>
	using invoke_result_t = typename invoke_result<F, Args...>::type;
	
	template <typename F, typename... Args>
	struct is_invocable : detail::is_invocable<void, F, Args...> {};
	template <typename R, typename F, typename... Args>
	struct is_invocable_r : std::integral_constant<bool, 
		is_invocable<F, Args...>::value && 
			std::is_convertible<invoke_result_t<F, Args...>, R>::value> {};
	template <typename F, typename... Args>
	struct is_nothrow_invocable : 
		detail::is_nothrow_invocable<is_invocable<F, Args...>::value, F, Args...> {};
	template <typename R, typename F, typename... Args>
	struct is_nothrow_invocable_r : std::integral_constant<bool, 
		is_nothrow_invocable<F, Args...>::value && 
			std::is_convertible<invoke_result_t<F, Args...>, R>::value> {};
			
	template<typename F, typename... Args>
	invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) 
		noexcept(is_nothrow_invocable<F, Args...>::value) {
	    return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
	}
	
	template <typename T, T... Index>
	struct integer_sequence {
		using value_type = T;
		static constexpr size_t size() { return sizeof...(Index); }
	};
	namespace detail {
		template <size_t... Index>
		struct index_tuple {
			using next = index_tuple<Index..., sizeof...(Index)>;
		};
		template <size_t N>
		struct build_index_tuple {
			using type = typename build_index_tuple<N - 1>::type::next;
		};
		template <>
		struct build_index_tuple<0> {
			using type = index_tuple<>;
		};
		template <typename T, T N,
			typename seq = typename build_index_tuple<N>::type>
		struct make_integer_sequence_impl;
		template <typename T, T N,  size_t... Index>
		struct make_integer_sequence_impl<T, N, index_tuple<Index...>> {
			static_assert(N >= 0, "Cannot make integer sequence of negative length");
			using type = integer_sequence<T, static_cast<T>(Index)...>;
		};
	}  // namespace detail
	template <typename T, T N>
	using make_integer_sequence
		= typename detail::make_integer_sequence_impl<T, N>::type;
	template <size_t... Index>
	using index_sequence = integer_sequence<size_t, Index...>;
	template <size_t N>
	using make_index_sequence = make_integer_sequence<size_t, N>;
	template <typename... Args>
	using index_sequence_for = make_index_sequence<sizeof...(Args)>;
	namespace detail {
		template <typename F, typename Tuple, std::size_t... I>
		constexpr auto apply_impl(F&& f, Tuple&& t, index_sequence<I...>)
			-> decltype(invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...)) {
		    return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
		}
	}  // namespace detail
	template <typename F, typename Tuple>
	constexpr auto apply(F&& f, Tuple&& t)
		-> decltype(detail::apply_impl(
	        std::forward<F>(f), std::forward<Tuple>(t),
	        make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{})) {
	    return detail::apply_impl(
	        std::forward<F>(f), std::forward<Tuple>(t),
	        make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
	}
}