#include "nodes/skeletons.hpp"
#include "visitors.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <tuple>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

void printer::tostring(const string& name, const skel_node& n) {
    res += name + "(";
    for (size_t i = 0; i < n.size(); i++) {
        n.get(i)->accept(*this);
        res += (i != n.size()-1 ? "," : "");
    }
    res += ")";
}

void printer::visit(seq_node& n) {
    tostring("seq", n);
}

void printer::visit(comp_node& n) {
    tostring("comp", n);
}

void printer::visit(pipe_node& n) {
    tostring("pipe", n);
}

void printer::visit(farm_node& n) {
    tostring("farm", n);
}

void printer::visit(map_node& n) {
    tostring("map", n);
}

void printer::visit(reduce_node& n) {
    tostring("reduce", n);
}

void printer::visit(id_node& n) {
    res += n.id;
}

string printer::print( skel_node& sk ){
    return (*this)( sk );
}

string printer::operator()(skel_node& sk){
    res.clear();
    sk.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

void label_printer::visit( seq_node& n ) {
    str = "seq";
}

void label_printer::visit( comp_node& n ) {
    str = "comp";
}

void label_printer::visit( pipe_node& n ) {
    str = "pipe";
}

void label_printer::visit( farm_node& n ) {
    str = "farm";
}

void label_printer::visit( map_node& n ) {
    str = "map";
}

void label_printer::visit( reduce_node& n ) {
    str = "reduce";
}

void label_printer::visit( id_node& n ) {
    str = n.id;
}

string label_printer::operator()( skel_node& sk ) {
    sk.accept(*this);
    return str;
}

///////////////////////////////////////////////////////////////////////////////

void single_node_cloner::visit( seq_node& n ) {
    tmp = new seq_node(n);
}

void single_node_cloner::visit( comp_node& n ) {
    tmp = new comp_node{};
}

void single_node_cloner::visit( pipe_node& n ) {
    tmp = new pipe_node{};
}

void single_node_cloner::visit( farm_node& n ) {
    tmp = new farm_node{};
}

void single_node_cloner::visit( map_node& n ) {
    tmp = new map_node{};
}

void single_node_cloner::visit( reduce_node& n ) {
    tmp = new reduce_node{};
}

void single_node_cloner::visit( id_node& n ) {
    tmp = new id_node(n.id);
}

skel_node* single_node_cloner::operator()( skel_node& sk ) {
    sk.accept(*this);
    return tmp;
}

///////////////////////////////////////////////////////////////////////////////

reduce_resources::reduce_resources( rpl_environment& env ) :
    ts(env), getres(env)
{}

void reduce_resources::visit( seq_node& n ) {
    res = false;
}

typedef std::tuple<skel_node*, double, size_t> sds_t;

template < typename triples_container >
void fill_triples( triples_container& triples, skel_node& n, servicetime& ts, resources& res ) {
    for (size_t i = 0; i < n.size(); i++) {
        triples.push_back( make_tuple(
            n.get(i),
            ts ( *n.get(i) ),
            res( *n.get(i) )
        ) );
    }
}

void reduce_resources::visit( comp_node& n ) {

    std::vector<sds_t> triples;
    fill_triples( triples, n, ts, getres );

    sort (triples.begin(), triples.end(), [](const sds_t& a, const sds_t& b) {
        if (get<2>(a) == get<2>(b))
            return get<1>(b) < get<1>(a);
        return get<2>(a) > get<2>(b);
    });

    for ( auto it = triples.begin(); it != triples.end() && !res; it++ )
        (*this)( *get<0>(*it) );
}

void reduce_resources::visit( pipe_node& n ) {

    std::vector<sds_t> triples;
    fill_triples( triples, n, ts, getres );

    sort (triples.begin(), triples.end(), [](const sds_t& a, const sds_t& b) {
        if (get<1>(a) == get<1>(b))
            return get<2>(b) < get<2>(a);
        return get<1>(a) < get<1>(b);
    });

    for ( auto it = triples.begin(); it != triples.end() && !res; it++ )
        (*this)( *get<0>(*it) );
}

void reduce_resources::visit( farm_node& n ) {
    res = n.pardegree > 1;
    if ( res )
        n.pardegree--;
    else
        (*this)( *n.get(0) );
}

void reduce_resources::visit( map_node& n ) {
    res = n.pardegree > 1;
    if ( res )
        n.pardegree--;
    else
        (*this)( *n.get(0) );
}

void reduce_resources::visit( reduce_node& n ) {
    res = false;
}

void reduce_resources::visit( id_node& n ) {
    res = false;
}

bool reduce_resources::operator()(skel_node& n) {
    res = false;
    n.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

void assign_resources::visit( seq_node& n ) {
}

void assign_resources::visit( comp_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)(*n.get(i), n.inputsize);
}

void assign_resources::visit( pipe_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)(*n.get(i), n.inputsize);
}

void assign_resources::visit( farm_node& n ) {
    (*this)(*n.get(0), n.inputsize);
}

void assign_resources::visit( map_node& n ) {
    (*this)(*n.get(0), n.inputsize/n.pardegree);
}

void assign_resources::visit( reduce_node& n ) {
    (*this)(*n.get(0), n.inputsize/n.pardegree);
}

void assign_resources::visit( id_node& n ) {
}

void assign_resources::operator()(skel_node& n, double inputsize) {
    n.inputsize = inputsize;
    n.accept(*this);
}
