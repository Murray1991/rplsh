#include "normalform.hpp"

normalform::normalform( rpl_environment& env ) :
    env( env ),
    success(true)
{}

void normalform::visit( seq_node& n ) {
    leaves.push_back( n.clone() );
}

void normalform::visit( comp_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        n.get(i)->accept(*this);
}

void normalform::visit( pipe_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        n.get(i)->accept(*this);
}

void normalform::visit( farm_node& n ) {
    n.get(0)->accept(*this);
}

void normalform::visit( map_node& n ) {
    success = false;
}

void normalform::visit( reduce_node& n ) {
    success = false;
}

void normalform::visit( id_node& n ) {
    auto p = env.range( n.id );
    skel_node& f = **p.first;
    if ( p.second - p.first != 1 )
        success = false;
    else if ( ! dynamic_cast<seq_node*>( &f ) )
        f.accept(*this);
    else
        leaves.push_back( f.clone() );
}

skel_node* normalform::operator()( skel_node& n ) {
    success = true;
    n.accept(*this);

    if ( !success ) {
        for (auto l : leaves) delete l;
        leaves.clear();
        return nullptr;
    }

    comp_node* c = new comp_node({});
    for (auto l : leaves)
        c->add(l);

    return new farm_node({c});
}
