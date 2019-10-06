#include "header.cpp"
#include "debugger.cpp"
#include "reporterror.cpp"
#include "lexer.cpp"
#include "parser.cpp"

int main(){
    lexer.init();
    freopen("source.cpp");
    lexer.begin();
    parser_.main();
    return 0;
}