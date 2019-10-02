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
    Position pos;
    char ch;
    int ident,number,error;

    //本部分函数调用频率大，使用inline加速调用效率
    inline void readchar(){
        ch=getchar();
        if(ch=='\n'){pos.line++;pos.col=0;}
        else if(ch=='\r'){pos.col=0;}
        else pos.col++;
    }
    inline bool isblank(){
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
        return token(pos,NUMBER,val);
    }
    inline token readident(){

    }

};