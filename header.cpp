#include<bits/stdc++.h>

using namespace std;

const int ARRAY_LIMIT=65536;

//关键字定义：
const int INT=ARRAY_LIMIT-1;

//token的三种类型
const int IDENT=INT-1;
const int KEYWORD=IDENT-1;
const int NUMBER=KEYWORD-1;

//系统保留字
const int FOR=NUMBER-1;
const int WHILE=FOR-1;
const int IF=WHILE-1;
const int ELSE=IF-1;
const int RETURN=ELSE-1;

//逻辑运算符
const int L_EQ=RETURN-1;
const int L_LEQ=L_EQ-1;
const int L_REQ=L_LEQ-1;
const int L_NEQ=L_REQ-1;
const int L_AND=L_NEQ-1;
const int L_OR=L_AND-1;

//其他特殊标识
const int ERROR=L_OR-1;
const int EXIT=ERROR-1;
const int COMMENT=EXIT-1;


//定义运算符号
//括号
const int LC1='(';
const int RC1=')';
const int LC2='[';
const int RC2=']';
const int BEGIN='{';
const int END='}';

//双目基本运算
const int PLUS='+';
const int MINUS='-';
const int MUL='*';
const int DEV='/';
const int MOD='%';

//逻辑运算符
const int NOT='!';
const int ASSIGN='=';
const int SEP=';';
const int LESS='<';
const int GREAT='>';

#define dbg(x) cout<<#x<<' = '<<(x)<<endl;