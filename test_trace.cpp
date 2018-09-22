//test part
#include <iostream>

struct trace
{
static int cons_n;
static int dest_n; 
static int copy_n;
static int move_n;
trace(){cons_n++;}
~trace(){dest_n++;}
trace(trace const&){copy_n++;}
trace(trace &&){move_n++;}

trace& operator=(trace const&){copy_n++;return *this;}
trace& operator=(trace &&){move_n++;return *this;}
static void test()
{
	using namespace std;
	cout<<"构造对象："<<cons_n<<endl;
	cout<<"拷贝对象："<<copy_n<<endl;
	cout<<"移动对象："<<move_n<<endl;
	cout<<"销毁对象："<<dest_n<<endl;
	if(cons_n + copy_n + move_n != dest_n)
		cout<<"有~T()没有被调用!"<<endl;
}
};
int trace::cons_n = 0;
int trace::dest_n = 0; 
int trace::copy_n = 0;
int trace::move_n = 0;

