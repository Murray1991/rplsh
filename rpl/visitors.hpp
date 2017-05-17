///////////////////////////////////////////////////////////////////////////////
//  Concrete visitors:
//  printer, servicetime, latencytime, ...
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_visitor_hpp
#define rpl_visitor_hpp

#include "rpl_environment.hpp"
#include "visitor_interface.hpp"

struct servicetime: public skel_visitor
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
    rpl_environment& env;
    double res;
};

struct latencytime: public skel_visitor
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
    rpl_environment& env;
    double res;
};

struct completiontime: public skel_visitor
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
    rpl_environment& env;
    double res;
};

struct pardegree: public skel_visitor
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
    rpl_environment& env;
    std::size_t res;
};

struct resources: public skel_visitor
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
    rpl_environment& env;
    std::size_t res;
};

struct printer : public skel_visitor
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

// implementation of a visitor
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

#endif
