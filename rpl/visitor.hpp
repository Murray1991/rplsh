#ifndef rpl_visitor_hpp
#define rpl_visitor_hpp

#include "environment.hpp"
#include "rtree.hpp"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
//  The skeleton visitor abstract class
//
///////////////////////////////////////////////////////////////////////////////
struct base_visitor
{
    virtual void visit(seq_node& n)     = 0;
    virtual void visit(comp_node& n)    = 0;
    virtual void visit(pipe_node& n)    = 0;
    virtual void visit(farm_node& n)    = 0;
    virtual void visit(map_node& n)     = 0;
    virtual void visit(reduce_node& n)  = 0;
    virtual void visit(id_node& n)      = 0;
};

struct skel_visitor : public virtual base_visitor
{
    using base_visitor::visit;
    virtual void print()                    = 0;
    virtual const skel_visitor& operator()(skel_node& sk)  = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The visitor abstract class
//
///////////////////////////////////////////////////////////////////////////////
struct visitor : public virtual base_visitor
{
    using base_visitor::visit;
    virtual void visit(assign_node& n) = 0;
    virtual void visit(show_node& n)   = 0;
    virtual void visit(set_node& n)    = 0;
    virtual void visit(ann_node& n)    = 0;
    virtual void visit(rwr_node& n)    = 0;
    virtual void visit(opt_node& n)    = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Concrete visitors:
//  servicetime, latencytime, ...
//
//  TODO: there is a way to reduce the boilerplate
//  of code for defining the visitors?
//
///////////////////////////////////////////////////////////////////////////////
template <typename Env>
struct servicetime: public skel_visitor
{
    servicetime(Env& env);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
protected:
    Env& env;
    double res;
};

template <typename Env>
struct latencytime: public skel_visitor
{
    //using servicetime::visit;
    latencytime(Env& env);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
protected:
    Env& env;
    double res;
};

template <typename Env>
struct completiontime: public skel_visitor
{
    completiontime(Env& env);
    void set_dimension(int dim);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
protected:
    Env& env;
    double res;
    int dim;
};

template <typename Env>
struct pardegree: public skel_visitor
{
    pardegree(Env& env);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
protected:
    Env& env;
    int res;
};

template <typename Env>
struct resources: public skel_visitor
{
    resources(Env& env);
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
protected:
    Env& env;
    int res;
};

struct printer : public skel_visitor
{
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);
    virtual void print();
    virtual const skel_visitor& operator()(skel_node& sk);
private:
    std::string res;
    void print(const std::string& name, const access_node& n);
};

#endif
