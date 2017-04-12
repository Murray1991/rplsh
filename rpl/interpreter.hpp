///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter visitor and environment declarations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_interpreter_hpp
#define rpl_interpreter_hpp

#include "visitor.hpp"
#include "environment.hpp"
#include "error_report.hpp"
#include <tuple>
#include <exception>
#include <iostream>
#include <map>

///////////////////////////////////////////////////////////////////////////////
//
//  Template class that dispatch a name of type K returning a shared_ptr of
//  the instantiated object associated to the name, or a logic_error
//  exception
//
///////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
struct dispatcher
{
    shared_ptr<V> operator[](const K& id);                               // could throw logic_error exception
    void add(const K& id, shared_ptr<V> value);
private:
    std::map<K, shared_ptr<V>> dispatch;
};

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

private:
    Env& env;
    Disp& dispatch;
    error_container& err_repo;
    bool success;
};

#endif
