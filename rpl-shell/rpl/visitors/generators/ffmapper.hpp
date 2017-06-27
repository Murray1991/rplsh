#ifndef rpl_ffmapper_hpp
#define rpl_ffmapper_hpp

#include "visitors/visitor_interface.hpp"
#include "visitors/evaluators/evaluators.hpp"
#include "environment/rpl_environment.hpp"

struct ffmapper : public skel_visitor {
public:
    ffmapper(rpl_environment& env);
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    void clear();
    std::vector<std::size_t> get_worker_mapping() const;
    const ffmapper& operator()(skel_node& n);

private:
    std::vector<std::size_t> mw;
    rpl_environment& env;
    std::size_t startID;
    std::size_t endID;
    resources res;
};

#endif
