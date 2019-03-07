#include <iostream>
#include <fstream> 
#include <string>
using namespace std;

ifstream file;//Դ�ļ�
ofstream ofile;//ʶ���ļ�
ofstream errorlist;//��������ļ� 
//��������ǰ��������ǰ�������ַ���
int rownum = 1, nowrow = 1, column = 0, characternum = 0;
//token����ĳ��� allword����ĳ���  allnum����ĳ���
int tokennum, wordnum = 0, digitnum = 0, searchwordnum, searchnnum;
//��ǰָ�� ע����Ŀ
int forword = 0, notenum=0;
char ch = ' ', buffer[4095];
//�ؼ������� 
char keyword[32][20] =
{ "include","main","int","float","double","char","long","bool","short",
"if","else","for","while","do","struct","typedef","const","default",
"return","case","switch","break","continue","enum","goto","sizeof",
"static","void","union","unsigned","signed","extern" };        


//���е��ʵ����� �������ֵ�����
char allword[512][32], allnum[512][32];
//�ַ����� ע������
char token[512], note[1024], getstring[1024];

//�ж����ֵĺ��� 
int digit()
{
	if ((ch >= 48 && ch <= 57))
		return 1;
	else
		return 0;
}

//�ж���ĸ�ĺ��� 
int letter()
{
	if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		return 1;
	else
		return 0;
}

//��ȡ�ַ�����buffer()���飬ÿ�ζ���ȡ������������һ�� 
void inbuffer(int pointer)
{
	int i = 0;
	char ch1;
	while (!file.eof() && i<2048) {
		file.read(&ch1, 1);//����һ���ַ� 
		buffer[pointer+i] = ch1;
		if (ch1 != ' ') {
			if (ch1 == '\n')
				rownum++;      //����+1 
			else
				characternum++;	//�ַ�+1	    
		}
		i++;
	}
	if (file.eof())
		buffer[pointer+i] = '\0';               //Դ�����ȡ���
}


//���˵������Ŀո� 
void deletespace()
{
	if (ch == ' ')
	{
		ch = buffer[forword];
		column++;
		if (forword == 2047)
		{
			inbuffer(2048);
			forword++;
		}
		else if (forword == 4095)
		{
			inbuffer(0);
			forword = 0;
		}
		else
			forword++;
		deletespace();
	}
}

//��ȡ������ÿ�δӻ������ж�����ǰָ���Ӧ���ַ�
void get_char()
{
	ch = buffer[forword];
	column++;//��ǰ��������
	if (forword == 2047)
	{
		inbuffer(2048);
		forword++;
	}
	else if (forword == 4095)
	{
		inbuffer(0);
		forword = 0;
	}
	else
		forword++;
}

//���Ӻ������ѵ�ǰ�ַ���token�е��ַ�����������
void cat()
{
	token[tokennum] = ch;
	tokennum++;
}

//�жϵ�ǰ�����Ƿ��Ѿ������뵥������
int findidword()
{
	for (int i = 0; i<wordnum; i++)
	{
		if (strcmp(token, allword[i]) == 0)
		{
			searchwordnum = i;
			return 1;
		}
	}
	return 0;
}

//���µ��ʲ��뵥������
void insertallword()
{
	strcpy_s(allword[wordnum], token);
	wordnum++;
}

//�жϵ�ǰ�����Ƿ��Ѿ�������������
int findnumword()
{
	for (int i = 0; i<digitnum; i++)
	{
		if (strcmp(token, allnum[i]) == 0)
		{
			searchnnum = i;
			return 1;
		}
	}
	return 0;
}

//�������ֲ�����������
void insertallnum()
{
	strcpy_s(allnum[digitnum], token);
	digitnum++;
}


//�жϵ�ǰ�����Ƿ�Ϊ�����Ĺؼ��֣����Ƿ���1�������򷵻�0 
int reserve()
{
	for (int i = 0; i<32; i++)
	{
		if (strcmp(token, keyword[i]) == 0)
			return 1;
	}
	return 0;
}


