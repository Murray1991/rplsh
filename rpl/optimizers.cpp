#include "optimizers.hpp"
#include "utils/rank.hpp"
#include <tuple>
#include <cmath>

using namespace std;

/* implementation of these functions is at the end */
tuple<int, int, int> longestrun( const pipe_node& n, servicetime& ts );
void pipe_merge( pipe_node&n, size_t left, size_t right );
pair<size_t, size_t> findmax( const skel_node& n, servicetime& ts );

///////////////////////////////////////////////////////////////////////////////

void optrule::visit( seq_node& n ) {
}

void optrule::visit( comp_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)( *n.get(i) );
}

void optrule::visit( pipe_node& n ) {
    for (size_t i = 0; i < n.size(); i++)
        (*this)( *n.get(i) );
}

void optrule::visit( farm_node& n ) {
    (*this)( *n.get(0) );
}

void optrule::visit( map_node& n ) {
    (*this)( *n.get(0) );
}

void optrule::visit( reduce_node& n ) {
    (*this)( *n.get(0) );
}

void optrule::visit( id_node& n ) {
    // TODO apply optrule recursively or not?
}

optrule::optrule( rpl_environment& env ) :
    env( env ),
    ts( env )
{}

///////////////////////////////////////////////////////////////////////////////

farmopt::farmopt( rpl_environment& env ) :
    optrule(env)
{}

void farmopt::visit( farm_node& n ) {
    (*this)( *n.get(0) );
    double tw = ts( *n.get(0) );
    double te = env.get_emitter_time();
    size_t nw = ceil( tw / te );
    n.pardegree = nw;
}

void farmopt::operator()( skel_node& sk ) {
    sk.accept( *this );
}

///////////////////////////////////////////////////////////////////////////////

pipeopt::pipeopt( rpl_environment& env ) :
    optrule( env ), balance( env )
{}

void pipeopt::visit( pipe_node& n ) {
    skel_node* ptr = &n;
    ranktounrank(ptr, ptr);

    for ( size_t i = 0; i < n.size(); i++ ) {
        (*this)( *n.get(i) );                   // optimize sub pipes
        balance( n );                           // balance the stages
    }

    auto run = longestrun(n, ts);
    while (get<2>(run) - get<1>(run) > 0 ) {
        pipe_merge(n, get<1>(run), get<2>(run));
        run = longestrun(n, ts);
    }

    single_node_cloner snc;
    unranktorank2(*ptr, snc);

}

void pipeopt::operator()( skel_node& n ) {
    n.accept( *this );
}

///////////////////////////////////////////////////////////////////////////////

pipebalance::pipebalance( rpl_environment& env ) :
    optrule(env)
{}

void pipebalance::visit( pipe_node& n ) {
    auto p = findmax( n, ts );
    size_t idx = p.first;
    size_t max = p.second;
    for (size_t i = 0; i < n.size(); i++)
        if (i != idx)
            (*this)(*n.get(i), max);
}

void pipebalance::visit( farm_node& n ) {
    double tw  = ts( *n.get(0) );
    n.pardegree = ceil( tw / ts_max );
}

void pipebalance::visit( map_node& n ) {
    double tw  = ts( *n.get(0) );
    n.pardegree = ceil( tw / ts_max );
}

void pipebalance::operator()( skel_node& n ) {
    n.accept(*this);
}

void pipebalance::operator()( skel_node& n, double max ) {
    ts_max = max;
    n.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

// find the index of the heaviest stage of the pipeline/sequence
pair<size_t, size_t> findmax( const skel_node& n, servicetime& ts ) {
    size_t idx = 0;
    size_t max = 0;
    for ( size_t i = 0; i < n.size(); i++ ) {
        size_t tmp = ts( *n.get(i) );
        if ( max <= tmp ) {
            idx = i;
            max = tmp;
        }
    }
    return { idx, max };
}

/* find the longest subarray where the sum of its elements
   is bounded to the max stage value of the pipeline */
tuple<int, int, int> longestrun( const pipe_node& n, servicetime& ts )
{
    // find threshold and fill values vector
    size_t thresh = 0;
    std::vector<size_t> values;
    for ( size_t i = 0; i < n.size(); i++ ) {
        size_t value = ts(*n.get(i));
        thresh = thresh < value ? value : thresh;
        values.push_back(value);
    }

    // find the max run such that the sum of its element is < thresh
    size_t sum = 0;
    tuple<size_t,size_t,size_t> max = make_tuple(0, 0, 0);
    tuple<size_t,size_t,size_t> cur = make_tuple(0, 0, 0);
    for ( size_t i = 0; i < values.size(); i++ ) {
        if ( sum + values[i] <= thresh ) {
            sum += values[i];
            cur = make_tuple( sum, get<1>(cur), i );
        } else {
            sum = values[i];
            cur = make_tuple( sum, i, i );
        }
        if (get<2>(max)-get<1>(max) <= get<2>(cur) - get<1>(cur))
            max = cur;
    }

    return max;
}

void pipe_merge( pipe_node&n, size_t left, size_t right ) {

    std::vector<skel_node*> rnodes; // nodes at right
    std::vector<skel_node*> mnodes; // nodes between left and right
    while ( n.size() != right+1 )
        rnodes.push_back(n.pop());
    while ( n.size() != left)
        mnodes.push_back(n.pop());

    skel_node* comp = new comp_node{};
    for (auto rit = mnodes.rbegin(); rit != mnodes.rend(); rit++)
        comp->add( *rit );

    n.add(comp);
    for (auto rit = rnodes.rbegin(); rit != rnodes.rend(); rit++)
        n.add( *rit );

}
