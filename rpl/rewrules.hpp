#ifndef rpl_rewrules_hpp
#define rpl_rewrules_hpp

#include "skeletons.hpp"
#include "visitors.hpp"
#include <memory>

struct rewrule;

struct rewrule
{
    rewrule( skel_node*&& left, skel_node*&& right = nullptr );
    virtual skel_node* rewrite( skel_node& tree ) = 0;

protected:
    bool match( skel_node *root, skel_node *rule, bool reset = true );
    std::unique_ptr<skel_node> left;
    std::unique_ptr<skel_node> right;
    std::vector<skel_node*> leaves;
};

struct farmintro : rewrule {
    farmintro();
    skel_node* rewrite( skel_node& tree );
};

struct farmelim : rewrule {
    farmelim();
    skel_node* rewrite( skel_node& tree );
};

struct pipeintro : rewrule {
    pipeintro();
    skel_node* rewrite( skel_node& tree );
};

struct pipeelim : rewrule {
    pipeelim();
    skel_node* rewrite( skel_node& tree );
};

struct pipeassoc : rewrule {
    pipeassoc();
    skel_node* rewrite( skel_node& tree );
};

struct compassoc : rewrule {
    compassoc();
    skel_node* rewrite( skel_node& tree );
};

struct mapofcomp : rewrule {
    mapofcomp();
    skel_node* rewrite( skel_node& tree );
};

struct compofmap : rewrule {
    compofmap();
    skel_node* rewrite( skel_node& tree );
};

struct mapofpipe : rewrule {
    mapofpipe();
    skel_node* rewrite( skel_node& tree );
};

struct pipeofmap : rewrule {
    pipeofmap();
    skel_node* rewrite( skel_node& tree );
};

#endif
