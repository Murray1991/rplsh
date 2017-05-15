#ifndef rpl_optimizers_hpp
#define rpl_optimizers_hpp

#include "visitor_interface.hpp"
#include "visitors.hpp"
#include "rpl_environment.hpp"

struct optrule : public skel_visitor
{
    virtual void operator() ( skel_node& sk ) = 0;
};

struct farmopt : public optrule
{
    farmopt(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    void operator()( skel_node& sk );
private:
    rpl_environment& env;
    servicetime ts;
};

#endif
