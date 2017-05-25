#ifndef rpl_optimizers_hpp
#define rpl_optimizers_hpp

#include "visitors/visitor_interface.hpp"
#include "visitors/visitors.hpp"
#include "environment/rpl_environment.hpp"

/*
    interface for each optimize rule: recursively
    apply the optimization rule for each children
    of a visited node, excluding the sequential
    node and the identifier node
*/
struct optrule : public skel_visitor
{
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    virtual void operator() ( skel_node& n ) = 0;

protected:
    optrule(rpl_environment& env);
    rpl_environment& env;
    servicetime ts;
};

// choose parallelism degree such that
// the new servicetime is equal to the
// emitter service time
struct farmopt : public optrule
{
    farmopt( rpl_environment& env );
    void visit( farm_node& n );
    void operator()( skel_node& n );
};

// given a skeleton pipe node it tries
// to reduce the parallelism degree of
// its fastest children
struct pipebalance : public optrule
{
    pipebalance( rpl_environment& env );
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void operator()( skel_node& n );
private:
    void operator()( skel_node& n, double max );
    double ts_max;
};

// 1) balances the stages -> call pipebalance
// 2) merges faster stages
struct pipeopt : public optrule
{
    pipeopt( rpl_environment& env );
    void visit( pipe_node& n );
    void operator()( skel_node& n );
private:
    pipebalance balance;
};

struct maxresources : public optrule
{
    maxresources ( rpl_environment& env );
    void visit( comp_node& n );
    void visit( pipe_node& n );
    void visit( farm_node& n );
    void visit( map_node& n );
    void operator()( skel_node& n );
private:
    void operator()( skel_node& n, size_t maxres );
    reduce_resources reduce_res;
    resources res;
    size_t maxres;
};

#endif
