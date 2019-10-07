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
const int SHIFTL=COMMENT-1;
const int SHIFTR=SHIFTL-1;

//输入输出部分
const int CIN=SHIFTR-1;
const int COUT=CIN-1;
const int PUTCHAR=COUT-1;
const int ENDL=PUTCHAR-1;


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

inline int read(){
    int f=1,x=0;char ch;
    do{ch=getchar();if(ch=='-')f=-1;}while(ch<'0'||ch>'9');
    do{x=x*10+ch-'0';ch=getchar();}while(ch>='0'&&ch<='9');
    return f*x;
}


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
        cerr<<"Line: "<<this->line<<",col: "<<this->col;
    }
};



class errors{

    public:
    Position pos;
    virtual void print()const =0;


};
class reporter{

    bool iserror;
    public:
    reporter(){iserror=0;}
    inline void issue(const errors &cur){cur.print();iserror=1;}
    inline bool haserror(){return iserror;}

};

class killprocess:public reporter{
//RE的报错
    public:
    killprocess(){}
    inline void issue(const errors &cur){
        cerr<<"-------Run-Time-Error:The process has been halted.-----------"<<endl;
        cur.print();
        cerr<<"-------------------------------------------------------------"<<endl;
        exit(1);
    }
}killprocess;


class HavePreviousZero:public errors{

//报错：数字开头有前导0
    public:
    HavePreviousZero(const Position &pos){this->pos=pos;}
    virtual void print()const{
        cerr<<"Syntex Error: Number cannot have previous zeros.at Line:"<<pos.line<<",col:"<<pos.col<<endl;
    }
};

class UndefinedIdent:public errors{

    public:
    UndefinedIdent(const Position &pos){this->pos=pos;}
    virtual void print()const{
        cerr<<"Syntex Error:Have a undefined ident/keyword.at Line"<<pos.line<<",col:"<<pos.col<<endl;
    }
};


class Vardefined:public errors{

    string name;
    public:
    Vardefined(const Position &pos,const string &name){
        this->pos=pos;
        this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:Variable has been defined before.at Line"<<pos.line<<",col:"<<pos.col<<endl;
    }


};

class IdentNotDefined:public errors{

    string name;
    public:
    IdentNotDefined(const Position &pos,const string &name){
        this->pos=pos;
        this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:Function or Variable hasn't been defined.at Line:"<<pos.line<<",col:"<<pos.col<<endl;
    }


};

class ArrayInExpr:public errors{

    string name;
    public:
    ArrayInExpr(const Position &pos,const string &name){
        this->pos=pos;this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:A expression includes an array.at Line: "<<pos.line<<",col:"<<pos.col<<endl;
    }

};

class SyntexError:public errors{

    public:
    SyntexError(const Position &pos){
        this->pos=pos;
    }
    virtual void print()const{
        cerr<<"Syntex Error At Line: "<<pos.line<<",col:"<<pos.col<<endl;
        exit(0);
    }

};

class SegmentError:public errors{

    public:
    SegmentError(const Position &pos){this->pos=pos;}
    virtual void print()const{
        cerr<<"Segment Fault At line: "<<pos.line<<",col:"<<pos.col<<endl;
    }    

};

class BadArrayOpt:public errors{

    public:
    BadArrayOpt(){}
    virtual void print()const{
        cerr<<"Bad Array Operation."<<endl;
    }

};

class FuncDefined:public errors{

    string name;
    public:
    FuncDefined(const Position &pos,const string &name){
        this->pos=pos;
        this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:Function has been defined before.At line:"<<pos.line<<",col:"<<pos.col<<endl;
    }

};

class VariableNotArray:public errors{

    string name;
    public:
    VariableNotArray(const Position &pos,const string &name){
        this->pos=pos;
        this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:Variable is not an array.At line: "<<pos.line<<",col="<<pos.col<<endl;
    }

};

class ArrayLengthExceed:public errors{

    string name;
    public:
    ArrayLengthExceed(const Position &pos,const string &name){
        this->pos=pos;
        this->name=name;
    }
    virtual void print()const{
        cerr<<"Syntex Error:Array Length exceed.At line:"<<pos.line<<",col="<<pos.col<<endl;
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
        if(type==ERROR)cout<<"ERROR"<<endl;
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

    reporter reporters;//报错

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
            readchar();if(isdigit(ch)){
                reporters.issue(HavePreviousZero(pos));
                //报错：数字有前导0
            }
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
            reporters.issue(UndefinedIdent(pos));//报错：未定义标识符
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
    inline bool iserror(){return reporters.haserror();}
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
        MakeOpt("+-*/%#()[]{}=!^<>;,");
        
        MakeIdent(IDENT);
        MakeNumber(NUMBER);
        MakeError(ERROR);
    }

}lexer;




class AST;
class VarDefiniton;
class FuncDefinition;
class Block;
class TopLevel;
class BinaryOpt;
class Ident;
class If;
class For;
class While;
class Return;
class ArrayOpt;
class Exec;
class Expr;

class CinOpt;
class CoutOpt;
class Putchar;


class visitor{

    public:
    virtual void VisitTree(AST *that){}
    virtual void VisitVarDefinition(VarDefiniton *that){}
    virtual void VisitFuncDefinition(FuncDefinition *that){}
    virtual void VisitBlock(Block *that){}
    virtual void VisitTopLevel(TopLevel *that){}
    virtual void VisitBinartOpt(BinaryOpt *that){}
    virtual void VisitIdent(Ident *that){}
    virtual void VisitIf(If *that){}
    virtual void VisitFor(For *that){}
    virtual void VisitWhile(While *that){}
    virtual void VisitReturn(Return *that){}
    virtual void VisitArrayOpt(ArrayOpt *that){}
    virtual void VisitExec(Exec *that){}

