#include "interpreter.hpp"

// explicit instantiations
typedef environment<string, shared_ptr<skel_node>> env_t;
typedef dispatcher<string, skel_visitor> disp_t;
typedef interpreter<env_t, disp_t> interpr_t;
template struct environment<string, shared_ptr<skel_node>>;
template struct dispatcher<string, skel_visitor>;
template struct interpreter<env_t, disp_t>;

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the dispatcher
//
///////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
shared_ptr<V> dispatcher<K,V>::operator[](const K& id) {
    return dispatch.at(id);
}

template <typename K, typename V>
void dispatcher<K, V>::add(const K& id, shared_ptr<V> value) {
    dispatch.insert(std::pair<K, shared_ptr<V>>(id, value));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the interpreter
//
///////////////////////////////////////////////////////////////////////////////
template <typename Env, typename Disp>
interpreter<Env, Disp>::interpreter(Env& env, Disp& dispatch, error_container& err_repo)
: env(env), dispatch(dispatch), err_repo(err_repo), success(true) {}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(assign_node& n) {
    auto skptr = n.rvalue;            // skptr cannot be null here
    skptr->accept(*this);             // recurse for semantic check in skel tree
    if (success)
        env.put(n.id, n.rvalue);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(show_node& n) {
    try {
        // dispatch, monamour
        skel_node& sn = *(env.get(n.id));
        (*dispatch[ n.prop ])(sn);
        (*dispatch[ n.prop ]).print();
    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(set_node& n) {

}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(ann_node& n) {

}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(rwr_node& n) {

}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(opt_node& n) {

}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(seq_node& n) {
    if (n.get(0) != nullptr)
        n.get(0)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(comp_node& n) {
    for (int i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(pipe_node& n) {
    for (int i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(farm_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(map_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(reduce_node& n) {
    n.get(0)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(id_node& n) {
    try {
        // check if it exists
        env.get(n.id);
    } catch (out_of_range& e) {
        success  = false;
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}
