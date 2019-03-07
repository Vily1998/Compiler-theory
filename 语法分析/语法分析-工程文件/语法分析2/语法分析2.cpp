#include<iostream>
#include<string>
#include<vector>
#include <iomanip>
using namespace std;

//产生式结构
struct createRule
{
	string rule;//产生式本身
	string leftsymbol;//生成式的左侧符号
	int length;//产生式右部的字符串的长度
};
typedef createRule mycreateRule;

//生成式的数组
mycreateRule nowCreateRule[8] =
{
	{ { "E->E+T" },{ "E" },{ 3 } },
	{ { "E->E-T" },{ "E" },{ 3 } },
	{ { "E->T"   },{ "E" },{ 1 } },
	{ { "T->T*F" },{ "T" },{ 3 } },
	{ { "T->T/F" },{ "T" },{ 3 } },
	{ { "T->F"   },{ "T" },{ 1 } },
	{ { "F->(E)" },{ "F" },{ 3 } },
	{ { "F->num" },{ "F" },{ 1 } },
};
string endsymbol[8] = { "(",")","+","-","*","/","num","$" };//终结符符号表

//分析表
string actionAnalyseMap[16][11] =
{
	{ "S4","error","error","error","error","error","S5","error","1","2","3" },
	{ "error","error","S6","S7","error","error","error","ACC","error","error","error" },
	{ "error","R3","R3","R3","S8","S9","error","R3","error","error","error" },
	{ "error","R6","R6","R6","R6","R6","error","R6","error","error","error" },
	{ "S4","error","error","error","error","error","S5","error","10","2","3" },
	{ "error","R8","R8","R8","R8","R8","error","R8","error","error","error" },
	{ "S4","error","error","error","error","error","S5","error","error","11","3" },
	{ "S4","error","error","error","error","error","S5","error","error","12","3" },
	{ "S4","error","error","error","error","error","S5","error","error","error","13" },
	{ "S4","error","error","error","error","error","S5","error","error","error","14" },
	{ "error","S15","S6","S7","error","error","error","error","error","error","error" },
	{ "error","R1","R1","R1","S8","S9","error","R1","error","error","error", },
	{ "error","R2","R2","R2","S8","S9","error","R2","error","error","error" },
	{ "error","R4","R4","R4","R4","R4","error","R4","error","error","error" },
	{ "error","R5","R5","R5","R5","R5","error","R5","error","error","error" },
	{ "error","R7","R7","R7","R7","R7","error","R7","error","error","error" },
};

//错误处理程序
void error()
{
	cout << "分析错误" << endl;
}

//找出终结符号在哪个位置
int findendsymbol(string a)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (a == endsymbol[i])
		{
			return i;
		}
	}
	return -1;
}


void main()
{
	vector<string> stateStack;//状态栈
	vector<string> symbolStack;//符号栈
	stateStack.push_back("0");//状态栈的初始状态
	symbolStack.push_back("_");//符号栈的初始状态
	string input;//分析串
	cout << "请输入你要分析的字符串" << endl;
	cin >> input;
	input += "$";
	int ip = 0;
	string a;
	string s;
	cout << "分析情况如下:" << endl << endl;
	cout.setf(ios::left);//设置左对齐
	cout << setw(20) << "分析栈" << setw(30) << "输入" << setw(20) << "分析动作" << endl;
	while (1)
	{
		cout << "———————————————————————————————" << endl;
		string string_content = "";
		for (int k = 0; k < stateStack.size(); k++)//输出状态栈内容
			string_content += stateStack[k]+" ";
		cout << setw(20) << string_content << endl;
		
		string_content = "";
		for (int k = 0; k < symbolStack.size(); k++)//输出符号栈内容
			string_content += symbolStack[k]+" ";
		cout << setw(20) << string_content;

		cout << setw(30) << input.substr(ip);//取子串输出分析符号


		s = stateStack.back();
		a = input.at(ip);
		if (a == "n")//处理“num”
		{
			a = "num";
			ip = ip + 2;
		}
		int index=findendsymbol(a);
		if (actionAnalyseMap[stoi(stateStack.back())][index].at(0) == 'S')//遇到分析动作是移进
		{
			
			string i =actionAnalyseMap[stoi(stateStack.back())][index].substr(1) ;
			cout << setw(20) << actionAnalyseMap[stoi(stateStack.back())][index] << endl;
			symbolStack.push_back(a);
			stateStack.push_back(i);
			ip = ip + 1;
		}
		else if (actionAnalyseMap[stoi(stateStack.back())][index].at(0) == 'R')//遇到分析动作是规约
		{
			int i = stoi(actionAnalyseMap[stoi(stateStack.back())][index].substr(1));
			cout << setw(20) << actionAnalyseMap[stoi(stateStack.back())][index] << endl;
			for (int j = 0; j < nowCreateRule[i - 1].length; j++)//出栈当前生成式左边符号长度个符号
			{
				symbolStack.pop_back();
				stateStack.pop_back();
			}
			string s1;
			s1 = stateStack.back();

			symbolStack.push_back(nowCreateRule[i - 1].leftsymbol);

			int s2;
			if (nowCreateRule[i - 1].leftsymbol == "E")//遇到“E”时转移的状态
			{
				s2 = 8;
			}
			else if (nowCreateRule[i - 1].leftsymbol == "T")//遇到“T”时转移的状态
			{
				s2 = 9;
			}
			else if (nowCreateRule[i - 1].leftsymbol == "F")//遇到“F”时转移的状态
			{
				s2 = 10;
			}
			

			if (actionAnalyseMap[stoi(s1)][s2] != "error")//若不是错误，则进栈
			{
				stateStack.push_back(actionAnalyseMap[stoi(s1)][s2]);
				
			}
			else
			{
				error();
				break;
			}
		}

		else if (actionAnalyseMap[stoi(stateStack.back())][index] == "ACC")
		{
			cout << "分析成功" << endl;
			break;
		}
		else
		{
			error();
			break;
		}
		
	} 
	system("pause");
}