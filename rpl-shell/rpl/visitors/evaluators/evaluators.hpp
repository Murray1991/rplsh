#ifndef rpl_evaluators_hpp
#define rpl_evaluators_hpp

#include "environment/rpl_environment.hpp"
#include "visitors/visitor_interface.hpp"
#include "utils/printable.hpp"

struct eval_visitor : public skel_visitor, public printable
{
    eval_visitor( rpl_environment& env );
protected:
    rpl_environment& env;
};

struct servicetime: public eval_visitor
{
    servicetime(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& n);
    double operator()(skel_node& n);
private:
    double res;
};

struct latencytime: public eval_visitor
{
    latencytime(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& n);
    double operator()(skel_node& n);
protected:
    double res;
};

struct completiontime: public eval_visitor
{
    completiontime(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& n);
    double operator()(skel_node& n);
protected:
    latencytime lat;
    servicetime ts;
    double res;
};

struct pardegree: public eval_visitor
{
    pardegree(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& n);
    std::size_t operator()(skel_node& n);
protected:
    std::size_t res;
};

struct resources: public eval_visitor
{
    resources(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& n);
    std::size_t operator()(skel_node& n);
protected:
    std::size_t res;
};

#endif
