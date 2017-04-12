#include "rtree.hpp"
#include "visitor.hpp"

using namespace std;

verb_node::verb_node(const string& id, const string& prop)
    : id(id), prop(prop) {}

assign_node::assign_node(const string& id, const shared_ptr<skel_node>& rvalue)
    : id(id), rvalue(rvalue) {}

void assign_node::accept(visitor &v) {
    v.visit(*this);
}

show_node::show_node(const string& id, const string& prop)
    : verb_node(id, prop) {}

void show_node::accept(visitor &v) {
    v.visit(*this);
}

set_node::set_node(const string& id, const string& prop, int value)
    : verb_node(id, prop), value(value) {}

void set_node::accept(visitor &v) {
    v.visit(*this);
}

ann_node::ann_node(const string& id, const string& prop, double value)
    : verb_node(id, prop), value(value) {}

void ann_node::accept(visitor &v) {
    v.visit(*this);
}

rwr_node::rwr_node(const string& id, const string& rule)
    : verb_node(id, rule) {}

void rwr_node::accept(visitor &v) {
    v.visit(*this);
}

opt_node::opt_node(const string& id, const string& opt)
    : verb_node(id, opt) {}

void opt_node::accept(visitor& v) {
    v.visit(*this);
}

access_node::access_node() {
}

access_node::access_node(std::shared_ptr<skel_node> sk) {
    children.push_back(sk);
}

void access_node::push(std::shared_ptr<skel_node> sk) {
    children.push_back(sk);
}

shared_ptr<skel_node> access_node::get(size_t idx) const {
    if (idx < children.size())
        return children[idx];
    return nullptr;
}

int access_node::size() const {
    return children.size();
}

seq_node::seq_node (double servicetime)
    : access_node(), servicetime(servicetime) {}

seq_node::seq_node (shared_ptr<skel_node> pattexp, double servicetime)
    : access_node(pattexp), servicetime(servicetime) {}

void seq_node::accept(visitor& v) {
    v.visit(*this);
}

void seq_node::accept(skel_visitor& v) {
    v.visit(*this);
}

void comp_node::accept(visitor& v) {
    v.visit(*this);
}

void comp_node::accept(skel_visitor& v) {
    v.visit(*this);
}

void pipe_node::accept(visitor& v) {
    v.visit(*this);
}

void pipe_node::accept(skel_visitor& v) {
    v.visit(*this);
}

farm_node::farm_node(shared_ptr<skel_node> pattexp, int pardegree)
        : access_node(pattexp), pardegree(pardegree) {
}

void farm_node::accept(visitor& v) {
    v.visit(*this);
}

void farm_node::accept(skel_visitor& v) {
    v.visit(*this);
}

map_node::map_node(shared_ptr<skel_node> pattexp, int pardegree)
        : access_node(pattexp), pardegree(pardegree) {}

void map_node::accept(visitor& v) {
    v.visit(*this);
}

void map_node::accept(skel_visitor& v) {
    v.visit(*this);
}

reduce_node::reduce_node(shared_ptr<skel_node> pattexp)
    : access_node(pattexp) {}

void reduce_node::accept(visitor& v) {
    v.visit(*this);
}

void reduce_node::accept(skel_visitor& v) {
    v.visit(*this);
}

id_node::id_node(string& id)
    : id (id) {}

void id_node::accept(visitor& v) {
    v.visit(*this);
}

void id_node::accept(skel_visitor& v) {
    v.visit(*this);
}
