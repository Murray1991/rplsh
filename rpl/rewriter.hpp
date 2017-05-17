#pragma once

#include "rewrules.hpp"
#include "visitors.hpp"
#include <unordered_map>

using namespace std;

typedef unordered_map<string, skel_node*>   node_set;
typedef pair<skel_node*, skel_node*>        pair_node;

void combine( skel_node& root, vector<pair_node>&& pairs, node_set& to_set );
void combine (skel_node& root, node_set&& from_set, node_set& to_set);
vector<pair_node> allpairs( node_set&& s1, node_set&& s2);
node_set merge(node_set& s1, node_set& s2);
void delset( node_set& s );

struct rewriter {
    node_set apply_allrules ( skel_node& tree, rr_dispatcher& rr_disp);
    template <typename Iterator>
    node_set apply_allrules ( Iterator& it1, Iterator& it2, rr_dispatcher& rr_disp);
    template <typename Iterator>
    node_set apply_rule ( Iterator& it1, Iterator& it2, rewrule& r);

private:
    printer print;
    single_node_cloner snc;
    skel_node* rewrite( skel_node& n, rewrule& r );
    node_set fullrecrewrite( skel_node& n, rewrule& r );
    void insert_or_delete( node_set& set, skel_node* rn );
};

template <typename Iterator>
node_set rewriter::apply_rule ( Iterator& begin, Iterator& end, rewrule& r) {
    node_set set;
    for ( auto it = begin ; it != end; ++it) {
        auto& skelptr = *it;
        insert_or_delete ( set, rewrite(*skelptr, r) );
    }
    return set;
}

node_set rewriter::apply_allrules ( skel_node& tree, rr_dispatcher& rr_disp) {
    node_set set;;
    vector<node_set> sets;
    for (auto& p : rr_disp.get_map())
        sets.push_back( fullrecrewrite(tree, *p.second) );
    for (auto& curr : sets)
        set = merge(set, curr);
    return set;
}

template <typename Iterator>
node_set rewriter::apply_allrules ( Iterator& begin, Iterator& end, rr_dispatcher& rr_disp) {
    node_set set;
    vector<node_set> sets;
    for (auto it = begin; it != end; it++) {
        auto& skelptr = *it;
        sets.push_back( apply_allrules(*skelptr, rr_disp) );
    }
    for (auto& curr : sets)
        set = merge(set, curr);
    return set;
}

skel_node* rewriter::rewrite( skel_node& n, rewrule& r ) {
    return r.rewrite(n);
}

node_set rewriter::fullrecrewrite( skel_node& n, rewrule& r ) {
    node_set set;
    insert_or_delete( set, n.clone() );
    insert_or_delete( set, rewrite(n, r) );
    if ( n.size() == 1 )
        combine(n, fullrecrewrite(*n.get(0), r), set);
    else if ( n.size() == 2 )
        combine(n, allpairs(fullrecrewrite(*n.get(0),r), fullrecrewrite(*n.get(1), r)), set);
    return set;
}

void rewriter::insert_or_delete( node_set& set, skel_node* rn ) {
    if (rn != nullptr) {
        auto p = set.insert({print(*rn), rn});
        if (!p.second)
            delete rn;
    }
}

template <typename ... T>
void comb_and_ins( node_set& s, skel_node& n, T* ... args ) {
    single_node_cloner snc;
    skel_node * rn = snc(n);
    printer print;
    (rn->add(args) , ...);
    auto p = s.insert({print(*rn), rn});
    if (!p.second)
        delete rn;
}

void combine( skel_node& root, vector<pair_node>&& pairs, node_set& to_set ) {
    for (auto& p : pairs)
        comb_and_ins(to_set, root, p.first, p.second);
}

void combine (skel_node& root, node_set&& from_set, node_set& to_set) {
    for (auto& it : from_set)
        comb_and_ins(to_set, root, it.second);
}

void delset( node_set& s ) {
    for (auto& it : s )
        delete it.second;
}

node_set merge(node_set& s1, node_set& s2) {
    vector<skel_node*> nodes;
    for (auto& p : s2) {
        auto o = s1.insert(p);
        if (!o.second)
            delete p.second;
    }
    return s1;
}

//TODO don't clone and avoid memory leaks...
vector<pair_node> allpairs( node_set&& s1, node_set&& s2) {
    vector<pair_node> pairs;
    for (auto& it1 : s1 )
        for (auto& it2 : s2)
            pairs.push_back(make_pair(it1.second->clone(), it2.second->clone()));
    delset(s1);
    delset(s2);
    return pairs;
}
