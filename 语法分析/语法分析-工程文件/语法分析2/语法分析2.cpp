#include<iostream>
#include<string>
#include<vector>
#include <iomanip>
using namespace std;

//����ʽ�ṹ
struct createRule
{
	string rule;//����ʽ����
	string leftsymbol;//����ʽ��������
	int length;//����ʽ�Ҳ����ַ����ĳ���
};
typedef createRule mycreateRule;

//����ʽ������
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
string endsymbol[8] = { "(",")","+","-","*","/","num","$" };//�ս�����ű�

//������
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

//���������
void error()
{
	cout << "��������" << endl;
}

//�ҳ��ս�������ĸ�λ��
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
	vector<string> stateStack;//״̬ջ
	vector<string> symbolStack;//����ջ
	stateStack.push_back("0");//״̬ջ�ĳ�ʼ״̬
	symbolStack.push_back("_");//����ջ�ĳ�ʼ״̬
	string input;//������
	cout << "��������Ҫ�������ַ���" << endl;
	cin >> input;
	input += "$";
	int ip = 0;
	string a;
	string s;
	cout << "�����������:" << endl << endl;
	cout.setf(ios::left);//���������
	cout << setw(20) << "����ջ" << setw(30) << "����" << setw(20) << "��������" << endl;
	while (1)
	{
		cout << "��������������������������������������������������������������" << endl;
		string string_content = "";
		for (int k = 0; k < stateStack.size(); k++)//���״̬ջ����
			string_content += stateStack[k]+" ";
		cout << setw(20) << string_content << endl;
		
		string_content = "";
		for (int k = 0; k < symbolStack.size(); k++)//�������ջ����
			string_content += symbolStack[k]+" ";
		cout << setw(20) << string_content;

		cout << setw(30) << input.substr(ip);//ȡ�Ӵ������������


		s = stateStack.back();
		a = input.at(ip);
		if (a == "n")//����num��
		{
			a = "num";
			ip = ip + 2;
		}
		int index=findendsymbol(a);
		if (actionAnalyseMap[stoi(stateStack.back())][index].at(0) == 'S')//���������������ƽ�
		{
			
			string i =actionAnalyseMap[stoi(stateStack.back())][index].substr(1) ;
			cout << setw(20) << actionAnalyseMap[stoi(stateStack.back())][index] << endl;
			symbolStack.push_back(a);
			stateStack.push_back(i);
			ip = ip + 1;
		}
		else if (actionAnalyseMap[stoi(stateStack.back())][index].at(0) == 'R')//�������������ǹ�Լ
		{
			int i = stoi(actionAnalyseMap[stoi(stateStack.back())][index].substr(1));
			cout << setw(20) << actionAnalyseMap[stoi(stateStack.back())][index] << endl;
			for (int j = 0; j < nowCreateRule[i - 1].length; j++)//��ջ��ǰ����ʽ��߷��ų��ȸ�����
			{
				symbolStack.pop_back();
				stateStack.pop_back();
			}
			string s1;
			s1 = stateStack.back();

			symbolStack.push_back(nowCreateRule[i - 1].leftsymbol);

			int s2;
			if (nowCreateRule[i - 1].leftsymbol == "E")//������E��ʱת�Ƶ�״̬
			{
				s2 = 8;
			}
			else if (nowCreateRule[i - 1].leftsymbol == "T")//������T��ʱת�Ƶ�״̬
			{
				s2 = 9;
			}
			else if (nowCreateRule[i - 1].leftsymbol == "F")//������F��ʱת�Ƶ�״̬
			{
				s2 = 10;
			}
			

			if (actionAnalyseMap[stoi(s1)][s2] != "error")//�����Ǵ������ջ
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
			cout << "�����ɹ�" << endl;
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