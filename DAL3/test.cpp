#include <iostream>
#include <sstream>
#include "other.hpp"
#include "bind_mean.hpp"
using namespace std;
using namespace data_analysis;
int main()
{
	//int (*p)(int) = (int (*)(int))(233);
//	p(0);
	/*
	using namespace data_analysis;
	std::vector<double> n;*/
	OperationSet<double> op = OperationSet<double>::getDefault();
/*	double input;
	string s;
	cout<<"请输入 ";
	getline(cin,s);
	stringstream ss(s);
	for(;ss>>input;n.push_back(input));
	cout<<"源数据";
	for(auto v : n)cout<<" "<<v;cout<<endl;
	cout<<"1.平均数 "<<MeanReflector::call("harmonic", n.begin(), n.end(), op)<<endl;
*/
	
	std::vector<double> n = {4, 5 ,1 ,3, 6, 1, 8,6 ,9, 5, 10 ,11, 5, 1,6};
	/*double input;
	string s;
	cout<<"请输入 ";
	getline(cin,s);
	stringstream ss(s);
	while(ss>>input)n.push_back(input);*/

	make_heap(n.begin(), n.end());
	cout<<"Making Heap:";
	for(auto vv : n)cout<<vv<<" ";
	cout<<endl;
	cout<<"median:"<<median(n.begin(), n.end(), [](double v, double v2, auto){return (v+v2)/2;}, op)<<endl;
	bool f = n.size() %2;
	auto first = n.begin(), last = n.end();
	auto cnt{0};
	while (first != last) {
		cout<<++cnt<<": ";
        std::pop_heap(first, last--);
		for(auto vv : n)cout<<vv<<" ";
		cout<<"median:"<<(f?n[n.size()/2]:(n[n.size()/2]+n[n.size()/2+1])/2)<<endl;
	}
		cout<<"median:"<<(f?n[n.size()/2]:(n[n.size()/2]+n[n.size()/2+1])/2)<<endl;
	cout<<"*last="<<*last<<endl;
	
	return 0;
}

