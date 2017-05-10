///////////////////////////////////////////////////////////////////////////////
//  Concrete visitors:
//  printer, servicetime, latencytime, ...
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_visitor_hpp
#define rpl_visitor_hpp

#include "environment.hpp"
#include "visitor_interface.hpp"

template <typename Env>
struct servicetime: public skel_visitor
{
    servicetime(Env& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    void print(skel_node& sk);
    double operator()(skel_node& sk);
private:
    Env& env;
    double res;
};


template <typename Env>
struct latencytime: public skel_visitor
{
    latencytime(Env& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    void print(skel_node& sk);
    double operator()(skel_node& sk);
protected:
    Env& env;
    double res;
};

/*
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
};*/

struct printer : public skel_visitor
{
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    void print(skel_node& sk);
    std::string operator()(skel_node& sk);
private:
    std::string res;
    void tostring(const std::string& name, const skel_node& n);
};

#endif
