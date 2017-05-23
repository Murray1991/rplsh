#ifndef rpl_visitor_dispatcher_hpp
#define rpl_visitor_dispatcher_hpp

#include "dispatchers/dispatcher.hpp"
#include "parameters.hpp"
#include "visitors.hpp"

struct visitor_dispatcher : public dispatcher<std::string, skel_visitor>
{
    visitor_dispatcher(rpl_environment& env) {
        add(par::show_default   ,  new printer());
        add(par::servicetime    ,  new servicetime(env));
        add(par::latency        ,  new latencytime(env));
        add(par::compltime      ,  new completiontime(env));
        add(par::pardegree      ,  new pardegree(env));
        add(par::resources      ,  new resources(env));
    }
};

#endif
