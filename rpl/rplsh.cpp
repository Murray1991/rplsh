///////////////////////////////////////////////////////////////////////////////
//
//          Main program
//
///////////////////////////////////////////////////////////////////////////////
#include "parser.hpp"
#include "interpreter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <locale>

using namespace std;

int main()
{
    string line;
    environment env;

    while (true)
    {
        cout << rplsh::name;
        getline(cin, line);

        if (line.empty() || line == string(line.length(), ' '))
            continue;

        //TODO command quit and command help
        if (line.length() == 1 && ( line[0] == 'q' || line[0] == 'Q' ))
            break;

        error_container err_repo;
        lexer _lexer(line, err_repo);
        parser _parser(_lexer, err_repo);
        interpreter _interpr(env, err_repo);

        auto t = _parser.parse();

        if (err_repo.size() == 0)
            t->accept(_interpr);
        else
            cout << err_repo.get(0);

    }

    return 0;
}
