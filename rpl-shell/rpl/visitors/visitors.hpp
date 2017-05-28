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

#endif
