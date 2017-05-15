#include "optimizers.hpp"
#include <cmath>

using namespace std;

farmopt::farmopt(rpl_environment& env) :
    env( env ),
    ts( env )
{}

void farmopt::visit( seq_node& n ) {
}

void farmopt::visit( comp_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)( *n.get(i) );
}

void farmopt::visit( pipe_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)( *n.get(i) );
}

void farmopt::visit( farm_node& n ) {
    (*this)( *n.get(0) );
    double te = env.get_emitter_time();
    size_t nw = ceil( ts(n) / te );
    n.pardegree = nw;
}

void farmopt::visit( map_node& n ) {
    (*this)( *n.get(0) );
}

void farmopt::visit( reduce_node& n ) {
    (*this)( *n.get(0) );
}

void farmopt::visit( id_node& n ) {
    // TODO apply farmopt recursively  or not?
}

void farmopt::operator()( skel_node& sk ) {
    sk.accept( *this );
}
