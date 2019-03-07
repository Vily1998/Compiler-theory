#include <iostream>
#include <fstream> 
#include <string>
using namespace std;

ifstream file;//源文件
ofstream ofile;//识别文件
ofstream errorlist;//错误分析文件 
//行数、当前行数、当前列数、字符数
int rownum = 1, nowrow = 1, column = 0, characternum = 0;
//token数组的长度 allword数组的长度  allnum数组的长度
int tokennum, wordnum = 0, digitnum = 0, searchwordnum, searchnnum;
//向前指针 注释数目
int forword = 0, notenum=0;
char ch = ' ', buffer[4095];
//关键字数组 
char keyword[32][20] =
{ "include","main","int","float","double","char","long","bool","short",
"if","else","for","while","do","struct","typedef","const","default",
"return","case","switch","break","continue","enum","goto","sizeof",
"static","void","union","unsigned","signed","extern" };        


//所有单词的数组 所有数字的数组
char allword[512][32], allnum[512][32];
//字符数组 注释数组
char token[512], note[1024], getstring[1024];

//判断数字的函数 
int digit()
{
	if ((ch >= 48 && ch <= 57))
		return 1;
	else
		return 0;
}

//判断字母的函数 
int letter()
{
	if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		return 1;
	else
		return 0;
}

//读取字符放入buffer()数组，每次都读取缓冲区容量的一半 
void inbuffer(int pointer)
{
	int i = 0;
	char ch1;
	while (!file.eof() && i<2048) {
		file.read(&ch1, 1);//读入一个字符 
		buffer[pointer+i] = ch1;
		if (ch1 != ' ') {
			if (ch1 == '\n')
				rownum++;      //行数+1 
			else
				characternum++;	//字符+1	    
		}
		i++;
	}
	if (file.eof())
		buffer[pointer+i] = '\0';               //源程序读取完毕
}


//过滤掉读到的空格 
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

//读取函数，每次从缓冲区中读出当前指针对应的字符
void get_char()
{
	ch = buffer[forword];
	column++;//当前列数增加
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

//连接函数，把当前字符与token中的字符串连接起来
void cat()
{
	token[tokennum] = ch;
	tokennum++;
}

//判断当前单词是否已经被放入单词数组
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

//将新单词插入单词数组
void insertallword()
{
	strcpy_s(allword[wordnum], token);
	wordnum++;
}

//判断当前数字是否已经放入数字数组
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

//将新数字插入数字数组
void insertallnum()
{
	strcpy_s(allnum[digitnum], token);
	digitnum++;
}


//判断当前单词是否为保留的关键字，若是返回1，不是则返回0 
int reserve()
{
	for (int i = 0; i<32; i++)
	{
		if (strcmp(token, keyword[i]) == 0)
			return 1;
	}
	return 0;
}


//识别/*开头的注释，需要循环处理
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




//错误处理程序 
void error()
{
	ofile << "词法分析程序发现错误：位于第" << nowrow << "行，第" << column << "列" << endl;
	cout << endl;
	cout << "词法分析程序发现错误：位于第" << nowrow << "行，第" << column << "列" << endl;
	errorlist << "词法分析程序发现错误：位于第" << nowrow << "行，第" << column << "列" << endl;
}


//主处理函数
int main()
{
	errorlist.open("错误分析报告.txt");
	ofile.open("程序分析结果.txt");

	string mytestname;
    cout << "请输入你要识别的测试文件名：";
	cin >> mytestname;
	file.open(mytestname, ios::in);
	if (!file)
	{
		cout << "未能正确打开测试文件" << endl;
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
			//单词处理 
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
			token[tokennum] = '\0';//添加末尾符号否则出现乱码
			forword--;
			result = reserve();
			if (result == 0)
			{
				iskey = findidword();//看看当前单词是否已经被放入单词数组
				if (iskey == 0)// 如果没放入
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
				ofile << "关键字：" << token << endl;
				//cout << "关键字:" << token << endl;
			}
				
			break;
			//数字处理 ，依循书上的处理方法
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
					error();//判断2.E这种类型的错误
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
			if (ch == 'E')//2.3E这种可以被识别
			{
				cat();
				get_char();
				if (ch == '+' || ch == '-')//E的后面可加+号也可以加-号
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
			//识别以<开头的所有符号 
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
			//识别以=开头的所有符号 
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
			//识别以>开头的所有符号 
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
				ofile << "注释如下：" << note << endl;
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
				ofile << "注释如下：" << note1;
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
			ofile << "< esc , BEl()响铃 >" << endl;
			break;
		case '\b':
			ofile << "< esc , BS(退格) >" << endl;
			break;
		case '\f':
			ofile << "< esc , FF(换页) >" << endl;
			break;
		case '\n':
			ofile << "< esc , LF(换行) >" << endl;
			nowrow++;
			column = 0;
			break;
		case '\r':
			ofile << "< esc , CR(回车) >" << endl;
			break;
		case '\t':
			ofile << "< esc , HT(水平制表) >" << endl;
			break;
		case '\v':
			ofile << "< esc , VT(垂直制表) >" << endl;
			break;
		case '\"':
			ofile << "< esc , 双引号字符 >" << endl;
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
			if (result == 1)//判断是否为字符串
				ofile << "字符串：" << getstring << endl;
			else
				ofile << "出错字符串：" << getstring << endl;
			ofile << "< esc, 双引号字符 >" << endl;
			break;
		case '\0':
			ofile << "< esc , NULL(空字符) >" << endl;
			break;
		case '\'':
			ofile << "< esc , '''(单引号) >" << endl;
			break;
		default:
			error();
			break;
		}
	}
	cout << endl;
	ofile << "分析程序中的语句行数为：" << rownum << endl;
	cout << "分析程序中的语句行数为：" << rownum << endl;
	ofile << "分析程序中的单词个数为：" << wordnum << endl;
	cout << "分析程序中的单词个数为：" << wordnum << endl;
	ofile << "源程序中字符个数为：" << characternum << endl;
	cout << "源程序中字符个数为：" << characternum << endl;
	cout << endl;
	cout << "具体分析过程，已输出到文件‘程序分析结果.txt’中，请查看。" << endl;
	cout << "错误分析，已输出到文件‘错误分析报告.txt’中，请查看。" << endl;
	cout << endl;
	file.close();
	ofile.close();
	system("pause");
	return 0;
}