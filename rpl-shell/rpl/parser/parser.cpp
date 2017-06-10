#include "parser.hpp"

using namespace std;

#include <limits>

///////////////////////////////////////////////////////////////////////////////
//  Implementation of parser class
///////////////////////////////////////////////////////////////////////////////
parser::parser(lexer& scanner, error_container& err_repo)
    : scanner(scanner), err_repo(err_repo) {
        error::line = scanner.getline();
}

unique_ptr<rpl_node> parser::parse()
{
    token tok = scanner.next();
    rpl_node* tree = start_rule(tok);
    if (scanner.has_next() && err_repo.size() > 0)
        return nullptr;
    return unique_ptr<rpl_node>(tree);
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

bool parser::expect(token& tok, token::type exp, pair<string, int>& el) {
    string w = "";
    int s = -1;

    bool rtv = expect(tok, exp, w);
    if ( rtv && tok.kind == token::open_square ) {
        rtv = rtv && expect(tok, token::open_square);
        rtv = rtv && expect(tok, token::integer, s);
        rtv = rtv && expect(tok, token::close_square);
    }

    el.first = w;
    el.second = s;
    return rtv;
}

bool parser::expect(token& tok, token::type exp, int& data)
{
    data = exp == tok.kind ? stoi(tok.data) : 0;
    return expect(tok, exp);
}

bool parser::expect(token& tok, token::type exp, double& data)
{
    data = (exp == tok.kind || exp == tok.kind) ? stod(tok.data) : 0;
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
rpl_node* parser::start_rule(token& tok)
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
            return opt_rule(tok);
        case token::history:
            return history_rule(tok);
        default:
            err_repo.add( make_shared<error_unexp>(tok.data, tok.pos) );
            return nullptr;
    }
}

//  TODO add possibility to put rewriting/opt rules as functions in the right part
rpl_node* parser::assign_rule(token& tok)
{
    string word = tok.data;
    skel_node* pattexp;
    expect(tok, token::word);
    expect(tok, token::equals);
    pattexp = pattexp_rule(tok);
    expect(tok, token::eol);
    if (err_repo.size() > 0) {
        delete pattexp;
        pattexp = nullptr;
    }
    return new assign_node(word, pattexp);
}

//  <show> ::= show <word> [<parameter>]
//  <parameter> ::= servicetime | latency | ...
rpl_node* parser::show_rule(token& tok)
{
    int num = numeric_limits<int>::max();
    string par = "show_default";
    pair<string,int> ident;
    vector<string> parameters;

    expect(tok, token::show);

    if ( tok.kind == token::parameter ) {
        expect(tok, token::parameter, par);
        expect(tok, token::eol);
        parameters.push_back(par);
        return new show_node("", 0, parameters);
    }

    expect(tok, token::word, ident);
    if (tok.kind == token::by) {
        expect(tok, token::by);
        expect(tok, token::parameter, par);
        while ( tok.kind == token::comma ) {
            parameters.push_back(par);
            expect(tok, token::comma);
            expect(tok, token::parameter, par);
        }
        parameters.push_back(par);
    }
    else if (tok.kind == token::parameter) {                     // optional
        expect(tok, token::parameter, par);
        parameters.push_back(par);
    }

    /* always put the default */
    parameters.push_back("show_default");

    if ( tok.kind == token::plus || tok.kind == token::minus ) {
        auto type = tok.kind;
        expect( tok, type );
        expect(tok, token::integer, num);
        num = ( type == token::minus )? -num : num;
    }

    expect(tok, token::eol);
    return new show_node(ident, num, parameters);
}

//  <set> ::= set <word> with <pattern> <integer>
//  <pattern> ::= farm | pipe | ...
rpl_node* parser::set_rule(token& tok)
{
    string par; double value;
    expect(tok, token::set);
    expect(tok, token::parameter, par);
    expect(tok, token::with);
    if ( tok.kind == token::integer )
        expect(tok, token::integer, value);
    else
        expect(tok, token::number, value);
    return new set_node(par, par, value);
}

