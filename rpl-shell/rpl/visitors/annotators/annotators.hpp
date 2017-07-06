#ifndef rpl_annotators_hpp
#define rpl_annotators_hpp

#include "environment/rpl_environment.hpp"
#include "visitors/visitor_interface.hpp"

struct ann_visitor : public skel_visitor
{
    ann_visitor( rpl_environment& env );
    virtual void visit(seq_node& n);
    virtual void visit(source_node& n);
    virtual void visit(drain_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);

    // implementations of this operator should return 'result';
    // result should be set to true or false in overriden
    // visit methods according to the logic of the annotator
    virtual bool operator()( skel_node& n, double value ) = 0;

protected:
    rpl_environment& env;
    bool result;
};

struct ann_servicetime : public ann_visitor
{
    ann_servicetime( rpl_environment& env );
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);

    /* Return true if the value is correctly annotated, false otherwise. */
    bool operator()( skel_node& n, double value );

private:
    double servicetime;
};

struct ann_pardegree : public ann_visitor
{
    ann_pardegree( rpl_environment& env );
    void visit(farm_node& n);
    void visit(map_node& n);

    /* Return true if the value is correctly annotated, false otherwise. */
    bool operator()( skel_node& n, double value );

private:
    std::size_t nw;
};

struct ann_datap : public ann_visitor
{
    ann_datap( rpl_environment& env );
    void visit(seq_node& n);

    /* Return true if the value is correctly annotated, false otherwise. */
    bool operator()( skel_node& n, double value );

private:
    bool datap;
};

#endif
