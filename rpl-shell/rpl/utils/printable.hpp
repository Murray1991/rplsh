#ifndef rpl_printable_hpp
#define rpl_printable_hpp

#include "nodes/skeletons.hpp"
#include <string>

struct printable {
    virtual std::string print(skel_node& n) = 0;
};

#endif
