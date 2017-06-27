#ifndef rpl_compseq_setter_hpp
#define rpl_compseq_setter_hpp

#include "visitors/visitor_interface.hpp"
#include "visitors/evaluators/evaluators.hpp"
#include "environment/rpl_environment.hpp"

struct compseq_setter : public skel_visitor {
public:
    compseq_setter(rpl_environment& env);
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    void operator()(skel_node& n);

private:
    rpl_environment& env;
};

#endif
