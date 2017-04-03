///////////////////////////////////////////////////////////////////////////////
//
//  Lexical scanner of the rpl shell language.
//  Class lexer performs the lexical analysis
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_lexer_hpp
#define rpl_lexer_hpp

#include "utils/error_report.hpp"
#include "token.hpp"
#include <iostream>
#include <string>
#include <regex>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  This class performs lexical analysis over an input string.
//  It generates a stream of tokens from a sequence of characters
//
///////////////////////////////////////////////////////////////////////////////
struct lexer
{
    lexer(const string& line, error_container& err_repo)
        : prev(0), pos(0), line(line), err_repo(err_repo), end(false) {}

    token next();

    bool has_next() {
        return !end;
    }

    const string& get_line() {
        return line;
    }

private:
    size_t prev, pos;
    const string& line;
    error_container& err_repo;
    bool end;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of lexer::next() method,
//  it produces a token from the input string
//
///////////////////////////////////////////////////////////////////////////////
token lexer::next()
{
    string word;
    int word_pos;

    while ((pos = line.find_first_of(" ,()", prev)) != string::npos )
    {
        if (pos > prev)
        {
            word = line.substr(prev, pos-prev);
            word_pos = prev;
            prev = pos;
        }
        else if (pos == prev)
        {
            word = line.substr(pos, 1);
            word_pos = pos;
            prev = pos+1;
        }

        if (!word.empty() && !isspace(word[0]))
            break;
    }

    // possibly the last word after the delimeter
    if (pos == string::npos && prev < line.length())
    {
        word = line.substr(prev, string::npos);
        word_pos = prev;
        prev = line.length();
    }

    // end has been reached and case of trailing space
    if (word.empty() || isspace(word[0]))
    {
        end = true;
        return token(token::eol, "", line.length());
    }

    // token is a keyword or comma/parenthesis
    token::type_t type = utils::st_map[word];
    if (type > 0)
        return token(type, word, word_pos);

    // check the word, distinguishing numbers from ids
    if (regex_match(word, regex("[a-zA-z][a-zA-Z_0-9]*")))
        return token(token::identifier, word, word_pos);
    else if (regex_match(word, regex("[1-9][0-9]*[.][0-9]*")))
        return token(token::number, word, word_pos);
    else if (regex_match(word, regex("[1-9][0-9]*")))
        return token(token::integer, word, word_pos);

    // returning illegal token
    token tok (token::illegal, word, word_pos);
    err_repo.add( unique_ptr<error>(new error_illegal(tok.word, tok.pos)) );

    return tok;
}

#endif
