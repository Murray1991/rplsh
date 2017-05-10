#include "interpreter.hpp"

// explicit instantiations
typedef environment<string, skel_node> env_t;
typedef dispatcher<string, skel_visitor> disp_t;
typedef interpreter<env_t, disp_t> interpr_t;
template struct environment<string, skel_node>;
template struct dispatcher<string, skel_visitor>;
template struct interpreter<env_t, disp_t>;

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
    n.rvalue->accept(*this);                // recurse for semantic check in skel tree
    if (success)
        env.put(n.id, n.rvalue);
    else
        delete n.rvalue;
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(show_node& n) {
    try {
        // dispatch, monamour
        skel_node& sn = *(env.get(n.id));
        (*dispatch[ n.prop ]).print((sn));

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
    for (size_t i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

template <typename Env, typename Disp>
void interpreter<Env, Disp>::visit(pipe_node& n) {
    for (size_t i = 0;  i < n.size(); i++)
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
        auto ptr = env.get(n.id);                 // check if it exists
    } catch (out_of_range& e) {
        success  = false;
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}
