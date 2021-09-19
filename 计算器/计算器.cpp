#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <cctype>
#include <vector>

using namespace std;

class equation
{
public:
	vector<double> num;
	vector<char> sign;
	vector<char> work;
	void load(const string& loadStr)
	{
		
		stringstream ss;
		ss.str(loadStr);
		char temp;
		int num_t;
		int i = 0;
		work.push_back('n');
		switch(temp = ss.get())
		{
			case '+':
				sign.push_back('+');
				break;
			case '-':
				sign.push_back('-');
				break;
			default:
				if(isdigit(temp))
				{
					sign.push_back('+');
					ss.putback(temp);
				}else
					throw string("以正负号或数字打头");
				break;	
		}
		
		temp = ss.get();
		if(!isdigit(temp))throw string("正负号之后跟数字");
		ss.putback(temp);
		
		ss>>num_t;
		num.push_back(num_t);
		
		for(i = 1;!ss.eof();i++)
		{
			
			temp = ss.get();
			
			switch(temp)
			{
				case '+':
					sign.push_back('+');
					work.push_back('n');
					break;
				case '-':
					sign.push_back('-');
					work.push_back('n');
					break;
				default:
					work.push_back(temp);
					temp = ss.get();
					switch(temp)
					{
						case '+':
							sign.push_back('+');
							break;
						case '-':
							sign.push_back('-');
							break;
						default:
							if(isdigit(temp))
							{
								sign.push_back('+');
								ss.putback(temp);
								
							}
							else throw string("请勿重叠运算符");
							
								
					}	
			}
			temp = ss.get();
			if(!isdigit(temp))throw string("请勿重叠运算符");
			ss.putback(temp);
			ss>>num_t;
			num.push_back(num_t);
		}
	}
	void compute()
	{
		vector<double> Lnum;
		double Rnum = 0;
		for(int i = 0;i < num.size();i++)
		{
			if(sign[i]=='+')
				Lnum.push_back(num[i]);
			else
				Lnum.push_back(-num[i]);
		}
		for(int i = 0; i < work.size();i++)
		{
			switch(work[i])
			{
				case 'n':Rnum += Lnum[i];break;
				case '*':Rnum *= Lnum[i];break;
				case '/':
					{
						if(Lnum[i]!=0)
							Rnum /= Lnum[i];
						else
							throw string("除数不能为0!");
							
					}break;
					
				case '%':
					{
						if(Lnum[i]!=0)
							Rnum = fmod(Rnum,Lnum[i]);
						else
							throw string("除数不能为0!");
					}break;
				case '^':
					{
						if(Rnum<0)
							Rnum = pow(Rnum,round(Lnum[i]));
						else
							Rnum = pow(Rnum,Lnum[i]);
					}break;
					
				default:
					throw string("未知运算符");
			}
		}
		cout<<"="<<Rnum<<endl;
	}
};


void run();
//主函数，能执行一次计算，可配合while(true)使用

void cmd(string& s);
//处理指令 

void help();
//命令列表
void info();
//给与帮助 


int main()
{ 
	while(true)
		run();	
	return 0;
}

void run()
{
	string s;
	cout<<"输入算式或命令:"<<endl;
	while(s == "")		
		getline(cin,s);
	if(s[0] == '/')
		cmd(s);
	else
	{
		class equation e;
		try
		{
			e.load(s);
			e.compute();
		}
		catch(string s)
		{
			cerr<<s<<endl;
		}
		
	}
}

void cmd(string& s)
{
		for(int i = 0; i < s.length(); i++)
			s[i] = tolower(s[i]);
        if(s=="/help"||s=="/h"||s=="/?")
        {help();}
		else
		if(s=="/info"||s=="/i")
		{info();}
		else
		if(s=="/clear"||s=="/clr"||s=="/c")
		{system("cls");}
		else
		if(s=="/quit"||s=="/q"||s=="/exit"||s=="/e"||s=="/stop"||s=="/s")
		{exit(0);}
		else
		{cout<<"未知的指令，请输入/help获取指令列表";}
	
}

inline void help()
{
	cout<<"/help 或 /h 或 /? - 指令列表"<<endl;
	cout<<"/info 或 /i - 开发者信息"<<endl;
	cout<<"/clear 或 /clr 或 /c - 清屏"<<endl;
	cout<<"/quit 或 /q 或 /exit 或 /e 或 /stop 或 /s - 退出"<<endl;
} 

inline void info()
{
	cout<<"Yao的计算器V2.1"<<endl;
}



