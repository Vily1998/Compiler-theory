#include <iostream>
#include <vector>
#include <string>
#include <string.h> 
#include <cstdio>
#define NN 20
#define MAXN 1000
#define INF 9999999
#define ERROR -1
using namespace std;
char *token_str[]={"","ID","digit","+","-","*","/","(",")","$"};
const char *ori_grammer[]={"E' -> E","E -> E + T","E -> E - T","E -> T","T -> T * F","T -> T / F","T -> F","F -> id","F -> ( E )","F -> digit"};
enum{E_=MAXN+1,E,T,F};
enum{NUL=0,ID,NUM,ADD,SUB,MUL,DIV,OPL,OPR,END};
const int GRAMMER[NN][NN]={{E_,E},{E,E,ADD,T},{E,E,SUB,T},{E,T},{T,T,MUL,F},{T,T,DIV,F},{T,F},{F,ID},{F,OPL,E,OPR},{F,NUM}};
const int GrammerL[]={1,3,3,1,3,3,1,1,3,1};
const int ACTION[NN][NN]={{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,INF,INF,7,8,INF,INF,INF,INF,0},
{INF,INF,INF,-3,-3,9,10,INF,-3,-3},
{INF,INF,INF,-6,-6,-6,-6,INF,-6,-6},
{INF,INF,INF,-7,-7,-7,-7,INF,-7,-7},
{INF,INF,INF,-9,-9,-9,-9,INF,-9,-9},
{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,4,5,INF,INF,INF,INF,6,INF,INF},
{INF,INF,INF,7,8,INF,INF,INF,16,INF},
{INF,INF,INF,-1,-1,9,10,INF,-1,-1},
{INF,INF,INF,-2,-2,9,10,INF,-2,-2},
{INF,INF,INF,-4,-4,-4,-4,INF,-4,-4},
{INF,INF,INF,-5,-5,-5,-5,INF,-5,-5},
{INF,INF,INF,-8,-8,-8,-8,INF,-8,-8}};

const int GOTO[NN][NN]={{INF,INF,1,2,3},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,11,2,3},
{INF,INF,INF,12,3},
{INF,INF,INF,13,3},
{INF,INF,INF,INF,14},
{INF,INF,INF,INF,15},{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF},
{INF,INF,INF,INF,INF}};

int isIDsym(char ch){
	return (ch=='_'||ch>='a'&&ch<='z'||ch>='A'&&ch<='Z');
}

int isDigit(char ch){
	return (ch>='0'&&ch<='9');
}

int isBlank(char ch){
	return (ch==' '||ch=='\t'||ch=='\n');
}

int SLRcalculate(vector<int> &token,vector<int> &token_val,int &answer){

char tmp_str[MAXN];

vector<int> S,val;

int I,a,pos=0,top;
S.push_back(0);
val.push_back(INF);
token.push_back(END);
while(1){
	I=S.back();
	a=token[pos];
	top=val.size()-1;
	printf("INUPT : ");
	for(int i=pos;i<token.size();i++){
		if(token[i]==NUM)
			printf("%d",token_val[i]);
		else
			printf("%s",token_str[token[i]]);
	}
	printf("\n");
	printf("state :\t");
	for(int i=0;i<S.size();i++){
		printf("%-5d",S[i]);
	}
	printf("\n");printf("val :\t");
	for(int i=0;i<val.size();i++){
		if(val[i]!=INF)
			printf("%-5d",val[i]);
		else
			printf("%-5s","- ");
	}
	printf("\n");
	strcpy(tmp_str,"");
	if(ACTION[I][a]==INF)
		return ERROR;
	if(ACTION[I][a]>0){
		sprintf(tmp_str,"Shift %d",ACTION[I][a]);
		S.push_back(ACTION[I][a]);
		val.push_back(token_val[pos]);
		pos++;
	}
	else if(ACTION[I][a]<0){
		int n=-ACTION[I][a];
		int ntop=top-GrammerL[n]+1;
		sprintf(tmp_str,"reduce by %s",ori_grammer[n]);
		switch(n){
			case 1: val[ntop]=val[top-2]+val[top];break;
			case 2: val[ntop]=val[top-2]-val[top];break;
			case 4: val[ntop]=val[top-2]*val[top];break;
			case 5: val[ntop]=val[top-2]/val[top];break;
			case 8: val[ntop]=val[top-1];break;
		}
		for(int i=0;i<top-ntop;i++)
			val.pop_back();
		for(int i=0;i<GrammerL[n];i++){
			S.pop_back();
		}
	int tmpgo=GOTO[S.back()][GRAMMER[n][0]-MAXN];
	S.push_back(tmpgo);
	}
	else{
		answer=val[top];
		return 0;
	}
	if(pos>=token.size())
		return ERROR;
		printf("%s\n\n",tmp_str);}
	}
	
	
int LexicalAnalyse(char *str,vector<int> &token,vector<int> &token_val){
	int state=0;
	char *ptr=str,ch;
	int value=0;
	while(ptr){
		ch=*ptr;
		switch(state){
		case 0:
		if(isDigit(ch)){
			state=1;
		}
		else if(isIDsym(ch)){
			state=2;
		}
		else{
			state=3;
		}
		break;
		case 1:
		if(isDigit(ch)){
			value=value*10+ch-'0';
			ptr++;
		}
		else{
			token.push_back(NUM);
			token_val.push_back(value);
			value=0;
			state=0;
		}
		break;
		case 2:
		if(isIDsym(ch)||isDigit(ch)){
		ptr++;
			}
		else{
			token.push_back(ID);
			token_val.push_back(INF);
			state=0;
			}
			break;
		case 3:
			switch(ch){
				case '+': token.push_back(ADD);break;
				case '-': token.push_back(SUB);break;
				case '*': token.push_back(MUL);break;case '/': token.push_back(DIV);break;
				case '(': token.push_back(OPL);break;
				case ')': token.push_back(OPR);break;
				default:if(!isBlank(ch))return ERROR;
			}
			ptr++;
			state=0;
			if(!isBlank(ch))
			token_val.push_back(INF);
			break;
		}
	}
}
int main(){
	char str[MAXN];
	int answer,flag;
	vector<int> token,token_val;
	printf("Please input the expression: ");
	gets(str);
	LexicalAnalyse(str,token,token_val);
	printf("Lexical Analyse :\n");
	for(int i=0;i<token.size();i++){
		printf("[%s] ",token_str[token[i]]);
	}
	printf("\n\n");
	printf("Syntax Analyse by SLR :\n");
	flag=SLRcalculate(token,token_val,answer);
	if(flag==0){
		printf("Accept.\n");
		printf("The final result is %d.\n",answer);
	}
	else{
		printf("Error input!\n");
	} 
	return 0;
}
