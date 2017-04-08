#include "interpreter.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the environment
//
///////////////////////////////////////////////////////////////////////////////
void environment::put(string& id, shared_ptr<skel_node> n)
{
    env[id] = move(n);
}

skel_node& environment::get(string& id)
{
    binding_map::iterator it = env.find(id);
    if (it == env.end())
        throw logic_error(id + " not found");
    return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Implementation of the interpreter
//
///////////////////////////////////////////////////////////////////////////////
interpreter::interpreter(environment& env, error_container& err_repo)
: env(env), err_repo(err_repo) {
    //register "commanders" to the map
    dispatch["show_default"] = make_shared<printer>();
    dispatch["servicetime"]  = make_shared<servicetime>();
}

void interpreter::visit(assign_node& n)
{
    skel_node& sk = *n.rvalue;
    sk.accept(*this);

    // TODO handle errors in interpretation
    env.put(n.id, move(n.rvalue));
}

string getname(string& s) {
    return s.empty() ? "show_default" : s;
}

void interpreter::visit(show_node& n)
{
    try {
        skel_node& sn = env.get(n.id);
        (*dispatch[ getname(n.prop) ])(sn);
    } catch (logic_error& e) {
        cout << e.what();
    }
    cout << endl;
}

void interpreter::visit(set_node& n)
{

}

void interpreter::visit(ann_node& n)
{

}

void interpreter::visit(rwr_node& n)
{

}

void interpreter::visit(opt_node& n)
{

}

void interpreter::visit(seq_node& n)
{
    cout << "seq" << endl;
}

void interpreter::visit(comp_node& n)
{
    cout << "comp" << endl;

}

void interpreter::visit(pipe_node& n)
{
    cout << "pipe\n";
}

void interpreter::visit(farm_node& n)
{
    cout << "farm\n";
}

void interpreter::visit(map_node& n)
{
    cout << "map\n";
}

void interpreter::visit(reduce_node& n)
{
    cout << "reduce\n";
}

void interpreter::visit(id_node& n)
{
    cout << "id: " << n.identifier << endl;
}
