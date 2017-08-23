///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter visitor and environment declarations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_interpreter_hpp
#define rpl_interpreter_hpp

#include "dispatchers/ann_dispatcher.hpp"
#include "dispatchers/set_dispatcher.hpp"
#include "dispatchers/get_dispatcher.hpp"
#include "dispatchers/opt_dispatcher.hpp"
#include "dispatchers/rr_dispatcher.hpp"
#include "dispatchers/eval_dispatcher.hpp"
#include "error_report/error_report.hpp"
#include "visitors/optimizers/normalform.hpp"
#include "visitors/generators/ffcode.hpp"
#include "visitors/visitors.hpp"
#include "nodes/skeletons.hpp"
#include "nodes/verbs.hpp"
#include "history.hpp"

#include <exception>
#include <iostream>
#include <tuple>
#include <map>

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
    void visit(history_node& n);
    void visit(import_node& n);
    void visit(gencode_node& n);
    void visit(expand_node& n);

    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    history& get_history();

private:
    rpl_environment& env;
    error_container& err_repo;
    rr_dispatcher rdispatch;
    ann_dispatcher adispatch;
    set_dispatcher sdispatch;
    get_dispatcher gdispatch;
    opt_dispatcher odispatch;
    eval_dispatcher vdispatch;
    assign_resources assignres;
    normalform normform;
    ffcode ff;
    history phistory;
    bool success;
};

#endif
