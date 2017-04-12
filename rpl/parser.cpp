#include "parser.hpp"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of parser class
//
///////////////////////////////////////////////////////////////////////////////
parser::parser(lexer& scanner, error_container& err_repo)
    : scanner(scanner), err_repo(err_repo) {
        error::line = scanner.getline();
}

shared_ptr<rpl_node> parser::parse()
{
    token tok = scanner.next();
    shared_ptr<rpl_node> tree = start_rule(tok);
    if (scanner.has_next() && err_repo.size() > 0)
        return nullptr;
    return tree;
}

//  Useful functions in parsing: check if the token tok has the expected output
//  and advance the scanner returning true, otherwise add error message to the
//  error_repo and return false
bool parser::expect(token& tok, token::type exp)
{
    if (tok.kind == exp) {
        tok = scanner.next();
        return true;
    } else {
        err_repo.add( make_shared<error_unexp>(tok.data, tok.pos) );
        return false;
    }
}

bool parser::expect(token& tok, token::type exp, string& data)
{
    data = tok.data;
    return expect(tok, exp);
}

bool parser::expect(token& tok, token::type exp, int& data)
{
    data = exp == token::integer ? stoi(tok.data) : 0;
    return expect(tok, exp);
}

bool parser::expect(token& tok, token::type exp, double& data)
{
    data = exp == token::integer ? stod(tok.data) : 0;
    return expect(tok, exp);
}

bool parser::expect(token& tok, token::type exp1, token::type exp2)
{
    return tok.kind == exp2 ? expect(tok, exp2) : expect(tok, exp1);
}

///////////////////////////////////////////////////////////////////////////////
//
//  The parsing rules actually implementing the recursive descent parser
//  of the shell language
//
///////////////////////////////////////////////////////////////////////////////
shared_ptr<rpl_node> parser::start_rule(token& tok)
{
    switch (tok.kind) {
        case token::word:
            return assign_rule(tok);
        case token::show:
            return show_rule(tok);
        case token::set:
            return set_rule(tok);
        case token::annotate:
            return ann_rule(tok);
        case token::rewrite:
            return rwr_rule(tok);
        case token::optimize:
            return  opt_rule(tok);
        default:
            err_repo.add( make_shared<error_unexp>(tok.data, tok.pos) );
            return nullptr;
    }
}

//  <assign> ::= <word> = <pattexp>
//  TODO add possibility to put rewriting/opt rules as functions in the right part
shared_ptr<rpl_node> parser::assign_rule(token& tok)
{
    string word = tok.data;
    shared_ptr<skel_node> pattexp;
    expect(tok, token::word);
    expect(tok, token::equals);
    pattexp = pattexp_rule(tok);
    expect(tok, token::eol);
    return make_shared<assign_node>(word, pattexp);
}

//  <show> ::= show <word> [<parameter>]
//  <parameter> ::= servicetime | latency | ...
shared_ptr<rpl_node> parser::show_rule(token& tok)
{
    string id, par = "show_default";
    expect(tok, token::show);
    expect(tok, token::word, id);
    if (tok.kind == token::parameter) {                     // optional
        expect(tok, token::parameter, par);
    }
    expect(tok, token::eol);
    return make_shared<show_node>(id, par);
}

//  <set> ::= set <word> with <pattern> <integer>
//  <pattern> ::= farm | pipe | ...
shared_ptr<rpl_node> parser::set_rule(token& tok)
{
    string id, par; int value;
    expect(tok, token::set);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::word, par);                          // TODO this parameter should be a keyword
    expect(tok, token::integer, value);
    return make_shared<set_node>(id, par, value);
}

//  <ann> ::= set <word> with <parameter> <value>
shared_ptr<rpl_node> parser::ann_rule(token& tok)
{
    string id, par; double value;
    expect(tok, token::annotate);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::parameter, par);
    expect(tok, token::number, value);
    return make_shared<ann_node>(id, par, value);
}

// <rwr> ::= rewrite <word> with <rwr-rule>
// <rwr-rule> ::= farmelim | farmfarmelim | farmintro | ...
shared_ptr<rpl_node> parser::rwr_rule(token& tok)
{
    string id, par;
    expect(tok, token::annotate);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::word, par);                      //TODO rewrite rule token here...
    return make_shared<rwr_node>(id, par);
}

