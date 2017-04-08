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
void printer::print(const string& name, const access_node& n) {
    cout << name << "(";
    for (int i = 0; i < n.size(); i++) {
        n.get(i)->accept(*this);
        cout << (i != n.size()-1 ? "," : "");
    }
    cout << ")";
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
    cout << n.identifier;
}
