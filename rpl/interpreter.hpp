///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter visitor and environment declarations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_interpreter_hpp
#define rpl_interpreter_hpp

#include "visitors.hpp"
#include "dispatcher.hpp"
#include "verbs.hpp"
#include "skeletons.hpp"
#include "environment.hpp"
#include "error_report.hpp"
#include <tuple>
#include <exception>
#include <iostream>
#include <map>

///////////////////////////////////////////////////////////////////////////////
//
//  Template class of the rpl interpreter. Template arguments are Env,
//  for the environment <name, skel_node_ptr> and Disp for the dispatcher
//  <name, skel_visitor>
//
///////////////////////////////////////////////////////////////////////////////
template <typename Env, typename Disp>
struct interpreter : public visitor
{
    interpreter(Env& env, Disp& dispatch, error_container& err_repo);
    void visit(assign_node& n);
    void visit(show_node& n);
    void visit(set_node& n);
    void visit(ann_node& n);
    void visit(rwr_node& n);
    void visit(opt_node& n);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

private:
    Env& env;
    Disp& dispatch;
    error_container& err_repo;
    bool success;
};

#endif
