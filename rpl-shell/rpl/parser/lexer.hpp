///////////////////////////////////////////////////////////////////////////////
//
//  Lexical scanner of the rpl shell language.
//  Class lexer performs the lexical analysis
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_lexer_hpp
#define rpl_lexer_hpp

#include "error/error_report.hpp"
#include "token.hpp"
#include <iostream>
#include <string>
#include <regex>

///////////////////////////////////////////////////////////////////////////////
//
//  This class performs lexical analysis over an input string.
//  It generates a stream of tokens from a sequence of characters
//
///////////////////////////////////////////////////////////////////////////////
struct lexer
{
    lexer(const std::string& line, error_container& err_repo);
    const std::string& getline() const;
    bool has_next() const;
    token next();

private:
    std::size_t prev, pos;
    const std::string& line;
    error_container& err_repo;
    bool end;
};

#endif