    virtual void VisitCinOpt(CinOpt *that){}
    virtual void VisitCoutOpt(CoutOpt *that){}
    virtual void VisitPutchar(Putchar *that){}

};

class AST{

    Position pos;
    public:
    AST(){}
    inline void setpos(const Position &pos){this->pos=pos;}
    inline Position getpos(){return this->pos;}
    AST(const Position &pos){setpos(pos);}
    virtual ~AST(){}
    virtual void visit(visitor &vis){vis.VisitTree(this);}
    virtual void print(int tab){assert(false);}



};

class Var{

    inline bool checkmul(int a,int b){return 1LL*a*b<=2147483647;}
    inline int checkindex(const vector<int> &indexs){
        int size=1;
        for(int i=0;i<indexs.size();i++){
            int v=indexs[i];
            if(v<0||!checkmul(size,v)){killprocess.issue(BadArrayOpt());}
            size*=v;
        }
        return size;
    }

    public:

    string type,name;
    vector<int> indexs;
    int idscope,inscope,size;
    bool leftvalue;

    Var(){}
    Var(const string &type,const string &name,const vector<int> &indexs){
        this->type=type;this->name=name;
        this->indexs=indexs;
        idscope=0;inscope=-1;
        leftvalue=1;
        size=checkindex(indexs);
    }
    Var(const string &type,const string &name){
        this->type=type;this->name=name;
        idscope=0;inscope=-1;
        leftvalue=1;
        size=1;
    }
    inline int GetArrayPos(const vector<int> &indexs){
        int pre=1,cnt=0,len=indexs.size();
        for(int i=len-1;~i;i--){
            cnt+=pre*indexs[i];
            pre*=this->indexs[i];
        }
        return cnt;
    }
    inline int GetArrayPos(int *indexs,int len){
        int pre=1,cnt=0;
        for(int i=len-1;~i;i--){
            cnt+=pre*indexs[i];
            pre*=this->indexs[i];
        }
        return cnt;
    }
    inline void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        cout<<type<<" "<<name<<endl;
        if(type=="array")for(int i=0;i<indexs.size();i++)cout<<"["<<indexs[i]<<"] ";
    }

    

};

class VarDefiniton:public AST{

    public:
    vector<Var> vars;
    int size;
    VarDefiniton(const Position &pos,const vector<Var> &var){
        setpos(pos);vars=var;
    }
    virtual ~VarDefiniton(){}
    virtual void visit(visitor &vis){vis.VisitVarDefinition(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("Vardefinition: ");assert(vars.size()>0);
        for(int i=0;i<vars.size();i++){
            vars[i].print(tab+2);
            if(i==vars.size()-1)putchar(' ');else putchar('\n');
        }
    }


};

class FuncDefinition:public AST{

    public:
    string name;
    vector<Var> argv;
    bool ismain;
    int size;
    AST *stmt;

    FuncDefinition(const Position &pos,const string &name,const vector<Var> &argv,AST *stmt){
        setpos(pos);
        this->name=name;
        this->argv=argv;
        this->stmt=stmt;
        if(name=="main")ismain=1;else ismain=0;
        size=argv.size();
    }
    virtual ~FuncDefinition(){if(stmt!=nullptr)delete stmt;}
    virtual void visit(visitor &vis){vis.VisitFuncDefinition(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("FuncDefinition: ");
        cout<<name<<endl;
        if(argv.size()==0)puts("<empty argvs>");
        else for(int i=0;i<argv.size();i++){
            argv[i].print(tab+2);
            if(i==argv.size()-1)putchar('\n');
            else putchar(' ');
        }
        if(stmt==nullptr)puts("<empty stmts>");
        else stmt->print(tab+2);
    }

};


class Block:public AST{

    public:
    vector<AST*> levels;
    Block(const Position &pos,const vector<AST*> levels){
        setpos(pos);
        this->levels=levels;
    }
    virtual ~Block(){}
    virtual void visit(visitor &vis){vis.VisitBlock(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        puts("Block: ");
        for(int i=0;i<levels.size();i++)levels[i]->print(tab+2);
    }


};

class TopLevel:public AST{

    public:
    vector<AST*> levels;
    TopLevel(const Position &pos,vector<AST*> &levels){
        setpos(pos);
        this->levels=levels;
    }
    virtual ~TopLevel(){
        for(int i=0;i<levels.size();i++)delete(levels[i]);
    }
    virtual void visit(visitor &vis){vis.VisitTopLevel(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        puts("TopLevel");
        for(int i=0;i<levels.size();i++)levels[i]->print(tab+2);
    }

};

class Expr:public AST{

    virtual void visit(visitor &vis)=0;
    virtual void print(int tab)=0;

};

class BinaryOpt:public Expr{

    public:
    string ident;
    int type;
    AST *ls,*rs;
    BinaryOpt(const Position &pos,const int type,const string &ident,AST *ls,AST *rs){
        setpos(pos);
        this->ident=ident;this->type=type;
        this->ls=ls;this->rs=rs;
    }
    ~BinaryOpt(){
        if(ls!=nullptr)delete(ls);
        if(rs!=nullptr)delete(rs);
    }
    virtual void visit(visitor &vis){vis.VisitBinartOpt(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("BinaryOpt: ");
        cout<<ident<<endl;
        if(ls!=nullptr)ls->print(tab+2);else puts("<empty ls>");
        if(rs!=nullptr)rs->print(tab+2);else puts("<empty rs>");
    }
};


class Ident:public Expr{

