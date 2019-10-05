#include "debugger.cpp"
#include "header.cpp"
#include "reporterror.cpp"
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
class ArrayOpt;
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
    virtual void VisitArrayOpt(ArrayOpt *that){}
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
    virtual void print(int tab){assert(false);}



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
    inline void print(int tab){
        for(int i=1;i<=tab;i++)putchar(' ');
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
    virtual void print()=0;

};

class BinaryOpt:public Expr{

    public:
    string ident;
    int type;
    AST *ls,*rs;
    BinaryOpt(const Position &pos,const string &ident,const int type,AST *ls,AST *rs){
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
    ArrayOpt(const Position &pos,const string &name,const vector<AST*> at){
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
        assert(!table.count(s));
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
            that->size+=that->vars[i]->size;
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

}typevisitor;
