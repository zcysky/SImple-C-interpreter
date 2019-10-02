#include "debugger.cpp"
#include "header.cpp"

class token{
    public:
    Position pos;//追踪位置，用于报错
    int type,number;
    string ident;

    token(){}
    token(Position pos,int type,const string &ident){
        this->pos=pos;this->type=type;this->ident=s;
    }
    token(Position pos,int type){
        this->pos=pos;this->type=type;
    }
    token(Position pos,int type,int number){
        this->pos=pos;this->type=type;this->number=number;
    }
    inline void output(){
        if(type=ERROR)cout<<"ERROR"<<endl;
        if(type==IDENT)cout<<"Ident:"<<ident<<end;
        if(type==NUMBER)cout<<"Number:"<<number<<endl;
        if(type==KEYWORD)cout<<"Keyword:"<<ident<<endl;
    }
};


class lexer{
    map<string,int> table;
    map<int,string> keys;
    //上面的存储一个双射，分别表示标识符/关键字被映射的编号
    
};