    public:
    int data,type;
    string name;
    Var var;
    Ident(const Position &pos,int data){
        setpos(pos);
        this->data=data;type=INT;
    }
    Ident(const Position &pos,int type,int data){
        setpos(pos);
        this->data=data;this->type=type;
    }
    Ident(const Position &pos,const string &name){
        setpos(pos);
        this->type=KEYWORD;this->name=name;
    }
    virtual void visit(visitor &vis){vis.VisitIdent(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)printf(" ");
        if(type==INT)printf("int:%d\n",data);
        else if(type==KEYWORD)printf("var :%s\n",name.c_str());
        else printf("endl\n");
    }


};

class ArrayOpt:public Expr{

    public:
    string name;
    int idscope,insocope;
    vector<AST*> at;
    Var var;
    ArrayOpt(const Position &pos,const string &name,const vector<AST*> &at){
        setpos(pos);
        this->name=name;this->at=at;
    }
    virtual ~ArrayOpt(){}
    virtual void visit(visitor &vis){vis.VisitArrayOpt(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("Array: ");
        cout<<name<<" ,size="<<at.size()<<endl;
        for(int i=0;i<at.size();i++)at[i]->print(tab+2);
    }

};

class Exec:public AST{

    public:
    string name;
    vector<AST*> argv;
    FuncDefinition *func;
    Exec(const Position &pos,const string &name,const vector<AST*> &argv){
        setpos(pos);
        this->name=name;this->argv=argv;
        func=nullptr;
    }
    virtual ~Exec(){
        for(int i=0;i<argv.size();i++)delete(argv[i]);
    }
    virtual void visit(visitor &vis){vis.VisitExec(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        cout<<"Exec "<<name<<endl;
        for(int i=0;i<argv.size();i++)argv[i]->print(tab+2);
    }

};

class If : public AST{

    public:
    AST* expr;
    AST* TrueBrench;
    AST* FalseBrench;
    If(const Position &pos,AST *Expr,AST *TrueBrench){
        setpos(pos);
        this->expr=Expr;this->TrueBrench=TrueBrench;
        this->FalseBrench=nullptr;
    }
    If(const Position &pos,AST *Expr,AST *TrueBrench,AST *FalseBrench){
        setpos(pos);
        this->expr=Expr;this->TrueBrench=TrueBrench;
        this->FalseBrench=FalseBrench;
    }
    virtual ~If(){
        if(expr!=nullptr)delete(expr);
        if(TrueBrench!=nullptr)delete(TrueBrench);
        if(FalseBrench!=nullptr)delete(FalseBrench);
    }
    virtual void visit(visitor &vis){vis.VisitIf(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("if ");
        expr->print(tab+2);
        if(TrueBrench!=nullptr)TrueBrench->print(tab+2);
        else puts("<empty truebrench>");
        if(FalseBrench!=nullptr)FalseBrench->print(tab+2);
        else puts("<empty falsebrunch>");
    }

};

class While:public AST{

    public:
    AST* expr;
    AST* stmt;
    While(const Position &pos,AST *expr,AST *stmt){
        setpos(pos);
        this->expr=expr;this->stmt=stmt;
    }
    virtual ~While(){
        if(expr!=nullptr)delete(expr);
        if(stmt!=nullptr)delete(stmt);
    }
    virtual void visit(visitor &vis){vis.VisitWhile(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)printf(" ");
        printf("while: ");
        expr->print(tab+2);
        if(stmt!=nullptr)stmt->print(tab+2);
        else puts("<empty stmt>");
    }

};

class For:public AST{

    public:
    AST* init;
    AST* expr;
    AST* delta;
    AST* stmt;
    For(const Position &pos,AST *init,AST *expr,AST *delta,AST *stmt){
        setpos(pos);
        this->init=init;this->expr=expr;
        this->delta=delta;this->stmt=stmt;
    }
    virtual ~For(){
        if(init!=nullptr)delete(init);
        if(expr!=nullptr)delete(expr);
        if(delta!=nullptr)delete(delta);
        if(stmt!=nullptr)delete(stmt);
    }
    virtual void visit(visitor vis){vis.VisitFor(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        if(init!=nullptr)init->print(tab+2);
        else puts("<empty init>");
        if(expr!=nullptr)expr->print(tab+2);
        else puts("<empty expr>");
        if(delta!=nullptr)delta->print(tab+2);
        else puts("<empty delta>");
        if(stmt!=nullptr)stmt->print(tab+2);
        else puts("<empty stmt>");
    }

};

class Return:public AST{

    public:
    AST* expr;
    Return(const Position &pos,AST *expr){
        setpos(pos);
        this->expr=expr;
    }
    virtual ~Return(){
        if(expr!=nullptr)delete(expr);
    }
    virtual void visit(visitor &vis){vis.VisitReturn(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        printf("return :\n");
        if(expr!=nullptr)expr->print(tab+2);
    }

};


class CinOpt:public Expr{

    public:
    vector<AST*> vars;
    CinOpt(const Position &pos,const vector<AST*> &vars){
        setpos(pos);this->vars=vars;
    }
    virtual ~CinOpt(){}
    virtual void visit(visitor &vis){vis.VisitCinOpt(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        puts("cin:");
        for(int i=0;i<vars.size();i++)vars[i]->print(tab+2);
    }

};

class CoutOpt:public Expr{

