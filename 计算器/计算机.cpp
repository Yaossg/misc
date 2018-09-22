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
					throw string("�������Ż����ִ�ͷ");
				break;	
		}
		
		temp = ss.get();
		if(!isdigit(temp))throw string("������֮�������");
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
							else throw string("�����ص������");
							
								
					}	
			}
			temp = ss.get();
			if(!isdigit(temp))throw string("�����ص������");
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
							throw string("��������Ϊ0!");
							
					}break;
					
				case '%':
					{
						if(Lnum[i]!=0)
							Rnum = fmod(Rnum,Lnum[i]);
						else
							throw string("��������Ϊ0!");
					}break;
				case '^':
					{
						if(Rnum<0)
							Rnum = pow(Rnum,round(Lnum[i]));
						else
							Rnum = pow(Rnum,Lnum[i]);
					}break;
					
				default:
					throw string("δ֪�����");
			}
		}
		cout<<"="<<Rnum<<endl;
	}
};


void run();
//����������ִ��һ�μ��㣬�����while(true)ʹ��

void cmd(string& s);
//����ָ�� 

void help();
//�����б�
void info();
//������� 


int main()
{ 
	while(true)
		run();	
	return 0;
}

void run()
{
	string s;
	cout<<"������ʽ������:"<<endl;
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
		{cout<<"δ֪��ָ�������/help��ȡָ���б�";}
	
}

inline void help()
{
	cout<<"/help �� /h �� /? - ָ���б�"<<endl;
	cout<<"/info �� /i - ��������Ϣ"<<endl;
	cout<<"/clear �� /clr �� /c - ����"<<endl;
	cout<<"/quit �� /q �� /exit �� /e �� /stop �� /s - �˳�"<<endl;
} 

inline void info()
{
	cout<<"Yao�ļ�����V2.1"<<endl;
}



