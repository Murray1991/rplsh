#include "visitors.hpp"
#include "skeletons.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

template <typename Visitor, typename BinaryOp>
double compute(Visitor& v, skel_node& n, BinaryOp op) {
    double ts = 0;
    for (size_t i = 0; i < n.size() ; i++)
        ts = op(ts, v(*n.get(i)));
    return ts;
}

///////////////////////////////////////////////////////////////////////////////

servicetime::servicetime(rpl_environment& env)
    : env(env) {}

void servicetime::visit(seq_node& n) {
    res = n.servicetime;
}

void servicetime::visit(comp_node& n) {
    res = compute(*this, n, std::plus<double>());
}

void servicetime::visit(pipe_node& n) {
    res = compute(*this, n, [](double a, double b){return std::max(a, b);});
}

void servicetime::visit(farm_node& n) {
    res = (*this)(*n.get(0)) / n.pardegree;
}

void servicetime::visit(map_node& n) {
    res = (*this)(*n.get(0)) / n.pardegree;
}

// TODO WRONG -> for reduce is important the dim of input
void servicetime::visit(reduce_node& n) {
    res = log2( (*this)(*n.get(0)) );
}

void servicetime::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(*ptr);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

string servicetime::print( skel_node& sk ){
    return std::to_string( (*this)( sk ) );
}

double servicetime::operator()(skel_node& sk){
    sk.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

latencytime::latencytime(rpl_environment& env)
    : env( env ) {}


void latencytime::visit(seq_node& n) {
    res = n.servicetime;
}

void latencytime::visit(comp_node& n) {
    res = compute(*this, n, std::plus<double>());
}

void latencytime::visit(pipe_node& n) {
    res = compute(*this, n, std::plus<double>());
}

void latencytime::visit(farm_node& n) {
    res = (*this)( *n.get(0) );
}

void latencytime::visit(map_node& n) {
    res = (*this)( *n.get(0) ) / n.pardegree;
}

// TODO WRONG -> for reduce is important the dim of input
void latencytime::visit(reduce_node& n) {
    res = log2( (*this)( *n.get(0) ) );
}

void latencytime::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(n);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

string latencytime::print( skel_node& sk ){
    return std::to_string( (*this)( sk ) );
}

double latencytime::operator()(skel_node& sk){
    sk.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

// TODO here the stream size is something "global"
// user should set it through some shell verbs
// like "set_dimension(_)" and get with "get_dimension"
// better ideas?

completiontime::completiontime( rpl_environment& env ) :
    lat( env ), ts( env ), env( env )
{}

void completiontime::visit( seq_node& n ) {
    res = n.servicetime;
}

void completiontime::visit( comp_node& n ) {
    res = compute(*this, n, std::plus<double>());
}

void completiontime::visit( pipe_node& n ) {
    /* latency of the first stage + ts*dimension */
    res = lat(*n.get(0)) + env.get_dim() * ts(n);
}

void completiontime::visit( farm_node& n ) {
    res = env.get_dim() / n.pardegree * lat(n);
}

void completiontime::visit( map_node& n ) {
    res = env.get_dim() / n.pardegree * ts(n);
}

void completiontime::visit( reduce_node& n ) {
    res = lat(n);
}

void completiontime::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(n);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

string completiontime::print( skel_node& n ){
    return std::to_string( (*this)( n ));;
}

double completiontime::operator()(skel_node& n){
    n.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

pardegree::pardegree( rpl_environment& env ) :
    env( env )
{}

void pardegree::visit( seq_node& n ) {
    res = 1;
}

void pardegree::visit( comp_node& n ) {
    res = compute(*this, n, [](double a, double b){return std::max(a, b);});
}

void pardegree::visit( pipe_node& n ) {
    res = compute(*this, n, std::plus<size_t>());
}

void pardegree::visit( farm_node& n ) {
    res = n.pardegree * (*this)(*n.get(0));
}

void pardegree::visit( map_node& n ) {
    res = n.pardegree * (*this)(*n.get(0));
}

// TODO also reduce should be have a configurable
// parallelism degree like farm and map!!
void pardegree::visit( reduce_node& n ) {
    res = 1;
}

void pardegree::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(n);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

string pardegree::print( skel_node& n ){
    return std::to_string( (*this)( n ) );;
}

size_t pardegree::operator()(skel_node& n){
    n.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

resources::resources( rpl_environment& env ) :
    env( env )
{}

void resources::visit( seq_node& n ) {
    res = 1;
}

void resources::visit( comp_node& n ) {
    res = compute(*this, n, [](size_t a, size_t b){return std::max(a, b);});
}

void resources::visit( pipe_node& n ) {
    res = compute(*this, n, std::plus<size_t>());
}

void resources::visit( farm_node& n ) {
    res = n.pardegree * (*this)(*n.get(0)) + 2;  //emitter + collector
}

void resources::visit( map_node& n ) {
    res = n.pardegree * (*this)(*n.get(0)) + 2;  //scatter + gather
}

// TODO also reduce should be have a configurable
// parallelism degree like farm and map!!
void resources::visit( reduce_node& n ) {
    res = 1;
}

void resources::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(n);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

string resources::print( skel_node& n ){
    return std::to_string( (*this)( n ) );;
}

size_t resources::operator()(skel_node& n){
    n.accept(*this);
    return res;
}

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