//  <ann> ::= set <word> with <parameter> <value>
rpl_node* parser::ann_rule(token& tok)
{
    string par; double value;
    pair<string,int> id;
    expect(tok, token::annotate);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::parameter, par);
    if ( tok.kind == token::integer )
        expect(tok, token::integer, value);
    else
        expect(tok, token::number, value);
    return new ann_node(id, par, value);
}

// <rwr> ::= rewrite <word> with <rwr-rule-list>...
rpl_node* parser::rwr_rule(token& tok)
{
    string par;
    pair<string,int> id;
    vector<string> parameters;

    expect(tok, token::rewrite);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::word, par);
    while ( tok.kind == token::comma ) {
        parameters.push_back(par);
        expect(tok, token::comma);
        expect(tok, token::word, par);
    }
    parameters.push_back(par);

    expect(tok, token::eol);
    return new rwr_node(id, parameters);
}

// <opt> ::= optimize <word> with <opt-rule-list>
rpl_node* parser::opt_rule(token& tok)
{
    string par;
    pair<string,int> id;
    vector<string> parameters;

    expect(tok, token::optimize);
    expect(tok, token::word, id);
    expect(tok, token::with);
    expect(tok, token::word, par);
    while ( tok.kind == token::comma ) {
        parameters.push_back(par);
        expect(tok, token::comma);
        expect(tok, token::word, par);
    }
    parameters.push_back(par);

    expect(tok, token::eol);
    return new opt_node(id, parameters);
}

rpl_node* parser::history_rule(token& tok)
{
    pair<string,int> id;
    expect(tok, token::history);
    if (tok.kind == token::word)
        expect(tok, token::word, id);
    expect(tok, token::eol);
    return new history_node(id.first);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Pattern expression rules
//
///////////////////////////////////////////////////////////////////////////////
skel_node* parser::pattexp_rule(token& tok)
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
skel_node* parser::seq_rule(token& tok)
{
    skel_node* pattexp;
    double ts = -1;
    expect(tok, token::seq);
    expect(tok, token::open);
    // cases without pattern expressions
    if ( tok.kind == token::close ) {                                   //e.g seq()
        expect(tok, token::close);
        return new seq_node(ts);
    }
    if (tok.kind == token::integer || tok.kind == token::number) {      //e.g seq(5)
        ts = stod(tok.data);
        expect(tok, token::integer, token::number);
        expect(tok, token::close);
        return new seq_node(ts);
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
    return new seq_node(pattexp, ts);
}


//  <comp_pipe> ::= ('comp' | 'pipe') '(' <pattexp> (',' <pattexp>)+ ')'
skel_node* parser::comp_pipe_rule(token& tok)
{
    skel_node* pattexp;
    token::type kind = tok.kind;
    expect(tok, token::comp, token::pipe);
    expect(tok, token::open);
    pattexp = pattexp_rule(tok);

    // distinguish comp and pipe
    skel_node* comp_pipe = ( kind == token::comp ) ?
        static_cast<skel_node*>(new comp_node{}):
        static_cast<skel_node*>(new pipe_node{});

    // add the pattexps to the comp_pipe
    comp_pipe->add(pattexp);
    while (tok.kind == token::comma) {
        tok = scanner.next();
        pattexp = pattexp_rule(tok);
        comp_pipe->add(pattexp);
    }

    expect(tok, token::close);
    return comp_pipe;
}

//  <farm_map> ::= ('farm' | 'map') '(' <pattexp> [',' <integer>] ')'
skel_node* parser::farm_map_rule(token& tok)
{
    skel_node* pattexp;
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
        return new farm_node(pattexp, nw);
    return new map_node(pattexp, nw);
}

//  <comp> ::= token::open <pattexp> token::close
//
///////////////////////////////////////////////////////////////////////////////
skel_node* parser::reduce_rule(token& tok)
{
    skel_node* pattexp;
    expect(tok, token::reduce);
    expect(tok, token::open);
    pattexp = pattexp_rule(tok);
    expect(tok, token::close);
    return new reduce_node(pattexp);
}

//  Identifier parse rule, it just eat the id token
skel_node* parser::id_rule(token& tok)
{
    pair<string, int> id;
    expect(tok, token::word, id);
    id.second = id.second < 0 ? 0 : id.second;
    return new id_node(id.first, id.second);
}