    public:

    vector<AST*> vars;
    CoutOpt(const Position &pos,const vector<AST*> &vars){
        setpos(pos);this->vars=vars;
    }
    virtual ~CoutOpt(){
        for(int i=0;i<vars.size();i++)if(vars[i]!=nullptr)delete(vars[i]);
    }
    virtual void visit(visitor &vis){vis.VisitCoutOpt(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        puts("cout:");
        for(int i=0;i<vars.size();i++)vars[i]->print(tab+2);
    }

};

class Putchar:public AST{

    public:
    AST *expr;
    Putchar(const Position &pos,AST* Tree){
        setpos(pos);this->expr=Tree;
    }
    virtual ~Putchar(){
        if(expr!=nullptr)delete(expr);
    }
    virtual void visit(visitor &vis){vis.VisitPutchar(this);}
    virtual void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
        puts("putchar:");
        if(expr!=nullptr)expr->print(tab+2);
    }

};

//scope部分

const int GLOBAL=-2;
template <typename T>

class Scope{
    map<string,pair<T,int> >table;
    int id;

    public:
    Scope(){id=0;}
    inline bool count(const string &s){return table.count(s);}
    inline pair<T,int> find(const string &s){
        assert(table.count(s));
        return table[s];
    }
    inline int create(const string &s,const T &var,int len=1){
        assert(!count(s));
        table[s]=make_pair(var,id);
        id+=len;
        return id-len;
    }
    inline int size(){return id;}

};

class ScopeStack{

    vector<Scope<Var> >stack;
    int id,maxsize;

    public:
    ScopeStack(){id=0;maxsize=0;}
    inline void init(){stack.push_back(Scope<Var> ());}
    inline int topsize(){return stack[stack.size()-1].size();}
    inline void close(){
        id-=stack[stack.size()-1].size();
        stack.pop_back();
    }
    inline int count(const string &s,bool through=1){
        if(!through)return stack[stack.size()-1].count(s);
        for(int i=stack.size()-1;~i;i--)if(stack[i].count(s))return 1;
        return 0;
    }
    inline pair<pair<Var,int>,int> find(const string &s){
        assert(count(s));
        for(int i=stack.size()-1;~i;i--)if(stack[i].count(s))return make_pair(stack[i].find(s),i==0?GLOBAL:stack.size()-1);
    }
    inline int create(const string &s,const Var &var,int len=1){
        id+=len;
        maxsize=max(maxsize,id);
        return stack[stack.size()-1].create(s,var,len);
    }
    inline int size(){return maxsize;}

}scopeStack;

//type-visitor:
//用于查看变量，以及输出AST

class TypeVisitor:public visitor{
    
    ScopeStack stack;
    reporter report;
    Scope<FuncDefinition*> FunctionScope;
    inline void newVar(const Position &pos,Var &cur){
        if(stack.count(cur.name,0)){
            report.issue(Vardefined(pos,cur.name));
        }
        else{
            if(cur.type!="array"){
                cur.leftvalue=1;
                cur.inscope=stack.create(cur.name,cur);
                cur.idscope=0;
            }
            else{
                cur.leftvalue=0;
                cur.inscope=stack.create(cur.name, cur ,cur.size);
                cur.idscope=0;
            }
        }
    }
    inline void useVar(const Position &pos,Var &cur){
        if(!stack.count(cur.name)){
            report.issue(IdentNotDefined(pos,cur.name));
        }
        else{
            pair<pair<Var,int>,int> v=stack.find(cur.name);
            if(v.first.first.type=="array"){report.issue(ArrayInExpr(pos,cur.name));}
            else{
                cur.idscope=v.second;
                cur.inscope=v.first.second;
            }
        }
    }
    inline void useArray(const Position &pos,const string &name,ArrayOpt* aopt){
        if(!stack.count(name)){report.issue(IdentNotDefined(pos,name));}
        else{
            pair<pair<Var,int>,int> v=stack.find(name);
            if(v.first.first.type!="array")report.issue(VariableNotArray(pos,name));
            else if(aopt->at.size()!=v.first.first.indexs.size())report.issue(ArrayLengthExceed(pos,name));
            else{
                aopt->idscope=v.second;
                aopt->insocope=v.first.second;
                aopt->var=v.first.first;
            }
        }
    }

