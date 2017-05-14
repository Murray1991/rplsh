#ifndef rpl_rr_dispatcher_hpp
#define rpl_rr_dispatcher_hpp

#include "dispatcher.hpp"
#include "rewrules.hpp"

struct rr_dispatcher : public dispatcher<std::string, rewrule>
{
    rr_dispatcher() {
        add("farmintro" , new farmintro());
        add("farmelim"  , new farmelim());
        add("pipeintro" , new pipeintro());
        add("pipeelim"  , new pipeelim());
        add("pipeassoc" , new pipeassoc());
        add("compassoc" , new compassoc());
        add("mapofcomp" , new mapofcomp());
        add("compofmap" , new compofmap());
        add("mapofpipe" , new mapofpipe());
        add("pipeofmap" , new pipeofmap());
    }
};

#endif
