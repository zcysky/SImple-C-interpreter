#include "debugger.cpp"
#include "header.cpp"
#include "lexer.cpp"

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
class Arrayopt;
class Exec;
class Expr;


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
    virtual void VisitArrayOpt(Arrayopt *that){}
    virtual void VisitExec(Exec *that){}

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
    virtual void print(){assert(false);}



};

class Var{

    inline bool checkmul(int a,int b){return 1LL*a*b<=2147483647;}
    inline int checkindex(const vector<int> &indexs){
        int size=1;
        for(int i=0;i<indexs.size();i++){
            int v=indexs[i];
            if(v<0||!checkmul(size,v)){/*报错：数组下标越界*/}
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
    inline void print(){
        cout<<type<<" "<<name<<endl;
        if(type=="array")for(int i=0;i<indexs.size();i++)cout<<name<<"["<<indexs[i]<<"] ";
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
    virtual void print(){
        printf("Vardefinition: ");assert(vars.size()>0);
        for(int i=0;i<vars.size();i++){
            vars[i].print();
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
    virtual void print(){
        printf("FuncDefinition: ");
        cout<<name<<endl;
        if(argv.size()==0)puts("<empty argvs>");
        else for(int i=0;i<argv.size();i++){
            argv[i].print();
            if(i==argv.size()-1)putchar('\n');
            else putchar(' ');
        }
        if(stmt==nullptr)puts("<empty stmts>");
        else stmt->print();
    }

};


class Block:public AST{

    vector<AST*> levels;
    Block(const Position &pos,const vector<AST*> levels){
        setpos(pos);
        this->levels=levels;
    }
    virtual ~Block(){}
    virtual void visit(visitor &vis){vis.VisitBlock(this);}
    virtual void print(){
        puts("Block: ");
        for(int i=0;i<levels.size();i++)levels[i]->print();
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
    virtual void print(){
        puts("TopLevel");
        for(int i=0;i<levels.size();i++)levels[i]->print();
    }

};

class Expr:public AST{

    virtual void visit(visitor &vis)=0;
    virtual void print()=0;

};

