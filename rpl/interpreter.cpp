#include "interpreter.hpp"
#include "rewriter.hpp"

using namespace std;

typedef environment<string, skel_node> env_t;
template struct interpreter<env_t>;
single_node_cloner snc;

///////////////////////////////////////////////////////////////////////////////

void unrank2rank( skel_node& n )
{
    // 2-rank the children
    for ( size_t i = 0; i < n.size(); i++)
        unrank2rank( *n.get(i) );

    // 2-rank the current node
    while (n.size() > 2) {
        skel_node* newnode = snc(n);
        skel_node* last2 = n.pop();
        skel_node* last1 = n.pop();
        newnode->add(last1);
        newnode->add(last2);
        n.add(newnode);
    }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Env>
interpreter<Env>::interpreter(Env& env, error_container& err_repo)
    : env(env), err_repo(err_repo), vdispatch(env), success(true) {}

template <typename Env>
void interpreter<Env>::visit(assign_node& n) {
    n.rvalue->accept(*this);                // recurse for semantic check in skel tree
    if (success) {
        unrank2rank(*n.rvalue);
        env.put(n.id, n.rvalue);
    }
    else
        delete n.rvalue;
}

template <typename Env>
void interpreter<Env>::visit(show_node& n) {
    try {

        auto range = env.range( n.id );
        auto& shower = *vdispatch[ n.prop ];
        for (auto it = range.first; it != range.second; it++) {
            skel_node& sn = *it->second;
            shower.print( sn );
        }

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

template <typename Env>
void interpreter<Env>::visit(set_node& n) {

}

template <typename Env>
void interpreter<Env>::visit(ann_node& n) {

}

template <typename Env>
void interpreter<Env>::visit(rwr_node& n) {
    try {

        string id = n.id;
        string rule = n.prop;
        auto p = env.range(id);
        node_set _set;
        rewriter _rewriter;
        _set = ( rule == "allrules" ) ?
            _rewriter.apply_allrules( p.first, p.second, rdispatch) :
            _rewriter.apply_rule( p.first, p.second, *rdispatch[ rule ]);

        env.clear( id );
        for ( auto& p : _set )
            env.add( id, p.second );

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

template <typename Env>
void interpreter<Env>::visit(opt_node& n) {

}

template <typename Env>
void interpreter<Env>::visit(seq_node& n) {
    if (n.get(0) != nullptr)
        n.get(0)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(comp_node& n) {
    for (size_t i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(pipe_node& n) {
    for (size_t i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(farm_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(map_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(reduce_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env>
void interpreter<Env>::visit(id_node& n) {
    try {
        auto ptr = env.get(n.id);                 // check if it exists
    } catch (out_of_range& e) {
        success  = false;
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}
