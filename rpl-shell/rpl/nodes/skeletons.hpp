#ifndef rpl_skeletons_hpp
#define rpl_skeletons_hpp

#include <string>
#include <vector>
#include "node_interface.hpp"

// The skeleton node interface
struct skel_node : public rvalue_node {

    skel_node(std::initializer_list<skel_node*> init);
    virtual ~skel_node();

    virtual void accept(visitor& v)                 = 0;
    virtual skel_node* clone()                      = 0;
    virtual bool operator==( const skel_node& rhs ) = 0;
    virtual bool operator!=( const skel_node& rhs ) = 0;

    /* common annotations */
    double inputsize;

    /* iterable interface */
    skel_node* get( std::size_t idx ) const;
    skel_node* pop();
    void set( skel_node* sk, std::size_t pos );
    void add( skel_node* sk );
    std::size_t size() const;

protected:
    std::vector<skel_node*> children;
};

template <typename skeleton>
struct concrete_skel_node : skel_node {
    virtual void accept(visitor& v);
    bool operator==( const skel_node& rhs );
    bool operator!=( const skel_node& rhs );
protected:
    concrete_skel_node( skeleton& sk );
    concrete_skel_node( skeleton& sk, std::initializer_list<skel_node*> init);
    concrete_skel_node( skeleton& sk, const skeleton& toclone);
private:
    bool match( const skel_node& rhs );
    skeleton& _sk;
};

struct seq_node : public concrete_skel_node<seq_node> {
    seq_node( double servicetime );
    seq_node( skel_node* pattexp, double servicetime = 1.0 );   //TODO check this...
    seq_node( const seq_node& other );
    skel_node* clone();
    double servicetime;
};

//TODO templated constructor taking iterators begin end?
struct comp_node : public concrete_skel_node<comp_node> {
    comp_node( std::initializer_list<skel_node*> init );
    comp_node( const comp_node& other );
    skel_node* clone();
};

struct pipe_node : public concrete_skel_node<pipe_node> {
    pipe_node( std::initializer_list<skel_node*> init );
    pipe_node( const pipe_node& other );
    skel_node* clone();
};

struct farm_node : public concrete_skel_node<farm_node> {
    farm_node( std::initializer_list<skel_node*> init );
    farm_node( skel_node* pattexp, int pardegree = 1 );
    farm_node( const farm_node& other );
    skel_node* clone();
    int pardegree;
};

struct map_node : public concrete_skel_node<map_node> {
    map_node( std::initializer_list<skel_node*> init );
    map_node( skel_node* pattexp, int pardegree = 1 );
    map_node( const map_node& other );
    skel_node* clone();
    int pardegree;
};

struct reduce_node : public concrete_skel_node<reduce_node> {
    reduce_node( std::initializer_list<skel_node*> init );
    reduce_node( skel_node* pattexp, int pardegree = 1 );
    reduce_node( const reduce_node& other );
    skel_node* clone();
    int pardegree;
};

struct id_node : public concrete_skel_node<id_node> {
    id_node( const std::string& id, const int& index );
    id_node( const std::string& id );
    id_node( const id_node& other );
    skel_node* clone();
    std::string id;
    int index;
};

// special node, it matches with every other node
struct _ : public concrete_skel_node<_> {
    _();
    _(const _& o);
    skel_node* clone();
};

#endif