    public:
    virtual void visitAST(AST *that){assert(false);}
    virtual void visitTopLevel(TopLevel *that){
        stack.init();
        for(int i=0;i<that->levels.size();i++)that->levels[i]->visit(*this);
        stack.close();
    }
    virtual void visitVarDefinition(VarDefiniton *that){
        that->size=0;
        for(int i=0;i<that->vars.size();i++){
            newVar(that->getpos(),that->vars[i]);
            that->size+=that->vars[i].size;
        }
    }
    virtual void visitFuncDefinition(FuncDefinition *that){
        if(FunctionScope.count(that->name))report.issue(FuncDefined(that->getpos(),that->name));
        else{
            FunctionScope.create(that->name,that);
            stack.init();
            for(int i=0;i<that->argv.size();i++)newVar(that->getpos(),that->argv[i]);
            if(that->stmt!=nullptr)that->stmt->visit(*this);
            stack.close();
        }

    }
    virtual void visitBlock(Block *that){
        stack.init();
        for(int i=0;i<that->levels.size();i++)that->levels[i]->visit(*this);
        stack.close();

    }
    virtual void visitIf(If *that){
        that->expr->visit(*this);
        if(that->TrueBrench!=nullptr)that->TrueBrench->visit(*this);
        if(that->FalseBrench!=nullptr)that->FalseBrench->visit(*this);
    }
    virtual void visitWhile(While *that){
        that->expr->visit(*this);
        if(that->stmt!=nullptr)that->stmt->visit(*this);
    }
    virtual void visitFor(For *that){
        stack.init();
        if(that->init!=nullptr)that->init->visit(*this);
        if(that->expr!=nullptr)that->expr->visit(*this);
        if(that->delta!=nullptr)that->delta->visit(*this);
        if(that->stmt!=nullptr)that->stmt->visit(*this);
        stack.close();
    }
    virtual void visitReturn(Return *that){
        that->expr->visit(*this);
    }
    virtual void visitBinaryOpt(BinaryOpt *that){
        that->ls->visit(*this);
        that->rs->visit(*this);
    }
    virtual void visitIdent(Ident *that){
        if(that->type==KEYWORD){
            that->var=Var("int",that->name);
            useVar(that->getpos(),that->var);
        }
    }
    virtual void visitExec(Exec *that){
        if(!FunctionScope.count(that->name))report.issue(IdentNotDefined(that->getpos(),that->name));
        else that->func=FunctionScope.find(that->name).first;
        for(int i=0;i<that->argv.size();i++)that->argv[i]->visit(*this);
    }
    virtual void visitArrayopt(ArrayOpt *that){
        useArray(that->getpos(),that->name,that);
        for(int i=0;i<that->at.size();i++)that->at[i]->visit(*this);
    }
    inline bool iserror(){
        return report.haserror();
    }
    inline int size(){return stack.topsize();}


    virtual void VisitCinOpt(CinOpt *that){
        for(int i=0;i<that->vars.size();i++)that->vars[i]->visit(*this);
    }
    virtual void VisitCoutOpt(CoutOpt *that){
        for(int i=0;i<that->vars.size();i++)that->vars[i]->visit(*this);
    }
    virtual void VisitPutChar(Putchar *that){
        that->expr->visit(*this);
    }

}typevisitor;


class SemiValue{

    public:
    int pos,ret;
    SemiValue(){pos=0;ret=0;}
    SemiValue(int ret,int pos){
        this->ret=ret;this->pos=pos;
    }

};

class runvisitor:public visitor{

    vector<int> pool;
    int top,ret;
    struct state{
        int ret,flag;
        state(){ret=0;flag=0;}
    };
    vector<state> stack;
    vector<int> index;
    SemiValue value;
    inline state& tope(){
        return stack[stack.size()-1];
    }
    inline void init(){index.push_back(top);}
    inline void close(){
        top=index[index.size()-1];
        index.pop_back();
    }
    inline int GetStackPos(int idscope,int inscope){
        if(idscope==GLOBAL)return inscope;
        return index[index.size()-1-idscope]+inscope;
    }
    inline void newfunciton(){
        stack.push_back(state());
    }
    inline void killfuncion(){
        stack.pop_back();
    }
    inline void execfunction(FuncDefinition *func,int *argv,size_t len){
        init();newfunciton();
        top+=func->size;
        pool.resize(top);
        size_t cnt=min(func->size,(int)len);
        for(int i=0;i<cnt;i++)pool[GetStackPos(func->argv[i].idscope,func->argv[i].inscope)]=argv[i];
        func->stmt->visit(*this);
        value=SemiValue(tope().ret,-1);
        killfuncion();
        close();
    }
    public:
    virtual void visitTopLevel(TopLevel *that){
        init();
        for(int i=0;i<that->levels.size();i++)that->levels[i]->visit(*this);
        close();
    }
    virtual void visitVarDefiniton(VarDefiniton *that){
        if(!stack.empty()&&tope().flag)return;
        top+=that->size;
        pool.resize(top);
    }
    virtual void visitFuncDefinition(FuncDefinition *that){
        if(that->ismain){
            int *array;
            execfunction(that,array,0);
        }
    }
    virtual void visitBlock(Block *that){
        if(!stack.empty()&&tope().flag)return;
        init();
        for(int i=0;i<that->levels.size();i++)that->levels[i]->visit(*this);
        close();
    }
    virtual void visitBinaryOpt(BinaryOpt *that){
        if(!stack.empty()&&tope().flag)return;
        SemiValue lc,rc;
        that->ls->visit(*this);
        lc=value;
        that->rs->visit(*this);
        rc=value;
        switch (that->type)
        {
        case '=':
            value=SemiValue(rc.ret,lc.pos);
            pool[lc.pos]=rc.ret;
            break;
        case '^':
            value=SemiValue(lc.ret^rc.ret,-1);
            break;
        case '%':
            value=SemiValue(lc.ret%rc.ret,-1);
            break;
        case '!':
            value=SemiValue(!rc.ret,-1);
            break;
        case '+':
            value=SemiValue(lc.ret+rc.ret,-1);
            break;
        case '-':
            value=SemiValue(lc.ret-rc.ret,-1);
            break;
        case '*':
            value=SemiValue(lc.ret*rc.ret,-1);
            break;
        case '/':
            value=SemiValue(lc.ret/rc.ret,-1);
            break;
        case '<':
            value=SemiValue(lc.ret<rc.ret,-1);
            break;
        case '>':
            value=SemiValue(lc.ret>rc.ret,-1);
            break;
        case L_EQ:
            value=SemiValue(lc.ret==rc.ret,-1);
            break;
        case L_LEQ:
            value=SemiValue(lc.ret<=rc.ret,-1);
            break;
        case L_REQ:
            value=SemiValue(lc.ret>=rc.ret,-1);
            break;
        case L_NEQ:
            value=SemiValue(lc.ret!=rc.ret,-1);
            break;
        case L_AND:
            value=SemiValue(lc.ret&&rc.ret,-1);
            break;
        case L_OR:
            value=SemiValue(lc.ret||rc.ret,-1);
            break;
        default:
            assert(false);
            break;
        }
    }

