#include "rewrules.hpp"
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//          Some useful (for reading purposes) functions and macros          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
typedef skel_node* skelptr;

#define __ (new _())                                               // I'm sorry
#define POS(i) (leaves[i]->clone())

inline skelptr Pipe(skelptr s1, skelptr s2) {
    return new pipe_node{s1, s2};
}

inline skelptr Comp(skelptr s1, skelptr s2) {
    return new comp_node{s1, s2};
}

inline skelptr Farm(skelptr s) {
    return new farm_node(s);
}

inline skelptr Map(skelptr s) {
    return new map_node(s);
}

inline skelptr Reduce(skelptr s) {
    return new reduce_node(s);
}

///////////////////////////////////////////////////////////////////////////////

rewrule::rewrule( skel_node*&& left0, skel_node*&& left1 ) :
    left0( left0 ),
    left1( left1 )
{}

///////////////////////////////////////////////////////////////////////////////

farmintro::farmintro() : rewrule (
    __
){}

skel_node* farmintro::rewrite( skel_node& tree ) {
    return match( &tree, left0.get() ) ?
        Farm( POS(0) ) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

farmelim::farmelim() : rewrule (
    Farm(__)
){}

skel_node* farmelim::rewrite( skel_node& tree ) {
    return match( &tree, left0.get() ) ?
        POS(0) : nullptr;
}

///////////////////////////////////////////////////////////////////////////////

pipeintro::pipeintro() : rewrule (
    Comp(__, __)
){}

skel_node* pipeintro::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Pipe( POS(0) , POS(1) ) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

pipeelim::pipeelim() : rewrule (
    Pipe(__,__)
){}

skel_node* pipeelim::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Comp( POS(0), POS(1) ) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

pipeassoc::pipeassoc() : rewrule (
    Pipe(Pipe(__,__),__),
    Pipe(__,Pipe(__,__))
){}

skel_node* pipeassoc::rewrite( skel_node& tree ) {
    skel_node* r = nullptr;
    if ( match(&tree, left0.get()) )
        return Pipe(POS(0), Pipe(POS(1), POS(2)));
    if ( match(&tree, left1.get()))
        return Pipe(Pipe(POS(0), POS(1)), POS(2));
    return r;
}

///////////////////////////////////////////////////////////////////////////////

compassoc::compassoc() : rewrule (
    Comp(Comp(__,__),__),
    Comp(__,Comp(__,__))
){}

skel_node* compassoc::rewrite( skel_node& tree ) {
    skel_node* r = nullptr;
    if ( match(&tree, left0.get()) )
        return Comp(POS(0), Comp(POS(1), POS(2)));
    if ( match(&tree, left1.get()))
        return Comp(Comp(POS(0), POS(1)), POS(2));
    return r;
}

///////////////////////////////////////////////////////////////////////////////

mapofcomp::mapofcomp() : rewrule (
    Comp(Map(__), Map(__))
){}

skel_node* mapofcomp::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Map(Comp(POS(0), POS(1))) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

compofmap::compofmap() : rewrule (
    Map(Comp(__,__))
){}

skel_node* compofmap::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Comp(Map(POS(0)), Map(POS(1))) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

mapofpipe::mapofpipe() : rewrule (
    Pipe(Map(__), Map(__))
){}

skel_node* mapofpipe::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Map(Pipe(POS(0), POS(1))) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

pipeofmap::pipeofmap() : rewrule (
    Map(Pipe(__,__))
){}

skel_node* pipeofmap::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Pipe(Map(POS(0)), Map(POS(1))) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

mapelim::mapelim() : rewrule (
    Map(__)
){}

skel_node* mapelim::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        POS(0) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

reduceelim::reduceelim() : rewrule (
    Reduce(__)
){}

skel_node* reduceelim::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        POS(0) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

mapmapelim::mapmapelim() : rewrule (
    Map(Map(__))
){}

skel_node* mapmapelim::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Map(POS(0)) :
        nullptr;
}

///////////////////////////////////////////////////////////////////////////////

farmfarmelim::farmfarmelim() : rewrule (
    Farm(Farm(__))
){}

skel_node* farmfarmelim::rewrite( skel_node& tree ) {
    return match(&tree, left0.get()) ?
        Farm(POS(0)) :
        nullptr;
}
