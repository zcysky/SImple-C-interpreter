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

//先写一个定位位置的，用于日后抛出报错
//报错以后再写

class Position{
    public:
    int line,col;
    Position () {}
    Position (int line,int col){
        this->line=line;this->col=col;
    }
    void report(){
        cerr<<"Line: "<<line<<",col: "<<col;
    }
};

class token{
    public:
    Position pos;//追踪位置，用于报错
    int type,number;
    string ident;

    token(){}
    token(Position pos,int type,const string &ident){
        this->pos=pos;this->type=type;this->ident=ident;
    }
    token(Position pos,int type){
        this->pos=pos;this->type=type;
    }
    token(Position pos,int type,int number){
        this->pos=pos;this->type=type;this->number=number;
    }
    inline void output(){
        if(type=ERROR)cout<<"ERROR"<<endl;
        if(type==IDENT)cout<<"Ident:"<<ident<<endl;
        if(type==NUMBER)cout<<"Number:"<<number<<endl;
        if(type==KEYWORD)cout<<"Keyword:"<<ident<<endl;
    }
};
/*
struct TrieNode{
    int val;
    TrieNode *c[30];
}TrieRoot,TrieNull;
struct Trie{
    inline TrieNode *newnode(){
        TrieNode *cur=NULL;
        cur=(TrieNode*)malloc(sizeof(TrieNode));
        if(cur==NULL){/*抛出错误：申请内存失败*///}
        /*
        memset(cur->c,0x00,sizeof(cur->c));
        cur->val=0;
        return cur;
    }
    int insert(TrieNode *cur,string s){
        int index=0;
        TrieNode *tmp=cur;
        if(cur==NULL||s==""||s[0]=='\0')return 0;
        for(int i=0;i<s.length();i++){
            if(s[i]<'a'||s[i]>'z')
        }
    }
};
//用trie树加速关键词的判定（未完成）
*/


class lexer{
    map<string,int> keys;
    map<int,string> table;
    //上面的存储一个双射，分别表示标识符/关键字被映射的编号
    Position pos;
    char ch;
    int ident,number,error;
    map<string,int> prefix;
    //暂时存储前缀，之后用Trie代替

    //本部分函数调用频率大，使用inline加速调用效率
    inline void readchar(){
        ch=getchar();
        if(ch=='\n'){pos.line++;pos.col=0;}
        else if(ch=='\r'){pos.col=0;}
        else pos.col++;
    }
    inline bool isblank(char ch){
        if(ch==' '||ch=='\n'||ch=='\t'||ch=='\r')return 1;
        else return 0;
    }
    inline void skipblank(){
        do{readchar();}while(isblank(ch));
    }
    inline token readnumber(){
        assert(isdigit(ch));
        int val=0;
        Position now=pos;//记录当前读入到的位置，下略
        if(ch!='0'){
            while(isdigit(ch)){val=val*10+ch-'0';readchar();}
        }
        else{
            readchar();if(isdigit(ch)){/*返回错误：数字的开头不能为0*/}
        }
        return token(now,NUMBER,val);
    }
    inline bool checkid(char ch){
        if('a'<=ch&&ch<='z')return 1;
        if('A'<=ch&&ch<='Z')return 1;
        if('0'<=ch&&ch<='9')return 1;
        if(ch=='_')return 1;
        return 0;
    }
    inline token readident(){
        puts("!!!");
        assert(isalpha(ch)||ch=='_');
        Position now=pos;string cur;
        while(checkid(ch)){cur+=ch;readchar();}
        if(keys.count(cur))return token(now,keys[cur],cur);
        else return token(now,IDENT,cur);
    }
    inline token readkeyword(){
        Position now=pos;string cur;
        while(prefix.count(cur+ch)){cur+=ch;readchar();}
        if(cur==""){
            //抛出错误：未定义标识符。
            while(!isblank(ch))readchar();
            return token(now,ERROR,cur);
        }
        else return token(now,keys[cur],cur);
    }

    public:
    lexer(){ch=' ';pos.line=1;pos.col=0;}
    inline void append(int id,const string &s){//向Lexer中插入语法单元
        assert(!keys.count(s));
        assert(!table.count(id));
        table[id]=s;keys[s]=id;
        string cur;
        for(size_t i=0;i<s.size();i++){
            cur=cur+s[i];
            prefix[cur]=1;
        }
    }
    inline void MakeIdent(int id){ident=id;}
    inline void MakeNumber(int id){number=id;}
    inline void MakeError(int id){error=id;}
    inline void MakeOpt(const string &s){
        for(size_t i=0;i<s.size();i++)append(s[i],s.substr(i,1));
    }
    inline token read(){
        if(isblank(ch))skipblank();
        if(ch==EOF)return token(pos,EXIT,"eof");
        if(ch=='_'||isalpha(ch))return readident();
        else if(isdigit(ch))return readnumber();
        else return readkeyword();
    }
    inline token readtoken(){
        token tok=read();
        while(tok.type==COMMENT){
            while(ch!='\n')readchar();
            readchar();tok=read();
        }
        return tok;
    }
    inline void begin(){skipblank();}
    inline bool iserror(){/*判定是否错误*/}
    inline void init(){
        append(INT,"int");
        append(FOR,"for");
        append(WHILE,"while");
        append(ELSE,"else");
        append(IF,"if");
        append(RETURN,"return");
        
        append(L_EQ,"==");
        append(L_LEQ,"<=");
        append(L_REQ,">=");
        append(L_NEQ,"!=");
        append(L_AND,"&&");
        append(L_OR,"||");

        append(COMMENT,"//");
        MakeOpt("#+-*/%()[]{}=!^;,<>");
        
        MakeIdent(IDENT);
        MakeNumber(NUMBER);
        MakeError(ERROR);
    }

}lexer;

//veoctor<token> tokens;
int main(){
    lexer.init();
    freopen("source.cpp","r",stdin);
    lexer.begin();
    token cur;
    int cnt=0;
    do{
        cur=lexer.readtoken();
        cnt++;
        //tokens.push_back(cur);
        cur.output();
        cout<<cur.ident<<endl;
      //  system("PAUSE");
       // if(cnt==10)return 0;
    }while(cur.type!=EXIT);
}