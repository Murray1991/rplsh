///////////////////////////////////////////////////////////////////////////////
//  Concrete visitors:
//  printer, servicetime, latencytime, ...
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_visitor_hpp
#define rpl_visitor_hpp

#include "environment/rpl_environment.hpp"
#include "evaluators/evaluators.hpp"
#include "visitor_interface.hpp"
#include "utils/printable.hpp"

struct printer : public skel_visitor, public printable
{
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::string print(skel_node& sk);
    std::string operator()(skel_node& sk);
private:
    std::string res;
    void tostring(const std::string& name, const skel_node& n);
};

// implementation of a visitor
struct label_printer : public skel_visitor {
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    std::string operator()(skel_node& n);
private:
    std::string str;
};

struct single_node_cloner : public skel_visitor {
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    skel_node* operator()(skel_node& n);
private:
    skel_node* tmp;
};

struct reduce_resources : public skel_visitor {
    reduce_resources( rpl_environment& env );
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    bool operator()(skel_node& n);
private:
    bool res;
    servicetime ts;
    resources getres;
};

struct assign_resources : public skel_visitor {
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    void operator()(skel_node& n, double inputsize);
};

struct get_seq_wrappers : public skel_visitor {
    get_seq_wrappers(rpl_environment& env);
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::vector<seq_node*> get_seq_nodes();
    std::vector<drain_node*> get_drain_nodes();
    std::vector<source_node*> get_source_nodes();

    void operator()(skel_node& n);
private:
    rpl_environment& env;
    std::vector<seq_node*> seq_nodes;
    std::vector<drain_node*> drn_nodes;
    std::vector<source_node*> src_nodes;
};

struct top_datap_skeletons : public skel_visitor {
    top_datap_skeletons(rpl_environment& env);
    void visit(seq_node& n);
    void visit(source_node& n);
    void visit(drain_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);

    std::vector<map_node*> get_map_nodes();
    std::vector<reduce_node*> get_reduce_nodes();

    void operator()(skel_node& n);
private:
    rpl_environment& env;
    std::vector<map_node*> map_nodes;
    std::vector<reduce_node*> red_nodes;
};

struct ranker : public skel_visitor {
    ranker(rpl_environment& env);
    void visit(seq_node& n);
    void visit(comp_node& n);
    void visit(pipe_node& n);
    void visit(farm_node& n);
    void visit(map_node& n);
    void visit(reduce_node& n);
    void visit(id_node& n);
    void operator()(skel_node& n);
private:
    rpl_environment& env;
    single_node_cloner snc;
};

struct unranker : public skel_visitor {
    unranker(rpl_environment& env);
    void visit(seq_node& n);
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
