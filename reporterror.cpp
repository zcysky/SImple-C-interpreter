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
