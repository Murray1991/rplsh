#pragma once

// TODO funzia, ma un po' incasinato

#include "rewrules.hpp"
#include "nodes/skeletons.hpp"
#include "visitors/visitors.hpp"
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
    rewriter(bool rec) : rec(rec) {}
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

    /* When it is set to true, the rewrules are considered as  recrewrules:
     * the rewriter will apply the rule recursively modifying the same
     * skel tree */
    bool rec;
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
    for (auto& str : rr_disp.get_allrules())
        sets.push_back( fullrecrewrite(tree, *rr_disp[str]) );
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
    skel_node* newptr = r.rewrite(n);
    skel_node* tmp;

    /* for rec support */
    while ( rec && newptr && (tmp = r.rewrite(*newptr)) )
        newptr = tmp;

    /* TODO really lazy now, should be written in a more concise way */
    if ( rec && newptr && newptr->size() == 1) {
        newptr->set( rewrite(*newptr->get(0), r), 0 );
    } else if ( rec && newptr && newptr->size() == 2) {
        newptr->set( rewrite(*newptr->get(0), r), 0 );
        newptr->set( rewrite(*newptr->get(1), r), 1 );
    } else if ( rec && n.size() == 1 ) {
        n.set( rewrite(*n.get(0), r), 0 );
    } else if ( rec && n.size() == 2) {
        n.set( rewrite(*n.get(0), r), 0 );
        n.set( rewrite(*n.get(1), r), 1 );
    }

    return newptr == nullptr ? n.clone() : newptr;
}

node_set rewriter::fullrecrewrite( skel_node& n, rewrule& r ) {
    node_set set;
    unique_ptr<skel_node> rn( rewrite(n, r) );
    insert_or_delete( set, n.clone() );
    insert_or_delete( set, rn->clone() );

    if ( n.size() == 1 )
        combine(n, fullrecrewrite(*n.get(0), r), set);
    else if ( n.size() == 2 ) {                             // Comp or Pipe
        combine(n, allpairs(fullrecrewrite(*n.get(0),r), fullrecrewrite(*n.get(1), r)), set);
        if (rn != nullptr && rn->size() == 2)
            combine(*rn, allpairs(fullrecrewrite(*rn->get(0),r), fullrecrewrite(*rn->get(1), r)), set);
    }

    return set;
}

void rewriter::insert_or_delete( node_set& set, skel_node* rn ) {
    if (rn != nullptr) {
        auto p = set.insert({print(*rn), rn});
        if (!p.second)
            delete rn;
    }
}

void comb_and_ins( node_set& s, skel_node& n, skel_node* p1 ) {
    single_node_cloner snc;
    skel_node * rn = snc(n);
    printer print;
    rn->add(p1);
    auto p = s.insert({print(*rn), rn});
    if (!p.second)
        delete rn;
}

void comb_and_ins( node_set& s, skel_node& n, skel_node* p1, skel_node* p2 ) {
    single_node_cloner snc;
    skel_node * rn = snc(n);
    printer print;
    rn->add(p1);
    rn->add(p2);
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
