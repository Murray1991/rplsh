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
    : _verb(_verb), id(id), index(0), prop(prop), parameters({}) {}

template <typename verb>
verb_node<verb>::verb_node( verb& _verb, const string& id, const std::vector<string>& parameters )
    : _verb(_verb), id(id), index(0), prop(""), parameters(parameters) {}

template <typename verb>
verb_node<verb>::verb_node( verb& _verb, const pair<string,int>& id, const string& prop )
    : _verb(_verb), id(id.first), index(id.second), prop(prop), parameters({}) {}

template <typename verb>
verb_node<verb>::verb_node( verb& _verb, const pair<string,int>& id, const std::vector<string>& parameters )
    : _verb(_verb), id(id.first), index(id.second), prop(""), parameters(parameters) {}

template <typename verb>
void verb_node<verb>::accept( visitor& v ) {
    v.visit( _verb );
}

///////////////////////////////////////////////////////////////////////////////

show_node::show_node(const string& id, const int& lines, const vector<string>& parameters)
    : verb_node(*this, id, parameters), lines_to_print(lines) {}

show_node::show_node(const pair<string,int>& id, const int& lines, const vector<string>& parameters)
    : verb_node(*this, id, parameters), lines_to_print(lines) {}

///////////////////////////////////////////////////////////////////////////////

set_node::set_node(const string& id, const string& prop, double value)
    : verb_node(*this, id, prop), value(value) {}

///////////////////////////////////////////////////////////////////////////////

ann_node::ann_node(const string& id, const string& prop, double value, bool bvalue)
    : verb_node(*this, id, prop), value(value), bvalue(bvalue) {}

ann_node::ann_node(const pair<string,int>& id, const string& prop, double value, bool bvalue)
    : verb_node(*this, id, prop), value(value), bvalue(bvalue) {}

///////////////////////////////////////////////////////////////////////////////

rwr_node::rwr_node(const string& id, const vector<string>& parameters)
    : verb_node(*this, id, parameters) {}

rwr_node::rwr_node(const pair<string,int>& id, const vector<string>& parameters)
    : verb_node(*this, id, parameters) {}

///////////////////////////////////////////////////////////////////////////////

opt_node::opt_node(const string& id, const vector<string>& parameters)
    : verb_node(*this, id, parameters) {}

opt_node::opt_node(const pair<string,int>& id, const vector<string>& parameters)
    : verb_node(*this, id, parameters) {}

///////////////////////////////////////////////////////////////////////////////

history_node::history_node(const string& id)
    : verb_node(*this, id, id) {}

///////////////////////////////////////////////////////////////////////////////

import_node::import_node(const string& id)
    : verb_node(*this, id, id) {}

///////////////////////////////////////////////////////////////////////////////

gencode_node::gencode_node(const string& id)
    : verb_node(*this, id, id) {}
