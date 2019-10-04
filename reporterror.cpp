#include "debugger.cpp"
#include<bits/stdc++.h>
using namespace std;

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