///////////////////////////////////////////////////////////////////////////////
//
//  Parser classes of the rpl shell language. Class parser performs
//  the syntax analysis and generate an intermediate AST
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_parser_hpp
#define rpl_parser_hpp

#include "lexer.hpp"
#include "rtree.hpp"
#include "visitor.hpp"
#include <iostream>
#include <ostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
//  This class generate a rpl tree (intermediate ast) by parsing
//  the defined rpl language. It implements a simple recursive
//  descent parser by calling its private parsing rules
//
///////////////////////////////////////////////////////////////////////////////
struct parser
{
    parser(lexer& scanner, error_container& err_repo);
    std::shared_ptr<rpl_node> parse();

private:
    std::shared_ptr<rpl_node> start_rule(token& tok);
    std::shared_ptr<rpl_node> assign_rule(token& tok);
    std::shared_ptr<rpl_node> show_rule(token& tok);
    std::shared_ptr<rpl_node> set_rule(token& tok);
    std::shared_ptr<rpl_node> ann_rule(token& tok);
    std::shared_ptr<rpl_node> opt_rule(token& tok);
    std::shared_ptr<rpl_node> rwr_rule(token& tok);

    std::shared_ptr<skel_node> pattexp_rule(token& tok);
    std::shared_ptr<skel_node> id_rule(token& tok);
    std::shared_ptr<skel_node> seq_rule(token& tok);
    std::shared_ptr<skel_node> comp_pipe_rule(token& tok);
    std::shared_ptr<skel_node> farm_map_rule(token& tok);
    std::shared_ptr<skel_node> reduce_rule(token& tok);

    bool expect(token& tok, token::type exp);
    bool expect(token& tok, token::type exp, std::string& data);
    bool expect(token& tok, token::type exp, int& data);
    bool expect(token& tok, token::type exp, double& data);
    bool expect(token& tok, token::type exp1, token::type exp2);

    lexer& scanner;
    error_container& err_repo;
};

#endif
