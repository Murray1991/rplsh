///////////////////////////////////////////////////////////////////////////////
//
//          Main program
//
///////////////////////////////////////////////////////////////////////////////
#include "parser.hpp"
#include "interpreter.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <locale>

using namespace std;

typedef environment<string, skel_node> env_t;
typedef dispatcher<string, skel_visitor> disp_t;
typedef interpreter<env_t, disp_t> interpr_t;
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
    string line;    // input line
    env_t env;      // environment: <name, skel_tree> bindings
    disp_t disp;    // dispatcher: <name, skel_visitor> bindings

    // TODO don't use costant strings but invent some container class...
    disp.add("show_default", make_shared<printer>());
    disp.add("servicetime", make_shared<servicetime<env_t>>(env));
    disp.add("latency", make_shared<latencytime<env_t>>(env));
    while ( print_rpl() && getline(cin, line) )
    {
        if (is_empty_input(line))
            continue;
        if (is_quit_input(line))
            break;

        error_container err_repo;                   // filled with errors
        interpr_t _interpr(env, disp, err_repo);
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
