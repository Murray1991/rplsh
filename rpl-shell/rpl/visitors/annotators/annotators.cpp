#include "annotators.hpp"
#include "nodes/verbs.hpp"
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

ann_visitor::ann_visitor( rpl_environment& env ) :
    env ( env )
{}

void ann_visitor::visit( seq_node& n ) {
    result = false;
}

void ann_visitor::visit( source_node& n ) {
    result = false;
}

void ann_visitor::visit( drain_node& n ) {
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

void ann_servicetime::visit( source_node& n ) {
    n.servicetime = servicetime;
    result        = true;
}

void ann_servicetime::visit( drain_node& n ) {
    n.servicetime = servicetime;
    result        = true;
}

bool ann_servicetime::operator()( skel_node& n, ann_node& a ) {
    if (a.value < 0) {
        cout << "warning: bad annotation value" << endl;
        return false;
    }
    servicetime = a.value;
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

bool ann_pardegree::operator()( skel_node& n, ann_node& a ) {
    if (a.value < 0) {
        cout << "warning: bad annotation value" << endl;
        return false;
    }
    this->nw = a.value;
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

bool ann_datap::operator()( skel_node& n, ann_node& a ) {
    this->datap = a.value == 0 ? false : true;
    n.accept( *this );
    return result;
}

///////////////////////////////////////////////////////////////////////////////

ann_typein::ann_typein( rpl_environment& env ) :
    ann_visitor ( env )
{}

void ann_typein::visit( seq_node& n ) {
    n.typein = type;
    result   = true;
}

void ann_typein::visit( drain_node& n ) {
    n.typein = type;
    result   = true;
}

bool ann_typein::operator()( skel_node& n, ann_node& a ) {
    type = a.word;
    n.accept(*this);
    return result;
}

///////////////////////////////////////////////////////////////////////////////

ann_typeout::ann_typeout( rpl_environment& env ) :
    ann_visitor ( env )
{}

void ann_typeout::visit( seq_node& n ) {
    n.typeout = type;
    result    = true;
}

void ann_typeout::visit( source_node& n ) {
    n.typeout = type;
    result    = true;
}

bool ann_typeout::operator()( skel_node& n, ann_node& a ) {
    type = a.word;
    n.accept(*this);
    return result;
}
