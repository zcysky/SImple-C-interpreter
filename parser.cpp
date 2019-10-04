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
        if(type=="array")for(int i=0;i<indexs.size();i++)cout<<name<<"["<<index[i]<<"] ";
    }

    

};

