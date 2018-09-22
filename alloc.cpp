#include <cstdlib>
#include <type_traits>

namespace yao { namespace alloc {
	
	template<typename T>
	T* unchecked_alloc() {
		return std::malloc(sizeof(T));
	}
	template<typename T>
	T* alloc() {
		static_assert(!is_abstract<T>::value, "try to alloc abstract object");
		return unchecked_alloc<T>();
	}
	
	template<typename T>
	T* unchecked_calloc(std::size_t size) {
		return std::calloc(sizeof(T), size);
	}
	template<typename T>
	T* calloc(std::size_t size) {
		static_assert(!is_abstract<T>::value, "try to alloc abstract object");
		return unchecked_calloc<T>(size);
	}
	
	template<typename T>
	T* unchecked_realloc(void* ptr) {
		return std::realloc(ptr, sizeof(T));
	}
	template<typename T>
	T* realloc(void* ptr) {
		static_assert(!is_abstract<T>::value, "try to alloc abstract object");
		return unchecked_realloc<T>(ptr);
	}
	
	using std::free;
} }
 