    virtual void visitIdent(Ident *that){
        if(!stack.empty()&&tope().flag)return;
        int cur;
        switch (that->type)
        {
        case INT:
            value=SemiValue(that->data,-1);
            break;
        case KEYWORD:
            cur=GetStackPos(that->var.idscope,that->var.inscope);
            value=SemiValue(pool[cur],cur);
            break;
        default:
            value=SemiValue(0,-2);
            break;
        }
    }
    virtual void visitExec(Exec *that){
        if(!stack.empty()&&tope().flag)return;
        int argv[that->argv.size()];
        for(int i=0;i<that->argv.size();i++){
            that->argv[i]->visit(*this);
            argv[i]=value.ret;
        }
        execfunction(that->func,argv,that->argv.size());
    }
    virtual void visitArrayOpt(ArrayOpt *that){
        if(!stack.empty()&&tope().flag)return;
        int indexs[that->at.size()];
        for(int i=0;i<that->at.size();i++){
            that->at[i]->visit(*this);
            indexs[i]=value.ret;
        }
        int cur=GetStackPos(that->idscope,that->insocope)+that->var.GetArrayPos(indexs,that->at.size());
        value=SemiValue(pool[cur],cur);
    }
    virtual void visitIf(If *that){
        if(!stack.empty()&&tope().flag)return;
        that->expr->visit(*this);
        if(value.ret){
            if(that->TrueBrench!=nullptr)that->TrueBrench->visit(*this);
        }
        else{
            if(that->FalseBrench!=nullptr)that->FalseBrench->visit(*this);
        }
    }
    virtual void visitWhile(While *that){
        if(!stack.empty()&&tope().flag)return;
        while(that->expr->visit(*this),value.ret)that->stmt->visit(*this);
    }
    virtual void visitFor(For *that){
        if(!stack.empty()&&tope().flag)return;
        init();
        if(that->init!=nullptr)that->init->visit(*this);
        for(;that->expr!=nullptr&&(that->expr->visit(*this),value.ret);that->delta->visit(*this))
            that->stmt->visit(*this);
        close();
    }
    virtual void visitReturn(Return *that){
        if(!stack.empty()&&tope().flag)return;
        that->expr->visit(*this);
        tope().flag=1;
        tope().ret=value.ret;
    }
    virtual void VisitCinOpt(CinOpt *that){
        if(!stack.empty()&&tope().flag)return;
        for(int i=0;i<that->vars.size();i++){
            that->vars[i]->visit(*this);
            pool[value.pos]=read();
        }
        value=SemiValue(0,-1);
    }
    virtual void visitCoutOpt(CoutOpt *that){
        if(!stack.empty()&&tope().flag)return;
        for(int i=0;i<that->vars.size();i++){
            that->vars[i]->visit(*this);
            if(value.pos==-2)puts("");
            else printf("%d ",value.ret);
        }
        value=SemiValue(0,-1);
    }
    virtual void VisitPutChar(Putchar *that){
        if(!stack.empty()&&tope().flag)return;
        that->expr->visit(*this);
        printf("%c ",value.ret);
    }

}runvisitor;


//parser主体部分

class parser{

    vector<token> tokens;
    reporter report;
    int pre;
    AST *program;

    inline token lookahead(){
        return tokens[pre];
    }
    inline void match(int id){
        if(pre==-1)return;
        else if(pre>tokens.size()){
            report.issue(SyntexError(tokens[tokens.size()-1].pos));
            pre=-1;
        }
        else if(tokens[pre].type!=id)report.issue(SyntexError(tokens[pre].pos));
        else pre++;
    }

    public:


    AST* head(){
        return toplevel();
    }
    AST* toplevel(){
        AST *tmp=nullptr;
        vector<AST*> argv;
        Position pos=lookahead().pos;
        while(true){
            switch (lookahead().type)
            {
            case EXIT:
                match(EXIT);
                return new TopLevel(pos,argv);
                break;
            case INT:
                match(INT);
                break;
            default:
                killprocess.issue(SyntexError(lookahead().pos));
                break;
            }
            argv.push_back(idents());
        }
        
    }

    AST* idents(){
        string name=lookahead().ident;
        Position pos=lookahead().pos;
        vector<Var> opt;
        AST* Tree;
        match(IDENT);
        switch (lookahead().type){
            case '(':
                Tree=funcdef(opt);
                return new FuncDefinition(pos,name,opt,Tree);
            case ',':
            case ';':
            case '[':
                vardef(name,opt);
                varsdef(opt);
                match(';');
                return new VarDefiniton(pos,opt);
            default:
            killprocess.issue(SyntexError(lookahead().pos));
        }
        assert(false);
    }

