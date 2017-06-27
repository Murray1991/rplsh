#include "ffmapper.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

ffmapper::ffmapper(rpl_environment& env) :
    env(env),
    res(env)
{}

void ffmapper::visit(seq_node& n) {
    size_t id = startID++ % endID;
    mw.push_back(id);
}

void ffmapper::visit(source_node& n) {
    size_t id = startID++ % endID;
    mw.push_back(id);
}

void ffmapper::visit(drain_node& n) {
    size_t id = startID++ % endID;
    mw.push_back(id);
}

void ffmapper::visit(comp_node& n) {
    size_t _startID = startID;
    size_t _endID   = startID + res(n);
    for (size_t i = 0; i < n.size(); i++) {
        startID = _startID;
        n.get(i)->accept(*this);
    }
    startID = _endID;
}

void ffmapper::visit(pipe_node& n) {
    for (size_t i = 0; i < n.size(); i++)
        n.get(i)->accept(*this);
}

void ffmapper::visit(farm_node& n) {
    // emitter
    mw.push_back(startID++ % endID);

    // recurse n.pardegree times for assign cpu ids to the workers
    for (int i = 0; i < n.pardegree; i++)
        n.get(0)->accept(*this);

    // collector
    mw.push_back(startID++ % endID);
}

// TODO check if it actually works...
void ffmapper::visit(map_node& n) {
    // scatter/emitter
    mw.push_back(startID++ % endID);

    // recurse n.pardegree times for assign cpu ids to the workers
    for (int i = 0; i < n.pardegree; i++)
        n.get(0)->accept(*this);

    // gather (?) <-- I don't think it's present for parallel_for/parallel_reduce...
    mw.push_back(startID++ % endID);
}

// TODO check if it actually works...
void ffmapper::visit(reduce_node& n) {
    // scatter/emitter (?)
    mw.push_back(startID++ % endID);

    // recurse n.pardegree times for assign cpu ids to the workers
    for (int i = 0; i < n.pardegree; i++)
        n.get(0)->accept(*this);

}

void ffmapper::visit(id_node& n) {
    auto ptr = env.get(n.id, n.index);
    if (ptr != nullptr)
        ptr->accept(*this);
    else
        cout << n.id << " whaaaat?" << endl;
}

// clear member vector mw
void ffmapper::clear() {
    mw.clear();
}

vector<size_t> ffmapper::get_worker_mapping() const {
    return mw;
}

const ffmapper& ffmapper::operator()(skel_node& n){
    this->startID = 0;
    this->endID   = std::min(res(n), env.get_res());
    n.accept(*this);
    return *this;
}
