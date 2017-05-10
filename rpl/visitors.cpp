#include "visitors.hpp"
#include "skeletons.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

// some template instantiations
typedef environment<std::string, skel_node> env_t;
template struct servicetime<env_t>;
template struct latencytime<env_t>;

///////////////////////////////////////////////////////////////////////////////

template <typename Visitor, typename BinaryOp>
double compute(Visitor& v, skel_node& n, BinaryOp op) {
    double ts = 0;
    for (size_t i = 0; i < n.size() ; i++)
        ts = op(ts, v(*n.get(i)));
    return ts;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Env>
servicetime<Env>::servicetime(Env& env)
    : env(env) {}

template <typename Env>
void servicetime<Env>::visit(seq_node& n) {
    res = n.servicetime;
}

template <typename Env>
void servicetime<Env>::visit(comp_node& n) {
    res = compute(*this, n, std::plus<double>());
}

template <typename Env>
void servicetime<Env>::visit(pipe_node& n) {
    res = compute(*this, n, [](double a, double b){return std::max(a, b);});
}

template <typename Env>
void servicetime<Env>::visit(farm_node& n) {
    res = (*this)(*n.get(0)) / n.pardegree;
}

template <typename Env>
void servicetime<Env>::visit(map_node& n) {
    res = (*this)(*n.get(0)) / n.pardegree;
}

// TODO WRONG -> for reduce is important the dim of input
template <typename Env>
void servicetime<Env>::visit(reduce_node& n) {
    res = log2( (*this)(*n.get(0)) );
}

template <typename Env>
void servicetime<Env>::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(*ptr);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

template <typename Env>
void servicetime<Env>::print( skel_node& sk ){
    cout << (*this)( sk ) << endl;;
}

template <typename Env>
double servicetime<Env>::operator()(skel_node& sk){
    sk.accept(*this);
    return res;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Env>
latencytime<Env>::latencytime(Env& env)
    : env( env ) {}

template <typename Env>
void latencytime<Env>::visit(seq_node& n) {
    res = n.servicetime;
}

template <typename Env>
void latencytime<Env>::visit(comp_node& n) {
    res = compute(*this, n, std::plus<double>());
}

template <typename Env>
void latencytime<Env>::visit(pipe_node& n) {
    res = compute(*this, n, std::plus<double>());
}

template <typename Env>
void latencytime<Env>::visit(farm_node& n) {
    res = (*this)( *n.get(0) );
}

template <typename Env>
void latencytime<Env>::visit(map_node& n) {
    res = (*this)( *n.get(0) ) / n.pardegree;
}

// TODO WRONG -> for reduce is important the dim of input
template <typename Env>
void latencytime<Env>::visit(reduce_node& n) {
    res = log2( (*this)( *n.get(0) ) );
}

template <typename Env>
void latencytime<Env>::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        res = (*this)(n);
    } catch (out_of_range& e) {
        //TODO handle in a better way
        cout << "error, not found: " << n.id << endl;
    }
}

template <typename Env>
void latencytime<Env>::print( skel_node& sk ){
    cout << (*this)( sk ) << endl;;
}

template <typename Env>
double latencytime<Env>::operator()(skel_node& sk){
    sk.accept(*this);
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

void printer::print( skel_node& sk ){
    cout << (*this)( sk ) << endl;
}

string printer::operator()(skel_node& sk){
    res.clear();
    sk.accept(*this);
    return res;
}
