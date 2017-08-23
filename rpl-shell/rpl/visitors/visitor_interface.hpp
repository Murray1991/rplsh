#ifndef rpl_Ivisitor_hpp
#define rpl_Ivisitor_hpp

#include "nodes/node_forwards.hpp"
#include <string>

struct visitor
{
    virtual void visit(assign_node& n)  = 0;
    virtual void visit(show_node& n)    = 0;
    virtual void visit(set_node& n)     = 0;
    virtual void visit(ann_node& n)     = 0;
    virtual void visit(rwr_node& n)     = 0;
    virtual void visit(opt_node& n)     = 0;
    virtual void visit(history_node& n) = 0;
    virtual void visit(import_node& n)  = 0;
    virtual void visit(gencode_node& n) = 0;
    virtual void visit(expand_node& n)  = 0;
    virtual void visit(add_node& n)     = 0;

    virtual void visit(seq_node& n)     = 0;
    virtual void visit(source_node& n)  = 0;
    virtual void visit(drain_node& n)   = 0;
    virtual void visit(comp_node& n)    = 0;
    virtual void visit(pipe_node& n)    = 0;
    virtual void visit(farm_node& n)    = 0;
    virtual void visit(map_node& n)     = 0;
    virtual void visit(reduce_node& n)  = 0;
    virtual void visit(id_node& n)      = 0;

    virtual void visit(_& n)            {}
};

struct skel_visitor : public visitor
{
    using visitor::visit;

    // notice hat source and drain have
    // the base_class implementation
    virtual void visit(source_node& n) {};
    virtual void visit(drain_node& n)  {};

    void visit(assign_node& n)  {};
    void visit(show_node& n)    {};
    void visit(set_node& n)     {};
    void visit(ann_node& n)     {};
    void visit(rwr_node& n)     {};
    void visit(opt_node& n)     {};
    void visit(history_node& n) {};
    void visit(import_node& n)  {};
    void visit(gencode_node& n) {};
    void visit(expand_node& n)  {};
    void visit(add_node& n)  {};
};

#endif
