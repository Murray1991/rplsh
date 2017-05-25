///////////////////////////////////////////////////////////////////////////////
//
//  Token class
//
///////////////////////////////////////////////////////////////////////////////
#ifndef token_hpp
#define token_hpp

#include <string>
#include <map>

struct token
{
    typedef enum
    {
        open = 1,
        close,
        comma,
        equals,
        show,
        set,
        annotate,
        rewrite,
        optimize,
        seq,
        comp,
        pipe,
        farm,
        map,
        reduce,
        integer,
        number,
        word,
        with,
        by,
        illegal,
        parameter,
        plus,
        minus,
        eol

    } type;

    /* constructor for a token object */
    token(token::type kind, std::string data, int pos = -1);

    /* static map containing all the <string, tokentype> bindings */
    static std::map<std::string, token::type> st_map;

    /* static map containing some of the <tokentype, string> bindings */
    static std::map<token::type, std::string> ts_map;

    /* public fields of a token obj */
    token::type kind;   // type of the token    (e.g. integer)
    std::string data;   // satellite data       (e.g. "1024")
    int pos;            // position in the text
};

#endif