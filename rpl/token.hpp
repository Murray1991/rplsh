///////////////////////////////////////////////////////////////////////////////
//
//  Token specification and useful classes
//
///////////////////////////////////////////////////////////////////////////////
#ifndef token_hpp
#define token_hpp

#include <string>
#include <map>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  This class represents a token.
//
///////////////////////////////////////////////////////////////////////////////
struct token
{
    typedef enum
    {
        open = 1, close, comma, equals, show, set, annotate, rewrite, optimize, seq,
        comp, pipe, farm, map, reduce, integer, number, word, with, by, illegal, parameter,
        eol,
    } type;

    token(token::type kind, string data, int pos = -1);
    static std::map<std::string, token::type> st_map;
    static std::map<token::type, std::string> ts_map;

    token::type kind;
    std::string data;
    int pos;
};

#endif
