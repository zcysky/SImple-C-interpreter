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
