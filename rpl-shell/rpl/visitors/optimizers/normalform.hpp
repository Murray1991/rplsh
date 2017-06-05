
#ifndef rpl_normalform_hpp
#define rpl_normalform_hpp

#include "visitors/visitor_interface.hpp"
#include "environment/rpl_environment.hpp"
#include <vector>

struct normalform : public skel_visitor
{
    normalform ( rpl_environment& env );
    void visit(seq_node& n)     ;
    void visit(comp_node& n)    ;
    void visit(pipe_node& n)    ;
    void visit(farm_node& n)    ;
    void visit(map_node& n)     ;
    void visit(reduce_node& n)  ;
    void visit(id_node& n)      ;

    skel_node* operator()( skel_node& n );

private:
    rpl_environment& env;
    std::vector<skel_node*> leaves;
    bool success;
};

#endif
