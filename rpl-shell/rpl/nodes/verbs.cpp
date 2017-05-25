#include "verbs.hpp"
#include "visitors/visitor_interface.hpp"

using namespace std;
///////////////////////////////////////////////////////////////////////////////

assign_node::assign_node(const string& id, skel_node* rvalue)
    : id(id), rvalue(rvalue) {}

void assign_node::accept(visitor &v) {
    v.visit( *this );
}

///////////////////////////////////////////////////////////////////////////////

template <typename verb>
verb_node<verb>::verb_node( verb& _verb, const string& id, const string& prop )
    : _verb(_verb), id(id), prop(prop) {}

template <typename verb>
void verb_node<verb>::accept( visitor& v ) {
    v.visit( _verb );
}

///////////////////////////////////////////////////////////////////////////////

show_node::show_node(const string& id, const int& lines, const string& prop, vector<string> parameters)
    : verb_node(*this, id, prop), parameters(parameters), lines_to_print(lines) {}

///////////////////////////////////////////////////////////////////////////////

set_node::set_node(const string& id, const string& prop, double value)
    : verb_node(*this, id, prop), value(value) {}

///////////////////////////////////////////////////////////////////////////////

ann_node::ann_node(const string& id, const string& prop, double value)
    : verb_node(*this, id, prop), value(value) {}

///////////////////////////////////////////////////////////////////////////////

rwr_node::rwr_node(const string& id, const string& rule)
    : verb_node(*this, id, rule) {}

///////////////////////////////////////////////////////////////////////////////

opt_node::opt_node(const string& id, const string& opt)
    : verb_node(*this, id, opt) {}
