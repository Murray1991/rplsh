///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter visitor and environment declarations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_interpreter_hpp
#define rpl_interpreter_hpp

#include "visitors.hpp"
#include "verbs.hpp"
#include "skeletons.hpp"
#include "rpl_environment.hpp"
#include "error_report.hpp"
#include <tuple>
#include <exception>
#include <iostream>
#include <map>
#include "dispatchers/rr_dispatcher.hpp"
#include "dispatchers/set_dispatcher.hpp"
#include "dispatchers/get_dispatcher.hpp"
#include "dispatchers/opt_dispatcher.hpp"
#include "dispatchers/visitor_dispatcher.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Template class of the rpl interpreter. Template arguments are Env,
//  for the environment <name, skel_node_ptr> and Disp for the dispatcher
//  <name, skel_visitor>
//
///////////////////////////////////////////////////////////////////////////////
struct interpreter : public visitor
{
    interpreter(rpl_environment& env, error_container& err_repo);
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
    rpl_environment& env;
    error_container& err_repo;
    rr_dispatcher rdispatch;
    set_dispatcher sdispatch;
    get_dispatcher gdispatch;
    opt_dispatcher odispatch;
    visitor_dispatcher vdispatch;
    bool success;
};

#endif
