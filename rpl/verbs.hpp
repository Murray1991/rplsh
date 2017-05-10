#ifndef rpl_verbs_hpp
#define rpl_verbs_hpp

#include <string>
#include "node_interface.hpp"

//Forward declaration of a skeleton node
//TODO maybe better templating the assign_node
//TODO probably better use some type "rvalue_node"
//and keep it in "node_interface.hpp"
//The #include "skeletons.hpp" works too
struct skel_node;

//  The interface for the assign node
struct assign_node : public rpl_node {
    assign_node(const std::string& id, skel_node* rvalue);
    void accept(visitor & v);
    std::string id;
    skel_node* rvalue;
};

// The interface for all the other verb nodes
template <typename verb>
struct verb_node : public rpl_node {
    void accept(visitor& v);
protected:
    verb_node(verb& _verb, const std::string& id, const std::string& prop);
private:
    verb& _verb;
public:
    std::string id;
    std::string prop;
};

// Show node
struct show_node : public verb_node<show_node> {
    show_node(const std::string& id, const std::string& prop);
};

// Set node
struct set_node : public verb_node<set_node> {
    set_node(const std::string& id, const std::string& prop, int value);
    int value;
};

// Ann node
struct ann_node : public verb_node<ann_node> {
    ann_node(const std::string& id, const std::string& prop, double value);
    double value;
};

// Rwr node
struct rwr_node : public verb_node<rwr_node> {
    rwr_node(const std::string& id, const std::string& rule);
};

// Opt node
struct opt_node : public verb_node<opt_node> {
    opt_node(const std::string& id, const std::string& opt);
};

#endif
