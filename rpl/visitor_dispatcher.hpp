#ifndef rpl_visitor_dispatcher_hpp
#define rpl_visitor_dispatcher_hpp

#include "dispatcher.hpp"
#include "visitors.hpp"

struct visitor_dispatcher : public dispatcher<std::string, skel_visitor>
{
    visitor_dispatcher(rpl_environment& env) {
        add("show_default",  new printer());
        add("servicetime" ,  new servicetime(env));
        add("latency"     ,  new latencytime(env));
    }
};

#endif