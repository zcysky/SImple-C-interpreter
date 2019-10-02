#include<bits/stdc++.h>

using namespace std;

const int ARRAY_LIMIT=65536;

//关键字定义：
const int INT=_ARRAY_LIMIT-1;

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
const int L_NEQ=L_LEQ-1;
const int L_AND=L_NEQ-1;
const int L_OR=L_AND-1;
