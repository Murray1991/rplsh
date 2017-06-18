#ifndef rpl_ffcode_hpp
#define rpl_ffcode_hpp

#include "visitors/visitor_interface.hpp"
#include "visitors/visitors.hpp"
#include "environment/rpl_environment.hpp"
#include <queue>
#include <utility>
#include <sstream>
#include <string>

struct ffcode : public skel_visitor {
    ffcode(rpl_environment& env);

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
    void comp_pipe(const std::string& type, const std::string& name, skel_node& n);
    void seq_wraps(const std::string& name);

protected:
    rpl_environment& env;
    get_seq_wrappers gsw;

    std::queue<std::pair<std::string,std::string>> code_lines;
};

#endif
