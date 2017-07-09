#include "evaluators.hpp"
#include "nodes/skeletons.hpp"
#include "visitors/visitors.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <tuple>
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

eval_visitor::eval_visitor(rpl_environment& env) :
    env( env )
{}

///////////////////////////////////////////////////////////////////////////////

servicetime::servicetime(rpl_environment& env) :
    eval_visitor( env )
{}

void servicetime::visit(seq_node& n) {
    if (n.datap_flag)
        res = n.servicetime * n.inputsize;
    else
        res = n.servicetime;
}

void servicetime::visit(source_node& n) {
    res = n.servicetime;
}

void servicetime::visit(drain_node& n) {
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
    // assumption: inputsize has been already propagated
    res = (*this)(*n.get(0));
}

void servicetime::visit(reduce_node& n) {
    // assuming Tf is the servicetime  of the reduce function f:
    // ts(n) = Tf / nw + log2(nw) * Tf

    // assumption: inputsize has been already propagated
    int nw = n.pardegree;
    res = (*this)(*n.get(0));                   // res == Tf / nw
    res = res + log2(n.pardegree) * (res/nw);   // Tf == res*nw
}

void servicetime::visit(id_node& n) {
    try {
        // we need to assign resources before
        auto ptr = env.get(n.id, n.index);
        assign_resources assignres;
        assignres(*ptr, n.inputsize);
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

latencytime::latencytime(rpl_environment& env) :
    eval_visitor( env )
{}


void latencytime::visit(seq_node& n) {
    if (n.datap_flag)
        res = n.servicetime * n.inputsize;
    else
        res = n.servicetime;
}

void latencytime::visit(source_node& n) {
    res = n.servicetime;
}

void latencytime::visit(drain_node& n) {
    res = n.servicetime;
}

void latencytime::visit(comp_node& n) {
    res = compute(*this, n, std::plus<double>());
}

void latencytime::visit(pipe_node& n) {
    res = compute(*this, n, std::plus<double>());
}

void latencytime::visit(farm_node& n) {
    res = env.get_emitter_time() + (*this)( *n.get(0) ) + env.get_collector_time();
}

void latencytime::visit(map_node& n) {
    res = env.get_scatter_time() + (*this)( *n.get(0) ) + env.get_gather_time();
}

void latencytime::visit(reduce_node& n) {
    // assuming Tf is the servicetime  of the reduce function f:
    // lat(n) = Tf / nw + log2(nw) * Tf

    // assumption: inputsize has been already propagated
    int nw = n.pardegree;
    res = (*this)(*n.get(0));                   // res == Tf / nw
    res = res + log2(n.pardegree) * (res/nw);   // Tf == res*nw
}

void latencytime::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id, n.index);
        assign_resources assignres;
        assignres(*ptr, n.inputsize);
        res = (*this)(*ptr);
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
    eval_visitor( env ),
    lat( env ),
    ts( env )
{}

void completiontime::visit( seq_node& n ) {
    if (n.datap_flag)
        res = n.servicetime * n.inputsize;
    else
        res = n.servicetime;

    res = res * env.get_dim();
}

void completiontime::visit(source_node& n) {
    res = n.servicetime * env.get_dim();
}

void completiontime::visit(drain_node& n) {
    res = n.servicetime * env.get_dim();
}

void completiontime::visit( comp_node& n ) {
    res = compute(*this, n, std::plus<double>());
}

void completiontime::visit( pipe_node& n ) {
    /* latency of the first stage + ts*dimension */
    res = lat(n) + env.get_dim() * ts(n);
}

void completiontime::visit( farm_node& n ) {
    // latency + ts
    res = (env.get_dim()-1) * ts(n) + lat(n);
}

void completiontime::visit( map_node& n ) {
    res = env.get_dim() * lat(n);
}

void completiontime::visit( reduce_node& n ) {
    res = env.get_dim() * lat(n);
}

void completiontime::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id, n.index);
        assign_resources assignres;
        assignres(*ptr, n.inputsize);
        res = (*this)(*ptr);
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
    eval_visitor( env )
{}

void pardegree::visit( seq_node& n ) {
    res = 1;
}

void pardegree::visit( source_node& n ) {
    res = 1;
}

void pardegree::visit( drain_node& n ) {
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
    res = n.pardegree * (*this)(*n.get(0));
}

void pardegree::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id, n.index);
        res = (*this)(*ptr);
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
    eval_visitor( env )
{}

void resources::visit( seq_node& n ) {
    res = 1;
}

void resources::visit( source_node& n ) {
    res = 1;
}

void resources::visit( drain_node& n ) {
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

void resources::visit( reduce_node& n ) {
    res = n.pardegree * (*this)(*n.get(0));
}

void resources::visit( id_node& n ) {
    try {
        auto ptr = env.get(n.id, n.index);
        res = (*this)(*ptr);
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