//ʶ��/*��ͷ��ע�ͣ���Ҫѭ������
void takingnote()
{
	get_char();
	note[notenum++] = ch;
	while (ch != '*')
	{
		get_char();
		note[notenum++] = ch;
	}
	get_char();
	note[notenum++] = ch;
}




//��������� 
void error()
{
	ofile << "�ʷ����������ִ���λ�ڵ�" << nowrow << "�У���" << column << "��" << endl;
	cout << endl;
	cout << "�ʷ����������ִ���λ�ڵ�" << nowrow << "�У���" << column << "��" << endl;
	errorlist << "�ʷ����������ִ���λ�ڵ�" << nowrow << "�У���" << column << "��" << endl;
}


//��������
int main()
{
	errorlist.open("�����������.txt");
	ofile.open("����������.txt");

	string mytestname;
    cout << "��������Ҫʶ��Ĳ����ļ�����";
	cin >> mytestname;
	file.open(mytestname, ios::in);
	if (!file)
	{
		cout << "δ����ȷ�򿪲����ļ�" << endl;
		return 1;
	}
	inbuffer(0);

	int iskey, result;
	while (ch != '\0')
	{
		get_char();
		deletespace();
		switch (ch) 
		{
			//���ʴ��� 
		case'a':case'b':case'c':case'd':case'e':case'f':case'g':
		case'h':case'i':case'j':case'k':case'l':case'm':case'n':
		case'o':case'p':case'q':case'r':case's':case't':case'u':
		case'v':case'w':case'x':case'y':case'z':case'A':case'B':
		case'C':case'D':case'E':case'F':case'G':case'H':case'I':
		case'J':case'K':case'L':case'M':case'N':case'O':case'P':
		case'Q':case'R':case'S':case'T':case'U':case'V':case'W':
		case'X':case'Y':case'Z':case'_':
			tokennum = 0;
			while (letter() || digit() || ch == '_')
			{
				cat();
				get_char();
				if (ch == '\n') nowrow--;
			}
			token[tokennum] = '\0';//���ĩβ���ŷ����������
			forword--;
			result = reserve();
			if (result == 0)
			{
				iskey = findidword();//������ǰ�����Ƿ��Ѿ������뵥������
				if (iskey == 0)// ���û����
				{
					insertallword();
					ofile << "< id , " << wordnum - 1 << " >:" << token << endl;
					//cout << "<id," << wordnum - 1 << ">:" << token << endl;
				}
				else
				{
					ofile << "< id , " << searchwordnum << " >:" << token << endl;
					//cout << "<id," << searchwordnum << ">:" << token << endl;
				}
			}
			else
			{
				ofile << "�ؼ��֣�" << token << endl;
				//cout << "�ؼ���:" << token << endl;
			}
				
			break;
			//���ִ��� ����ѭ���ϵĴ�����
		case '0':case'1':case'2':case'3':case'4':case'5':
		case '6':case'7':case'8':case'9':
			tokennum = 0;
			while (digit())
			{
				cat();
				get_char();
			}
			if (ch == '.')
			{
				cat();
				get_char();
				if (!digit())
				{
					error();//�ж�2.E�������͵Ĵ���
					break;
				}
				else
				{
					while (digit())
					{
						cat();
						get_char();
					}
				}
			}
			if (ch == 'E')//2.3E���ֿ��Ա�ʶ��
			{
				cat();
				get_char();
				if (ch == '+' || ch == '-')//E�ĺ���ɼ�+��Ҳ���Լ�-��
				{
					cat();
					get_char();
					if (!digit())
					{
						error();
						break;
					}
					else {
						while (digit())
						{
							cat();
							get_char();
						}
					}
				}
				else if (digit())
				{
					while (digit())
					{
						cat();
						get_char();
					}
				}
			}
			token[tokennum] = '\0';
			forword--;
			iskey = findnumword();
			if (iskey == 0)
			{
				insertallnum();

				ofile << "< num ," << digitnum - 1 << " >:" << token << endl;
				//cout << "<num," << digitnum - 1 << ">:" << token << endl;
			}
			else
			{
				ofile << "< num ," << searchnnum << " >:" << token << endl;
				//cout << "<num," << searchnnum << ">:" << token << endl;
			}
			break;
			//ʶ����<��ͷ�����з��� 
		case '<':
			get_char();
			if (ch == '=')
			{
				ofile << "< relop , LE(<=) >" << endl;
				//cout << "<relop,LE(<=)>" << endl;
			}
			else if (ch == '>')
			{
				ofile << "< relop , NE(<>) >" << endl;
				//cout << "<relop,NE(<>)>" << endl;
			}
			else
			{
				forword--;
				ofile << "< relop , LT(<) >" << endl;
				//cout << "<relop,LT(<)>" << endl;
			}
			break;
			//ʶ����=��ͷ�����з��� 
		case '=':
			get_char();
			if (ch == '=')
			{
				ofile << "< relop, EQ(==) >" << endl;
				//cout << "<relop,EQ(==)>" << endl;
			}
			else 
			{
				forword--;
				ofile << "< relop , ass(=) >" << endl;
				//cout << "<relop,ass(=)>" << endl;
			}
			break;
			//ʶ����>��ͷ�����з��� 
		case '>':
			get_char();
			if (ch == '=')
			{
				ofile << "< relop , GE(>=) >" << endl;
				//cout << "<relop,GE(>=)>" << endl;

			}
			else
			{
				forword--;
				ofile << "< relop , GT(>) >" << endl;
				//cout << "<relop,GT(>)>" << endl;
			}
			break;
		case ':':
			get_char();
			if (ch == '=')
			{
				ofile << "< assign-op , -(:=) >" << endl;
				//cout << "<assign-op,-(:=)>" << endl;
			}
			else
			{
				forword--;
				ofile << "< ':' , - >" << endl;
				//cout << "<':',->" << endl;
			}
			break;
		case '+':
			get_char();
			if (ch == '=')
			{
				ofile << "< assignop , assad(+=) >" << endl;
				//cout << "<assignop,assad(+=)>" << endl;
			}
			else if (ch == '+')
			{
				ofile << "< '++' , - >" << endl;
				//cout << "<'++',->" << endl;

			}
			else
			{
				forword--;
				ofile << "< '+' , - >" << endl;
				//cout << "<'+',->" << endl;
			}
			break;
		case '-':
			get_char();
			if (ch == '=')
			{
				ofile << "< assignop , assub(-=) >" << endl;
				//cout << "<assignop,assub(-=)>" << endl;
			}
			else if (ch == '-')
			{
				ofile << "< '--' , - >" << endl;
				//cout << "<'--',->" << endl;
			}
			else
			{
				forword--;
				ofile << "< '-' , - >" << endl;
				//cout << "<'-',->" << endl;
			}
			break;
		case '*':
			get_char();
			if (ch == '=')
				ofile << "< assignop , assmul(*=) >" << endl;
			else
			{
				forword--;
				ofile << "< '*' , - >" << endl;
			}
			break;
		case '(':
			ofile << "< '(' , - >" << endl;
			break;
		case ')':
			ofile << "< ')' , - >" << endl;
			break;
		case ';':
			ofile << "< ';' , - >" << endl;
			break;
		case '%':
			ofile << "< '%' , - >" << endl;
			break;
		case '[':
			ofile << "< '[' , - >" << endl;
			break;
		case ']':
			ofile << "< ']' , - >" << endl;
			break;
		case '{':
			ofile << "< '{' , - >" << endl;
			break;
		case '}':
			ofile << "< '}' , - >" << endl;
			break;
		case ',':
			ofile << "< ',' , - >" << endl;
			break;
		case '#':
			ofile << "< '#' , - >" << endl;
			break;
		case '.':
			ofile << "< '.' , - >" << endl;
			break;
		case '?':
			ofile << "< '?' , - >" << endl;
			break;
		case '!':
			get_char();
			if (ch == '=')
				ofile << "< relop , (!=) >" << endl;
			else
			{
				forword--;
				ofile << "< logicop , not(!) >" << endl;
			}
			break;
		case '|':
			get_char();
			if (ch == '|')
				ofile << "< logicop , or(||) >" << endl;
			break;
		case '&':
			get_char();
			if (ch == '&')
				ofile << "< logicop , and(&&) >" << endl;
			else
			{
				forword--;
				ofile << "< '&' , - >" << endl;
			}
			break;
		case '/':
			get_char();
			if (ch == '*')
			{
				ofile << "< '/*' , note >" << endl;
				notenum = 0;
				takingnote();
				while (ch != '/') takingnote();
				note[notenum - 1] = ' ';
				note[notenum - 2] = ' ';
				ofile << "ע�����£�" << note << endl;
				ofile << "< '*/' , note >" << endl;
				break;
			}
			else if (ch == '/')
			{
				ofile << "< '//' , note >" << endl;
				notenum = 0;
				char note1[1024];
				while (ch != '\n')
				{
					get_char();
					note1[notenum++] = ch;
				}
				note1[notenum++] = '\0';
				ofile << "ע�����£�" << note1;
				break;
			}
			else if (ch == '=')
				ofile << "< assignop , assdiv'/=' >" << endl;
			else {
				forword--;
				ofile << "< '/' , - >" << endl;
			}
			break;
		case '\\':
			ofile << "< esc , \ >" << endl;
			break;
		case '\a':
			ofile << "< esc , BEl()���� >" << endl;
			break;
		case '\b':
			ofile << "< esc , BS(�˸�) >" << endl;
			break;
		case '\f':
			ofile << "< esc , FF(��ҳ) >" << endl;
			break;
		case '\n':
			ofile << "< esc , LF(����) >" << endl;
			nowrow++;
			column = 0;
			break;
		case '\r':
			ofile << "< esc , CR(�س�) >" << endl;
			break;
		case '\t':
			ofile << "< esc , HT(ˮƽ�Ʊ�) >" << endl;
			break;
		case '\v':
			ofile << "< esc , VT(��ֱ�Ʊ�) >" << endl;
			break;
		case '\"':
			ofile << "< esc , ˫�����ַ� >" << endl;
			get_char();
			iskey = 0, result = 1;
			getstring[128];
			while (ch != '\"')
			{
				getstring[iskey] = ch;
				if (ch == '\n') 
				{
					error();
					forword--;
					result = 0;
					break;
				}
				get_char();
				iskey++;
			}
			for (int i = iskey; i <= 100; i++) getstring[i] = ' ';
			if (result == 1)//�ж��Ƿ�Ϊ�ַ���
				ofile << "�ַ�����" << getstring << endl;
			else
				ofile << "�����ַ�����" << getstring << endl;
			ofile << "< esc, ˫�����ַ� >" << endl;
			break;
		case '\0':
			ofile << "< esc , NULL(���ַ�) >" << endl;
			break;
		case '\'':
			ofile << "< esc , '''(������) >" << endl;
			break;
		default:
			error();
			break;
		}
	}
	cout << endl;
	ofile << "���������е��������Ϊ��" << rownum << endl;
	cout << "���������е��������Ϊ��" << rownum << endl;
	ofile << "���������еĵ��ʸ���Ϊ��" << wordnum << endl;
	cout << "���������еĵ��ʸ���Ϊ��" << wordnum << endl;
	ofile << "Դ�������ַ�����Ϊ��" << characternum << endl;
	cout << "Դ�������ַ�����Ϊ��" << characternum << endl;
	cout << endl;
	cout << "����������̣���������ļ�������������.txt���У���鿴��" << endl;
	cout << "�����������������ļ��������������.txt���У���鿴��" << endl;
	cout << endl;
	file.close();
	ofile.close();
	system("pause");
	return 0;
}