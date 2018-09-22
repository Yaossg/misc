#include "variance.hpp" 
/*
template<typename T>
struct Optional
{
	Optional():p(nullptr){}
	Optional(T v):p(new T(v)){};
	bool empty()const{return p == nullptr;}
	T get()const{return *p;}
	T set(T v)const{this->~Optional();p=new T(v);}
	~Optional(){if(p!=nullptr)delete p;}
	T* p;
};
*/
namespace data_analysis
{
	template<typename T>
	struct DataAnalysisResult
	{
	
	};
	template<typename InIt, typename T>
	class DataAnalysis
	{
		typename InIt::difference_type dist;
	public:
		InIt first, last;
		OperationSet<T> op;
		DataAnalysis(InIt _first, InIt _last, 
			OperationSet<T> const& _op)
		:first(_first), last(_last), op(_op), 
		dist(std::distance(_first,_last)) 
		{
			if(first == last)throw EmptyRange();
		}
		
	};	
}
