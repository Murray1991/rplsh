#include "skeletons.hpp"
#include "visitor_interface.hpp"

using namespace std;
///////////////////////////////////////////////////////////////////////////////

skel_node::~skel_node() {
    for (auto sk : children)
        delete sk;
}

skel_node* skel_node::get( size_t idx ) const {
    return idx < children.size() ? children[idx] : nullptr;
}

skel_node* skel_node::pop() {
    if (children.size() > 0) {
        skel_node* last = children.back();
        children.pop_back();
        return last;
    }
    return nullptr;
}

void skel_node::add( skel_node* sk ) {
    children.push_back(sk);
}

void skel_node::set( skel_node* sk, size_t pos) {
    //delete the old and then set
    delete children[pos];
    children[pos] =sk;
}

std::size_t skel_node::size() const {
    return children.size();
}

///////////////////////////////////////////////////////////////////////////////

template <typename skeleton>
void concrete_skel_node<skeleton>::accept( visitor & v ) {
    v.visit(_sk);
}

template <typename skeleton>
concrete_skel_node<skeleton>::concrete_skel_node( skeleton& sk )
    : _sk(sk) {}

template <typename skeleton>
concrete_skel_node<skeleton>::concrete_skel_node( skeleton& sk, const skeleton& toclone )
    : _sk(sk) {

    for (size_t i = 0; i < toclone.size(); i++)
        add( toclone.get(i)->clone() );

}

///////////////////////////////////////////////////////////////////////////////

seq_node::seq_node ( double servicetime )
    : concrete_skel_node( *this ), servicetime(servicetime) {}

seq_node::seq_node( skel_node* pattexp, double servicetime )
    : concrete_skel_node( *this ), servicetime(servicetime) {
        add(pattexp);
}

seq_node::seq_node( const seq_node& other )
    : concrete_skel_node( *this, other ), servicetime(other.servicetime) {}

skel_node* seq_node::clone() {
    return new seq_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

comp_node::comp_node()
    : concrete_skel_node( *this ) {}

comp_node::comp_node( const comp_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node * comp_node::clone() {
    return new comp_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

pipe_node::pipe_node()
    : concrete_skel_node( *this ) {}

pipe_node::pipe_node( const pipe_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node * pipe_node::clone() {
    return new pipe_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

farm_node::farm_node( skel_node* pattexp, int pardegree )
        : concrete_skel_node( *this ), pardegree( pardegree ) {
    add(pattexp);
}

farm_node::farm_node(const farm_node& other)
        : concrete_skel_node( *this, other ), pardegree(other.pardegree) {}

skel_node* farm_node::clone() {
    return new farm_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

map_node::map_node(skel_node* pattexp, int pardegree)
        : concrete_skel_node( *this ), pardegree( pardegree ) {
    add(pattexp);
}

map_node::map_node(const map_node& other)
        : concrete_skel_node( *this, other ), pardegree(other.pardegree) {}

skel_node * map_node::clone() {
    return new map_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

reduce_node::reduce_node( skel_node* pattexp )
        : concrete_skel_node( *this ) {
    add(pattexp);
}

reduce_node::reduce_node( const reduce_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node* reduce_node::clone() {
    return new reduce_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

id_node::id_node( const string& id )
    : concrete_skel_node( *this ), id( id ) {}

id_node::id_node( const id_node& other )
    : concrete_skel_node( *this ), id( other.id ) {}

skel_node* id_node::clone() {
    return new id_node(*this);
}
