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
//  Enumeration type token::type_t range values from 1 to 14
//
///////////////////////////////////////////////////////////////////////////////
struct token
{
    typedef enum
    {
        open = 1,
        close,
        comma,
        seq,
        comp,
        pipe,
        farm,
        map,
        reduce,
        integer,
        number,
        identifier,
        illegal,
        eol
    } type_t;

    token(type_t type, string word, int pos = -1)
        : type(type), word(word), pos(pos) {}

    type_t type;
    string word;
    int pos;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Utils class that contains st_map and ts_map
//
///////////////////////////////////////////////////////////////////////////////
struct utils
{
    static map<string, token::type_t> st_map;
    static map<token::type_t, string> ts_map;
};

map<string, token::type_t> utils::st_map =
{
   {"(",        token::open},
   {")",        token::close},
   {",",        token::comma},
   {"seq",      token::seq},
   {"Seq",      token::seq},
   {"comp",     token::comp},
   {"Comp",     token::comp},
   {"pipe",     token::pipe},
   {"Pipe",     token::pipe},
   {"farm",     token::farm},
   {"Farm",     token::farm},
   {"map",      token::map},
   {"Map",      token::map},
   {"reduce",   token::reduce},
   {"Reduce",   token::reduce}
};

map<token::type_t, string> utils::ts_map =
{
   {token::open,    "("},
   {token::close,   ")"},
   {token::comma,   ","},
   {token::seq,     "seq"},
   {token::comp,    "comp"},
   {token::pipe,    "pipe"},
   {token::farm,    "farm"},
   {token::map,     "map"},
   {token::reduce,  "reduce"}
};

#endif
