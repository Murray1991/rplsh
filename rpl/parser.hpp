///////////////////////////////////////////////////////////////////////////////
//
//  Parser of the rpl shell language.
//  Class parser performs the syntax analysis
//  and generate an intermediate AST for the
//  pattern expressions (our skeleton tree)
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_parser_hpp
#define rpl_parser_hpp

#include "lexer.hpp"
#include "tree.hpp"
#include <ostream>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  "Forward" declarations of the parsing rules
//
///////////////////////////////////////////////////////////////////////////////
struct patt_exp_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
    static unique_ptr<skel_tree> parse(lexer& scanner, error_container& err_repo);
};

struct seq_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
};

struct comp_pipe_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
};

struct farm_map_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
};

struct reduce_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
};

struct identifier_rule
{
    static unique_ptr<skel_tree> parse(token& tok, lexer& scanner, error_container& err_repo);
};

///////////////////////////////////////////////////////////////////////////////
//
//  This class generate an rpl tree (intermediate ast) by parsing
//  a parallel pattern expression. It implements a simple recursive
//  descent parser by calling the parsing rules
//
///////////////////////////////////////////////////////////////////////////////
struct parser
{
    parser(lexer& scanner, error_container& err_repo)
        : scanner(scanner), err_repo(err_repo) {}

    unique_ptr<skel_tree> parse()
    {
        error::line = scanner.get_line();
        token tok = scanner.next();

        unique_ptr<skel_tree> tree = patt_exp_rule::parse(tok, scanner, err_repo);
        if (scanner.has_next() && err_repo.size() > 0)
            return nullptr;

        return tree;
    }

private:
    lexer& scanner;
    error_container& err_repo;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Useful macros in parsing (1) check if the token tok has the expected output
//  and advance the scanner, otherwise print error message and return
//  a nullptr (2) check if pattexp is a nullptr
//
///////////////////////////////////////////////////////////////////////////////
bool expect(token& tok, token::type_t expected, lexer& scanner, error_container& err_repo)
{
    if (tok.type == expected) {
        tok = scanner.next();
        return true;
    } else {
        err_repo.add( unique_ptr<error>(new error_unexp(tok.word, tok.pos)) );
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Patt exp parse rules, the fragment of the grammar that hey parse are
//  <pattexp> ::= ( token::seq <seq> )
//                | ( token::comp <comp_pipe> )
//                | ( token::pipe <comp_pipe> )
//                | ( token::farm <farm_map> )
//                | ( token::map <farm_map> )
//                | ( token::reduce <reduce> )
//                | token::identifier
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> patt_exp_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    switch(tok.type)
    {
        case token::seq:
            return seq_rule::parse(tok, scanner, err_repo);
        case token::comp:
            return comp_pipe_rule::parse(tok, scanner, err_repo);
        case token::pipe:
            return comp_pipe_rule::parse(tok, scanner, err_repo);
        case token::farm:
            return farm_map_rule::parse(tok, scanner, err_repo);
        case token::map:
            return farm_map_rule::parse(tok, scanner, err_repo);
        case token::reduce:
            return reduce_rule::parse(tok, scanner, err_repo);
        case token::identifier:
            return identifier_rule::parse(tok, scanner, err_repo);
        default:
            err_repo.add( unique_ptr<error>(new error_unexp(tok.word, tok.pos)) );
            return nullptr;
    }
}

unique_ptr<skel_tree> patt_exp_rule::parse(lexer& scanner, error_container& err_repo)
{
    token tok = scanner.next();
    return parse(tok, scanner, err_repo);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Seq parse rule: the fragment of the grammar that it parses is
//  <seq> ::= token::open token::identifier [token::comma token::number] token::close
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> seq_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    unique_ptr<skel_tree> pattexp;
    token::type_t type = tok.type;
    tok = scanner.next();

    expect(tok, token::open, scanner, err_repo);

    pattexp = patt_exp_rule::parse(tok, scanner, err_repo);
    if (pattexp == nullptr)
        return nullptr;

    bool comma = false;
    if ((comma =  tok.type == token::comma))
        tok = scanner.next();
    if (comma && tok.type == token::integer)
        expect(tok, token::integer, scanner, err_repo);
    else if (comma)
        expect(tok, token::number, scanner, err_repo);

    expect(tok, token::close, scanner, err_repo);

    return unique_ptr<skel_tree>(new skel_tree(type, move(pattexp)));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Comp-Pipe parse rule: the fragment of the grammar that it parses is
//  <comp_pipe> ::= token::open <pattexp> (token::comma <pattexp>)+ token::close
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> comp_pipe_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    unique_ptr<skel_tree> pattexp;
    token::type_t type = tok.type;
    tok = scanner.next();

    expect(tok, token::open, scanner, err_repo);

    pattexp = patt_exp_rule::parse(tok, scanner, err_repo);
    if (pattexp == nullptr)
        return nullptr;

    unique_ptr<skel_tree> comp_pipe( new skel_tree(type) );
    comp_pipe->add(move(pattexp));

    while (tok.type == token::comma)
    {
        tok = scanner.next();
        pattexp = patt_exp_rule::parse(tok, scanner, err_repo);
        if (pattexp == nullptr)
            return nullptr;
        comp_pipe->add(move(pattexp));
    }

    expect(tok, token::close, scanner, err_repo);

    //TODO should be ok for move semantics
    return comp_pipe;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Farm-Map parse rule, the fragment of the grammar that it parses is
//  <comp> ::= token::open <pattexp> [token::comma token::integer] token::close
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> farm_map_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    unique_ptr<skel_tree> pattexp;
    token::type_t type = tok.type;
    tok = scanner.next();

    expect(tok, token::open, scanner, err_repo);

    pattexp = patt_exp_rule::parse(tok, scanner, err_repo);
    if (pattexp == nullptr)
        return nullptr;

    expect(tok, token::comma, scanner, err_repo);

    if (tok.type == token::integer)
        tok = scanner.next();

    expect(tok, token::close, scanner, err_repo);

    return unique_ptr<skel_tree>(new skel_tree(type, move(pattexp)));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Reduce parse rule, the fragment of the grammar that it parses is
//  <comp> ::= token::open <pattexp> token::close
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> reduce_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    unique_ptr<skel_tree> pattexp;
    token::type_t type = tok.type;
    tok = scanner.next();

    expect(tok, token::open, scanner, err_repo);

    pattexp = patt_exp_rule::parse(tok, scanner, err_repo);
    if (pattexp == nullptr)
        return nullptr;

    expect(tok, token::close, scanner, err_repo);

    return unique_ptr<skel_tree>(new skel_tree(type, move(pattexp)));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Identifier parse rule, it just eat the identifier token
//
///////////////////////////////////////////////////////////////////////////////
unique_ptr<skel_tree> identifier_rule::parse(token& tok, lexer& scanner, error_container& err_repo)
{
    string word = tok.word;
    tok = scanner.next();

    return unique_ptr<skel_tree>(new skel_tree(token::identifier, tok.word));
}

#endif
