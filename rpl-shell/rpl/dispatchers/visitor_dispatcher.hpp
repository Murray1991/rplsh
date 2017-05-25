#ifndef rpl_visitor_dispatcher_hpp
#define rpl_visitor_dispatcher_hpp

#include "dispatcher.hpp"
#include "visitors/visitors.hpp"
#include "parameters.hpp"
#include <memory>

struct visitor_dispatcher : public dispatcher<std::string, std::unique_ptr<skel_visitor>>
{
    visitor_dispatcher(rpl_environment& env) {
        add(par::show_default   ,  std::unique_ptr<skel_visitor>(new printer()));
        add(par::servicetime    ,  std::unique_ptr<skel_visitor>(new servicetime(env)));
        add(par::latency        ,  std::unique_ptr<skel_visitor>(new latencytime(env)));
        add(par::compltime      ,  std::unique_ptr<skel_visitor>(new completiontime(env)));
        add(par::pardegree      ,  std::unique_ptr<skel_visitor>(new pardegree(env)));
        add(par::resources      ,  std::unique_ptr<skel_visitor>(new resources(env)));
    }
};

#endif
