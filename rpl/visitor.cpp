#include "visitor.hpp"
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the servicetime visitor
//
///////////////////////////////////////////////////////////////////////////////
void servicetime::visit(seq_node& n) {
}

void servicetime::visit(comp_node& n) {
}

void servicetime::visit(pipe_node& n) {
}

void servicetime::visit(farm_node& n) {
}

void servicetime::visit(map_node& n) {
}

void servicetime::visit(reduce_node& n) {
}

void servicetime::visit(id_node& n) {
    cout << n.identifier;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the printer
//
///////////////////////////////////////////////////////////////////////////////
template<typename Iterator>
void printer::print(const string& name, Iterator begin, Iterator end) {
    cout << name << "(";
    for (auto it = begin ; it != end; it++) {
        (*it)->accept(*this);
        cout << (it != end-1 ? "," : "");
    }
    cout << ")";
}

void printer::visit(seq_node& n) {
    print("seq", n.begin(), n.end());
}

void printer::visit(comp_node& n) {
    print("comp", n.begin(), n.end());
}

void printer::visit(pipe_node& n) {
    print("pipe", n.begin(), n.end());
}

void printer::visit(farm_node& n) {
    print("farm", n.begin(), n.end());
}

void printer::visit(map_node& n) {
    print("map", n.begin(), n.end());
}

void printer::visit(reduce_node& n) {
    print("reduce", n.begin(), n.end());
}

void printer::visit(id_node& n) {
    cout << n.identifier;
}
