#ifndef rpl_Inode_hpp
#define rpl_Inode_hpp

#include "visitor_interface.hpp"

//  The base node interface of the rpl tree
struct rpl_node {
    virtual void accept(visitor& v) = 0;
};

#endif
