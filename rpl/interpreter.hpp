///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter visitor and environment declarations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_interpreter_hpp
#define rpl_interpreter_hpp

#include "visitor.hpp"
#include "error_report.hpp"
#include <exception>
#include <iostream>
#include <map>

typedef std::map<std::string, std::shared_ptr<skel_node>> binding_map;

///////////////////////////////////////////////////////////////////////////////
//
//  The environment contains the following bindings:
//  * name - skeleton tree
//  * name - annotation                 <--- TODO
//
///////////////////////////////////////////////////////////////////////////////
struct environment
{
    skel_node& get(std::string& id);    // throw an exception env[id] == null
    void put(std::string& id, std::shared_ptr<skel_node> n);

    binding_map env;
};

struct interpreter : public visitor
{
    interpreter(environment& env, error_container& err_repo);
    virtual void visit(assign_node& n);
    virtual void visit(show_node& n);
    virtual void visit(set_node& n);
    virtual void visit(ann_node& n);
    virtual void visit(rwr_node& n);
    virtual void visit(opt_node& n);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);

    environment& env;
    error_container& err_repo;
    std::map<std::string, shared_ptr<skel_visitor>> dispatch;
};

#endif