    void parmas(vector<Var> &opt){
        string name;
        if(lookahead().type==INT){
            match(INT);
            name=lookahead().ident;
            match(IDENT);
            opt.push_back(Var("int",name));
            if(lookahead().type==','){match(',');parmas(opt);}
        }
    }
    AST* funcdef(vector<Var> &opt){
        match('(');parmas(opt);match(')');
        AST *Tree=blockdef();
        return Tree;
    }
    AST* blockdef(){
        Position pos=lookahead().pos;
        match('{');vector<AST*> Trees;
        stmtdef(Trees);
        match('}');
        return new Block(pos,Trees);
    }
    AST* stmtdef(vector<AST*> &Tree){
        switch(lookahead().type){
            case IDENT:
            case NUMBER:
            case '(':
            case '+':
            case '!':
            case '-':
            case FOR:
            case CIN:
            case COUT:
            case IF:
            case RETURN:
            case WHILE:
            case INT:
                Tree.push_back(stmtdef());
                stmtdef(Tree);
                break;

        }
    }
    AST* stmtdef(){
        Position pos=lookahead().pos;
        vector<Var> vars;
        vector<AST*> Trees;
        AST* Tree;
        switch(lookahead().type){
            case IF:
                return ifdef();
            case WHILE:
                return whiledef();
            case FOR:
                return fordef();
            case RETURN:
                return returndef();
            case INT:
                match(INT);
                vardef(vars);
                varsdef(vars);
                match(';');
               return new VarDefiniton(pos,vars);
            case '{':
                match('{');
                stmtdef(Trees);
                match('}');
                return new Block(pos,Trees);
            case ':':
                match(';');
                return nullptr;
            default:
                Tree=exprdef();
                match(';');
                return Tree;
        }

    }

    AST* ifdef(){
        assert(lookahead().type==IF);
        Position pos=lookahead().pos;
        match(IF);match('(');
        AST *a=exprdef();
        match(')');
        AST *b=stmtdef();
        if(lookahead().type==ELSE){
            match(ELSE);
            return new If(pos,a,b,stmtdef());
        }
        return new If(pos,a,b);
    }

    AST* fordef(){
        Position pos=lookahead().pos;
        match(FOR);match('(');
        AST* init=forstmt();match(';');
        AST *expr=expropt();match(';');
        AST *delta=expropt();match(')');
        return new For(pos,init,expr,delta,stmtdef());
    }

    AST *forstmt(){
        AST* Tree;vector<Var> vars;
        Position pos=lookahead().pos;
        switch(lookahead().type){
            case IDENT:
            case NUMBER:
            case '(':
            case '+':
            case '!':
            case CIN:
            case COUT:
            case '-':
                return expropt();
            case INT:
                match(INT);
                vardef(vars);
                varsdef(vars);
                return new VarDefiniton(pos,vars);
        }
        return nullptr;
    }
    AST* expropt(){
        Position pos=lookahead().pos;
        switch(lookahead().type){
            case IDENT:
            case NUMBER:
            case CIN:
            case COUT:
            case '(':
            case '+':
            case '!':
            case '-':
                return expropt();
            default:
                return new Ident(pos,1);
        }
    }
    void vardef(const string &name,vector<Var> &opt){
        vector<int> indexs;
        if(lookahead().type=='['){
            arrayidx(indexs);
            opt.push_back(Var("array",name,indexs));
        }
        else opt.push_back(Var("int",name));
    }
    AST *whiledef(){
        Position pos=lookahead().pos;
        match(WHILE);match('(');
        AST* expr=exprdef();match(')');
        return new While(pos,expr,stmtdef());
    }

    AST *returndef(){
        Position pos=lookahead().pos;
        match(RETURN);
        AST* expr=exprdef();match(';');
        return new Return(pos,expr);
    }

    void vardef(vector<Var> &opt){
        vector<int> indexs;string name;
        if(lookahead().type==IDENT){
            name=lookahead().ident;
            match(IDENT);
            vardef(name,opt);
        }
    }
    void arrayidx(vector<int> &indexs){
        int id;
        if(lookahead().type=='['){
            match('[');
            id=lookahead().number;
            match(NUMBER);
            indexs.push_back(id);match(']');
            arrayidx(indexs);
        }
    }
    void arrayidxexpr(vector<AST*> &indexs){
        if(lookahead().type=='['){
            match('[');indexs.push_back(exprdef());
            match(']');
            arrayidxexpr(indexs);
        }
    }
    void varsdef(vector<Var> &opt){
        if(lookahead().type==','){
            match(',');
            vardef(opt);varsdef(opt);
        }        
    }
    AST* exprdef(){
        Position pos=lookahead().pos;
        vector<AST*> exprs;
        switch (lookahead().type){
        case COUT:
            match(COUT);match(SHIFTL);
            coutopt(exprs);
            return new CoutOpt(pos,exprs);
            break;
        case CIN:
            match(CIN);match(SHIFTR);
            cinopt(exprs);
            return new CinOpt(pos,exprs);
            break;
        
        default:
            return lv9();
            break;
        }
        
    }

    void cinopt(vector<AST*> &vars){
        vars.push_back(leftval());
        switch(lookahead().type){
            case SHIFTR:
                match(SHIFTR);
                cinopt(vars);
        }
    }
    void coutopt(vector<AST*> &vars){
        vars.push_back(exprdef());
        switch(lookahead().type){
            case SHIFTL:
                match(SHIFTL);
                coutopt(vars);
        }
    }
    
    AST* leftval(){
        Position pos=lookahead().pos;
        string name=lookahead().ident;
        match(IDENT);
        vector<AST*> Trees;AST* Tree;
        if(lookahead().type=='['){
            arrayidxexpr(Trees);
            return new ArrayOpt(pos,name,Trees);
        }
        else return new Ident(pos,name);
    }
    
