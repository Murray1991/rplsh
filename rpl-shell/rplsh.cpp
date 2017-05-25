///////////////////////////////////////////////////////////////////////////////
//
//          Main program
//
///////////////////////////////////////////////////////////////////////////////
#include "rpl/interpreter.hpp"
#include "parser/lexer.hpp"
#include "parser/parser.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <locale>

using namespace std;

typedef interpreter interpr_t;
typedef lexer scanner_t;
typedef parser parser_t;

bool print_rpl() {
    cout << rplsh::name;
    return true;
}

bool is_empty_input(string& line) {
    return line.empty() || line == string(line.length(), ' ');
}

bool is_quit_input(string& line) {
    return line.length() == 1 && ( line[0] == 'q' || line[0] == 'Q' );
}

int main(int argc, char * argv[])
{
    string line;              // input line
    rpl_environment env;      // environment: <name, skel_tree> bindings

    while ( print_rpl() && getline(cin, line) )
    {
        if (is_empty_input(line))
            continue;
        if (is_quit_input(line))
            break;

        error_container err_repo;                   // filled with errors
        interpr_t _interpr(env, err_repo);
        scanner_t _scanner(line, err_repo);
        parser_t _parser(_scanner, err_repo);

        shared_ptr<rpl_node> t = _parser.parse();
        if (err_repo.size() == 0)
            t->accept(_interpr);
        if (err_repo.size() > 0)
            cout << err_repo.get(0);
    }

    return 0;
}
