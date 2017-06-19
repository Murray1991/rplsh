#include "annotators.hpp"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

ann_visitor::ann_visitor( rpl_environment& env ) :
    env ( env )
{}

void ann_visitor::visit( seq_node& n ) {
    result = false;
}

void ann_visitor::visit( comp_node& n ) {
    result = false;
}

void ann_visitor::visit( pipe_node& n ) {
    result = false;
}

void ann_visitor::visit( farm_node& n ) {
    result = false;
}

void ann_visitor::visit( map_node& n ) {
    result = false;
}

void ann_visitor::visit( reduce_node& n ) {
    result = false;
}

void ann_visitor::visit( id_node& n ) {
    result = false;
}

///////////////////////////////////////////////////////////////////////////////

ann_servicetime::ann_servicetime( rpl_environment& env ) :
    ann_visitor ( env )
{}

void ann_servicetime::visit( seq_node& n ) {
    n.servicetime = servicetime;
    result        = true;
}

bool ann_servicetime::operator()( skel_node& n, double value ) {
    servicetime = value;
    n.accept( *this );
    return result;
}

///////////////////////////////////////////////////////////////////////////////

ann_pardegree::ann_pardegree( rpl_environment& env ) :
    ann_visitor ( env )
{}

void ann_pardegree::visit( farm_node& n ) {
    n.pardegree = nw;
    result      = true;
}

void ann_pardegree::visit( map_node& n ) {
    n.pardegree = nw;
    result      = true;
}

bool ann_pardegree::operator()( skel_node& n, double value ) {
    this->nw = value;
    n.accept( *this );
    return result;
}

///////////////////////////////////////////////////////////////////////////////

ann_datap::ann_datap( rpl_environment& env ) :
    ann_visitor ( env )
{}

void ann_datap::visit( seq_node& n ) {
    n.datap_flag = datap;
    result       = true;
}

bool ann_datap::operator()( skel_node& n, double value ) {
    this->datap = value == 0 ? false : true;
    n.accept( *this );
    return result;
}