    AST* lv0ident(){
        Position pos=lookahead().pos;string name=lookahead().ident;
        match(IDENT);
        vector<AST*> Trees;AST* Tree;
        switch (lookahead().type)
        {
        case '[':
            arrayidxexpr(Trees);
            return new ArrayOpt(pos,name,Trees);
            break;
        case '(':
            match('(');calc(Trees);match(')');
            return new Exec(pos,name,Trees);
            break;
        default:
            return new Ident(pos,name);
            break;
        }
    }
    AST* lv0(){
        Position pos=lookahead().pos;AST* Tree;int data;
        switch (lookahead().type)
        {
        case '(':
            match('(');Tree=exprdef();match(')');return Tree;
            break;
        case NUMBER:
            data=lookahead().number;
            match(NUMBER);
            return new Ident(pos,data);
            break;
        case IDENT:
            return lv0ident();
            break;
        case ENDL:
            match(ENDL);
            return new Ident(pos,ENDL,'\n');
        default:
            report.issue(SyntexError(pos));
            break;
        }
    }

    AST *lv1(){
        Position pos=lookahead().pos;
        switch (lookahead().type)
        {
        case '!':
            match('!');
            return new BinaryOpt(pos,'!',"!",(AST*)new Ident(pos,0),lv1());
            break;
        case '+':
            match('+');
            return new BinaryOpt(pos,'+',"+",(AST*)new Ident(pos,0),lv1());
            break;
        case '-':
            match('-');
            return new BinaryOpt(pos,'-',"-",(AST*)new Ident(pos,0),lv1());
            break;
        
        default:
            return lv0();
            break;
        }
    }

    AST* lv2(){
        AST *ls=lv1();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case '*':
                    match('*');
                    ls=new BinaryOpt(pos,'*',"*",ls,lv1());
                    break;
                case '/':
                    match('/');
                    ls=new BinaryOpt(pos,'/',"/",ls,lv1());
                    break;
                case '%':
                    match('%');
                    ls=new BinaryOpt(pos,'%',"%",ls,lv1());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST* lv3(){
        AST* ls=lv2();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case '+':
                    match('+');
                    ls=new BinaryOpt(pos,'+',"+",ls,lv2());
                    break;
                case '-':
                    match('-');
                    ls=new BinaryOpt(pos,'-',"-",ls,lv2());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST* lv4(){
        AST *ls=lv3();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case '<':
                    match('<');
                    ls=new BinaryOpt(pos,'<',"<",ls,lv3());
                    break;
                case '>':
                    match('>');
                    ls=new BinaryOpt(pos,'>',">",ls,lv3());
                    break;
                case L_LEQ:
                    match(L_LEQ);
                    ls=new BinaryOpt(pos,L_LEQ,"<=",ls,lv3());
                    break;
                case L_REQ:
                    match(L_REQ);
                    ls=new BinaryOpt(pos,L_REQ,">=",ls,lv3());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST *lv5(){
        AST *ls=lv4();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case L_EQ:
                    match(L_EQ);
                    ls=new BinaryOpt(pos,L_EQ,"==",ls,lv4());
                    break;
                case L_NEQ:
                    match(L_NEQ);
                    ls=new BinaryOpt(pos,L_NEQ,"==",ls,lv4());
                    break;
                default:
                    return ls;  
            }
        }
    }

    AST* lv6(){
        AST *ls=lv5();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case '^':
                    match('^');
                    ls=new BinaryOpt(pos,'^',"^",ls,lv5());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST *lv7(){
        AST *ls=lv6();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case L_AND:
                    match(L_AND);
                    ls=new BinaryOpt(pos,L_AND,"&&",ls,lv6());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST *lv8(){
        AST *ls=lv7();
        while(true){
            Position pos=lookahead().pos;
            switch(lookahead().type){
                case L_OR:
                    match(L_OR);
                    ls=new BinaryOpt(pos,L_OR,"||",ls,lv7());
                    break;
                default:
                    return ls;
            }
        }
    }

    AST *lv9(){
        AST *rs=lv8();Position pos=lookahead().pos;
        while(true){
            switch (lookahead().type){
                case '=':
                    match('=');
                    rs=new BinaryOpt(pos,'=',"=",rs,lv8());
                    break;
                default:
                    return rs;
            }
        }
    }

    void calc(vector<AST*> &exprs){
        Position pos=lookahead().pos;
        switch(lookahead().type){
		case ')':
			break;
			case IDENT:
            case NUMBER:
            case CIN:
            case COUT:
            case '(':
            case '+':
            case '!':
            case '-':
				exprs.push_back(exprdef());
			switch(lookahead().type){
			case ',':
				match(',');
				break;
			case ')':
				break;
			default:
				report.issue(SyntexError(pos));
			}
			calc(exprs);
			break;
		default:
			report.issue(SyntexError(pos));
		}
    }


    parser(){pre=0;}
    ~parser(){if(program!=nullptr)delete(program);}

    int main(){
        lexer.begin();
        token cur;
        do{
            cur=lexer.readtoken();
            tokens.push_back(cur);
            cur.output(); 
        }
        while(cur.type!=EXIT);
        if(lexer.iserror())exit(0);
        program=head();
        program->visit(typevisitor);
        if(typevisitor.iserror()){
            cerr<<"error"<<endl;
            exit(0);
        }
        program->visit(runvisitor);
    }


}parser_;




int main(){
    lexer.init();
    freopen("source.cpp","r",stdin);;
    lexer.begin();
    parser_.main();
    return 0;
}
