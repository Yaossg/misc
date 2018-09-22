#include <cstddef>
#include <initializer_list>
#include <algorithm>

namespace yao_ptr
{
	class PointerCounter
	{
	public:
		using CounterType = long;
		PointerCounter():cp(new CounterType(1)){}
		PointerCounter(const PointerCounter& other):cp(other.cp){++*cp;}
		PointerCounter(PointerCounter&& other):cp(std::exchange(other.cp, nullptr)){}
		~PointerCounter(){if(--*cp == 0)delete cp;}
		inline CounterType count(){return *cp;}
	private:
		CounterType* cp;
	};
	
	template<typename T>
	class CountedPointer
	{
	public:
		typedef T value_type;
		CountedPointer() = default;
		CountedPointer(const CountedPointer&) = default;
		CountedPointer(CountedPointer&&) = default;
		CountedPointer(T* ptr): ptr(ptr) {}
		CountedPointer& operator=(const CountedPointer& other)
		{
			this -> ~CountedPointer();
			ptr = other.ptr;
			ptr_c = other.ptr_c;
			return *this;
		}
		CountedPointer& operator=(CountedPointer&& other)
		{
			this -> ~CountedPointer();
			ptr = std::exchange(other.ptr, nullptr);
			ptr_c = std::move(other.ptr_c);
			return *this;
		}
				T& operator*()		{return *ptr;}
		const 	T& operator*() const{return *ptr;}
				T* operator->()		{return ptr;}
		const 	T* operator->()const{return ptr;}
		
		bool operator ==(const CountedPointer& other){return ptr == other.ptr;}
		bool operator !=(const CountedPointer& other){return ptr != other.ptr;}
		
		~CountedPointer(){if(ptr_c.count() == 1)delete ptr;}
	private:
		T* ptr = nullptr;
		PointerCounter ptr_c;
	};
}