// <opt> ::= optimize <word> with <opt-rule>
// <opt-rule> ::= pipeopt | farmopt | ...
shared_ptr<rpl_node> parser::opt_rule(token& tok)
{
    string id, par;
    expect(tok, token::annotate);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::word, par);                      //TODO opt rule token here
    return make_shared<rwr_node>(id, par);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Pattern expression rules
//
///////////////////////////////////////////////////////////////////////////////
shared_ptr<skel_node> parser::pattexp_rule(token& tok)
{
    switch(tok.kind)
    {
        case token::seq:
            return seq_rule(tok);
        case token::comp:
            return comp_pipe_rule(tok);
        case token::pipe:
            return comp_pipe_rule(tok);
        case token::farm:
            return farm_map_rule(tok);
        case token::map:
            return farm_map_rule(tok);
        case token::reduce:
            return reduce_rule(tok);
        case token::word:
            return id_rule(tok);
        default:
            err_repo.add( make_shared<error_unexp>(tok.data, tok.pos) );
            return nullptr;
    }
}


//  <seq> ::= 'seq' '(' <word> [ ',' <number> ] ')'
//  TODO maybe also <word> should be optional -> possibility to do "a = seq()"
shared_ptr<skel_node> parser::seq_rule(token& tok)
{
    shared_ptr<skel_node> pattexp;
    double ts = -1;
    expect(tok, token::seq);
    expect(tok, token::open);
    // cases without pattern expressions
    if ( tok.kind == token::close ) {                                   //e.g seq()
        expect(tok, token::close);
        return make_shared<seq_node>(ts);
    }
    if (tok.kind == token::integer || tok.kind == token::number) {      //e.g seq(5)
        ts = stod(tok.data);
        expect(tok, token::integer, token::number);
        expect(tok, token::close);
        return make_shared<seq_node>(ts);
    }
    // cases with pattern expressions
    pattexp = pattexp_rule(tok);
    if ( tok.kind == token::comma ) {
        expect(tok, token::comma);                      //success for sure
        if ( tok.kind == token::integer || tok.kind == token::number)
            ts = stod(tok.data);
        expect(tok, token::integer, token::number);
    }
    expect(tok, token::close);
    return make_shared<seq_node>(pattexp, ts);
}


//  <comp_pipe> ::= ('comp' | 'pipe') '(' <pattexp> (',' <pattexp>)+ ')'
shared_ptr<skel_node> parser::comp_pipe_rule(token& tok)
{
    shared_ptr<skel_node> pattexp;
    token::type kind = tok.kind;
    expect(tok, token::comp, token::pipe);
    expect(tok, token::open);
    pattexp = pattexp_rule(tok);

    // distinguish comp and pipe
    shared_ptr<access_node> comp_pipe = (kind == token::comp)?
        static_pointer_cast<access_node>(make_shared<comp_node>()):
        static_pointer_cast<access_node>(make_shared<pipe_node>());

    // add the pattexps to the comp_pipe
    comp_pipe->push(pattexp);
    while (tok.kind == token::comma) {
        tok = scanner.next();
        pattexp = pattexp_rule(tok);
        comp_pipe->push(pattexp);
    }

    expect(tok, token::close);
    return comp_pipe;
}

//  <farm_map> ::= ('farm' | 'map') '(' <pattexp> [',' <integer>] ')'
shared_ptr<skel_node> parser::farm_map_rule(token& tok)
{
    shared_ptr<skel_node> pattexp;
    token::type kind = tok.kind;
    int nw           = 1;                   // default number of workers

    expect(tok, token::farm, token::map);
    expect(tok, token::open);
    pattexp = pattexp_rule(tok);
    if (tok.kind == token::comma) {
        expect(tok, token::comma);
        expect(tok, token::integer, nw);
    }
    expect(tok, token::close);

    if (kind == token::farm)
        return make_shared<farm_node>(pattexp, nw);
    return make_shared<map_node>(pattexp, nw);
}

//  <comp> ::= token::open <pattexp> token::close
//
///////////////////////////////////////////////////////////////////////////////
shared_ptr<skel_node> parser::reduce_rule(token& tok)
{
    shared_ptr<skel_node> pattexp;
    expect(tok, token::reduce);
    expect(tok, token::open);
    pattexp = pattexp_rule(tok);
    expect(tok, token::close);
    return make_shared<reduce_node>(pattexp);
}

//  Identifier parse rule, it just eat the id token
shared_ptr<skel_node> parser::id_rule(token& tok)
{
    string word = tok.data;
    expect(tok, token::word);
    return make_shared<id_node>(word);
}
