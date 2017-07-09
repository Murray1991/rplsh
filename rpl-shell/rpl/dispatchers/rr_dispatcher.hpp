#ifndef rpl_rr_dispatcher_hpp
#define rpl_rr_dispatcher_hpp

#include "dispatcher.hpp"
#include "rewriting/rewrules.hpp"
#include "parameters.hpp"
#include <string>
#include <memory>
#include <vector>

struct rr_dispatcher : public dispatcher<std::string, std::unique_ptr<rewrule>>
{
    rr_dispatcher() {
        add(par::pipeintro , std::unique_ptr<rewrule> (new pipeintro()));
        add(par::pipeelim  , std::unique_ptr<rewrule> (new pipeelim()));
        add(par::compassoc , std::unique_ptr<rewrule> (new compassoc()));
        add(par::pipeassoc , std::unique_ptr<rewrule> (new pipeassoc()));
        add(par::mapofcomp , std::unique_ptr<rewrule> (new mapofcomp()));
        add(par::compofmap , std::unique_ptr<rewrule> (new compofmap()));
        add(par::mapofpipe , std::unique_ptr<rewrule> (new mapofpipe()));
        add(par::pipeofmap , std::unique_ptr<rewrule> (new pipeofmap()));
        add(par::mapelim   , std::unique_ptr<rewrule> (new mapelim()));
        add(par::farmelim  , std::unique_ptr<rewrule> (new farmelim()));
        add(par::farmintro , std::unique_ptr<rewrule> (new farmintro()));
    }

    std::vector<std::string> get_allrules() {
        return {
            par::mapelim,
            par::pipeintro,
            par::pipeelim,
            par::compassoc,
            par::pipeassoc,
            par::mapofcomp,
            par::compofmap,
            par::mapofpipe,
            par::pipeofmap,
            par::farmelim,
            par::farmintro
        };
    }

};

#endif
