#include "skeletons.hpp"
#include "visitors/visitor_interface.hpp"

using namespace std;
///////////////////////////////////////////////////////////////////////////////

skel_node::skel_node( initializer_list<skel_node*> init )
    : children(init) {}

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
bool concrete_skel_node<skeleton>::match ( const skel_node& rhs ) {
    return dynamic_cast<const skeleton*>(&rhs);
}

template <typename skeleton>
bool concrete_skel_node<skeleton>::operator==( const skel_node& rhs ) {
    return match(rhs);
}

template <typename skeleton>
bool concrete_skel_node<skeleton>::operator!=( const skel_node& rhs ) {
    return !match(rhs);
}

template <typename skeleton>
concrete_skel_node<skeleton>::concrete_skel_node( skeleton& sk )
    : skel_node({}), _sk(sk) {}

template <typename skeleton>
concrete_skel_node<skeleton>::concrete_skel_node( skeleton& sk, const skeleton& toclone )
    : skel_node({}), _sk(sk) {

    for (size_t i = 0; i < toclone.size(); i++)
        add( toclone.get(i)->clone() );

}

template <typename skeleton>
concrete_skel_node<skeleton>::concrete_skel_node( skeleton& sk, initializer_list<skel_node*> init)
    : skel_node(init), _sk(sk) {}

///////////////////////////////////////////////////////////////////////////////

seq_node::seq_node( double servicetime, bool datap_flag ) :
    concrete_skel_node( *this ),
    servicetime(servicetime),
    datap_flag(datap_flag)
{}

seq_node::seq_node( std::string name, string typein, string typeout, string file ) :
    concrete_skel_node( *this ),
    servicetime(1.0),
    datap_flag(false),
    name(name),
    typein(typein),
    typeout(typeout),
    file(file)
{}

seq_node::seq_node( const seq_node& other ) :
    concrete_skel_node( *this, other ),
    servicetime(other.servicetime),
    datap_flag(other.datap_flag),
    name(other.name),
    typein(other.typein),
    typeout(other.typeout),
    file(other.file)
{}

skel_node* seq_node::clone() {
    return new seq_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

source_node::source_node( double servicetime ) :
    concrete_skel_node( *this ),
    servicetime(servicetime)
{}

source_node::source_node( string name, string typeout, string file ) :
    concrete_skel_node( *this ),
    servicetime(1.0),
    name(name),
    typeout(typeout),
    file(file)
{}

source_node::source_node( const source_node& other ) :
    concrete_skel_node( *this, other ),
    servicetime(other.servicetime),
    name(other.name),
    typeout(other.typeout),
    file(other.file)
{}

skel_node* source_node::clone() {
    return new source_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

drain_node::drain_node( double servicetime ) :
    concrete_skel_node( *this ),
    servicetime(servicetime)
{}

drain_node::drain_node( string name, string typein, string file ) :
    concrete_skel_node( *this ),
    servicetime(1.0),
    name(name),
    typein(typein),
    file(file)
{}

drain_node::drain_node( const drain_node& other ) :
    concrete_skel_node( *this, other ),
    servicetime(other.servicetime),
    name(other.name),
    typein(other.typein),
    file(other.file)
{}

skel_node* drain_node::clone() {
    return new drain_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

comp_node::comp_node( initializer_list<skel_node*> init )
    : concrete_skel_node( *this, init ) {}

comp_node::comp_node( const comp_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node * comp_node::clone() {
    return new comp_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

pipe_node::pipe_node( initializer_list<skel_node*> init )
    : concrete_skel_node( *this, init ) {}

pipe_node::pipe_node( const pipe_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node * pipe_node::clone() {
    return new pipe_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

farm_node::farm_node(initializer_list<skel_node*> init)
    : concrete_skel_node( *this, init ), pardegree(1) {}

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

map_node::map_node(initializer_list<skel_node*> init)
    : concrete_skel_node( *this, init ), pardegree(1) {}

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

reduce_node::reduce_node(initializer_list<skel_node*> init)
    : concrete_skel_node( *this, init ) {}

reduce_node::reduce_node( skel_node* pattexp, int pardegree )
        : concrete_skel_node( *this ), pardegree(pardegree) {
    add(pattexp);
}

reduce_node::reduce_node( const reduce_node& other )
    : concrete_skel_node( *this, other ) {}

skel_node* reduce_node::clone() {
    return new reduce_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

id_node::id_node( const string& id, const int& index )
    : concrete_skel_node( *this ), id( id ), index(index) {}

id_node::id_node( const string& id )
    : concrete_skel_node( *this ), id( id ), index(0) {}

id_node::id_node( const id_node& other )
    : concrete_skel_node( *this ), id( other.id ), index(other.index) {}

skel_node* id_node::clone() {
    return new id_node(*this);
}

///////////////////////////////////////////////////////////////////////////////

_::_()
    : concrete_skel_node( *this ) {}

_::_( const _& other )
    : concrete_skel_node( *this ) {}

skel_node* _::clone() {
    return new _(*this);
}
