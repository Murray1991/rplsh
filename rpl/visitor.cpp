#include "visitor.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

typedef environment<string, shared_ptr<skel_node>> env_t;
template struct servicetime<env_t>;
template struct latencytime<env_t>;

template <typename BinaryOp>
void compute(double& res, skel_visitor& v, access_node& n, BinaryOp op) {
    double ts = 0;
    for (int i = 0; i < n.size() ; i++) {
        auto ptr = n.get(i);
        ptr->accept(v);
        cout << "---res " << res << endl;
        ts = op(ts,res);
        cout << "---ts " << ts << endl;
    }
    res = ts;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the servicetime visitor
//
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
    compute(res, *this, n, std::plus<double>());
}

template <typename Env>
void servicetime<Env>::visit(pipe_node& n) {
    compute(res, *this, n, [](double a, double b){return std::max(a, b);});
}

template <typename Env>
void servicetime<Env>::visit(farm_node& n) {
    n.get(0)->accept(*this);
    res = res / n.pardegree;
}

template <typename Env>
void servicetime<Env>::visit(map_node& n) {
    n.get(0)->accept(*this);
    res = res / n.pardegree;
}

// TODO WRONG. It should take as parameter the
// number of items and the time of the operation
template <typename Env>
void servicetime<Env>::visit(reduce_node& n) {
    n.get(0)->accept(*this);
    res = log2(res);
}

template <typename Env>
void servicetime<Env>::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        ptr->accept(*this);
    } catch (out_of_range& e) {
        cout << "error, not found: " << n.id << endl;
    }
}

template <typename Env>
void servicetime<Env>::print() {
    std::cout << res << std::endl;
}

template <typename Env>
const skel_visitor& servicetime<Env>::operator()(skel_node& sk){
    res = 0;
    sk.accept(*this);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the latencytime visitor
//
///////////////////////////////////////////////////////////////////////////////
template <typename Env>
latencytime<Env>::latencytime(Env& env)
    : env(env) {}

template <typename Env>
void latencytime<Env>::visit(seq_node& n) {
    res = n.servicetime;
}

template <typename Env>
void latencytime<Env>::visit(comp_node& n) {
    compute(res, *this, n, std::plus<double>());
}

template <typename Env>
void latencytime<Env>::visit(pipe_node& n) {
    compute(res, *this, n, std::plus<double>());
}

template <typename Env>
void latencytime<Env>::visit(farm_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env>
void latencytime<Env>::visit(map_node& n) {
    n.get(0)->accept(*this);
    res /= n.pardegree;             // it's equal to the servicetime
}

// TODO WRONG. It should take as parameter the
// number of items and the time of the operation
template <typename Env>
void latencytime<Env>::visit(reduce_node& n) {
    n.get(0)->accept(*this);
    res = log2(res);
}

template <typename Env>
void latencytime<Env>::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);
        ptr->accept(*this);
    } catch (out_of_range& e) {
        cout << "error, not found: " << n.id << endl;
    }
}

template <typename Env>
void latencytime<Env>::print() {
    std::cout << res << std::endl;
}

template <typename Env>
const skel_visitor& latencytime<Env>::operator()(skel_node& sk){
    res = 0;
    sk.accept(*this);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the printer
//  TODO: show pretty
//
///////////////////////////////////////////////////////////////////////////////
void printer::print(const string& name, const access_node& n) {
    res += name + "(";
    for (int i = 0; i < n.size(); i++) {
        n.get(i)->accept(*this);
        res += (i != n.size()-1 ? "," : "");
    }
    res += ")";
}

void printer::visit(seq_node& n) {
    print("seq", n);
}

void printer::visit(comp_node& n) {
    print("comp", n);
}

void printer::visit(pipe_node& n) {
    print("pipe", n);
}

void printer::visit(farm_node& n) {
    print("farm", n);
}

void printer::visit(map_node& n) {
    print("map", n);
}

void printer::visit(reduce_node& n) {
    print("reduce", n);
}

void printer::visit(id_node& n) {
    res += n.id;
}

void printer::print() {
    std::cout << res << std::endl;
}

const skel_visitor& printer::operator()(skel_node& sk){
    res.clear();
    sk.accept(*this);
    return *this;
}
