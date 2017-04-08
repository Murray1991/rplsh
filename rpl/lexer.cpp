#include "lexer.hpp"

using namespace std;

lexer::lexer (const string& line, error_container& err_repo)
    : prev(0), pos(0), line(line), err_repo(err_repo), end(false) {}

bool lexer::has_next() const {
    return !end;
}

const string& lexer::getline() const {
    return line;
}

token get_token(const string& word, int wpos, error_container& err_repo)
{
    token::type kind = token::st_map[word];

    if (kind > 0)                                                   // keyword
        return token(kind, word, wpos);
    else if (regex_match(word, regex("[a-zA-z][a-zA-Z_0-9]*")))     // word
        return token(token::word, word, wpos);
    else if (regex_match(word, regex("[1-9][0-9]*[.][0-9]*")))      // number
        return token(token::number, word, wpos);
    else if (regex_match(word, regex("[1-9][0-9]*")))               // integer
        return token(token::integer, word, wpos);

    // nothing found
    err_repo.add( shared_ptr<error>(new error_illegal(word, wpos)));
    return token(token::illegal, word, wpos);                      // illegal
}

token lexer::next() {
    string word;

    while ((pos = line.find_first_of(" ,()=", prev)) != string::npos ) {
        word = line.substr(prev, pos - prev + (pos == prev)*1 );
        int wpos = prev; prev = pos + (pos == prev)*1;
        if (!word.empty() && !isspace(word[0]))
            return get_token(word, wpos, err_repo);
    }

    // the last word
    if (pos == string::npos && prev < line.length()) {
        int wpos = prev; prev = string::npos;
        return get_token(line.substr(wpos, string::npos), wpos, err_repo);
    }

    // end has been reached and case of trailing space
    end = true;
    return token(token::eol, "", line.length());
}
