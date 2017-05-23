#ifndef rpl_rr_dispatcher_hpp
#define rpl_rr_dispatcher_hpp

#include "dispatchers/dispatcher.hpp"
#include "parameters.hpp"
#include "rewrules.hpp"

struct rr_dispatcher : public dispatcher<std::string, rewrule>
{
    rr_dispatcher() {
        add(par::farmintro , new farmintro());
        add(par::farmelim  , new farmelim());
        add(par::pipeintro , new pipeintro());
        add(par::pipeelim  , new pipeelim());
        add(par::pipeassoc , new pipeassoc());
        add(par::compassoc , new compassoc());
        add(par::mapofcomp , new mapofcomp());
        add(par::compofmap , new compofmap());
        add(par::mapofpipe , new mapofpipe());
        add(par::pipeofmap , new pipeofmap());
    }
};

#endif
