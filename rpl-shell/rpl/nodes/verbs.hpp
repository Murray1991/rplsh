#ifndef rpl_verbs_hpp
#define rpl_verbs_hpp

#include <string>
#include <vector>
#include "node_interface.hpp"

//Forward declaration of a skeleton node
//TODO maybe better templating the assign_node
//TODO probably better use some type "rvalue_node"
//and keep it in "node_interface.hpp"
//The #include "skeletons.hpp" works too
struct skel_node;

// Node representing an assignment
struct assign_node : public rpl_node {
    assign_node(const std::string& id, skel_node* rvalue);
    void accept(visitor & v);
    std::string id;
    skel_node* rvalue;
};

// The interface for all the verb nodes
template <typename verb>
struct verb_node : public rpl_node {
    void accept(visitor& v);
protected:
    verb_node(verb& _verb, const std::string& id, const std::string& prop);
    verb_node(verb& _verb, const std::string& id, const std::vector<std::string>& parameters);
    verb_node(verb& _verb, const std::pair<std::string, std::size_t>& id, const std::string& prop);
    verb_node(verb& _verb, const std::pair<std::string, std::size_t>& id, const std::vector<std::string>& parameters);
private:
    verb& _verb;
public:
    std::string id;
    std::size_t index;
    std::string prop;
    const std::vector<std::string> parameters;
};

// Node representing the "show" verb
struct show_node : public verb_node<show_node> {
    show_node(const std::string& id, const int& lines, const std::vector<std::string>& parameters);
    show_node(const std::pair<std::string, std::size_t>& id, const int& lines, const std::vector<std::string>& parameters);
    const int lines_to_print;
};

// Node representing the "set" verb
struct set_node : public verb_node<set_node> {
    set_node(const std::string& id, const std::string& prop, double value);
    double value;
};

// Node representing the "annotate" verb
struct ann_node : public verb_node<ann_node> {
    ann_node(const std::string& id, const std::string& prop, double value);
    ann_node(const std::pair<std::string,std::size_t>& id, const std::string& prop, double value);
    double value;
};

// Node representing the "rewrite" verb
struct rwr_node : public verb_node<rwr_node> {
    rwr_node(const std::string& id, const std::vector<std::string>& parameters);
    rwr_node(const std::pair<std::string,std::size_t>& id, const std::vector<std::string>& parameters);
};

// Node representing the "optimize" verb
struct opt_node : public verb_node<opt_node> {
    opt_node(const std::string& id, const std::vector<std::string>& parameters);
    opt_node(const std::pair<std::string,std::size_t>& id, const std::vector<std::string>& parameters);
};

#endif
