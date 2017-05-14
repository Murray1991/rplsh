#ifndef rpl_visitor_dispatcher_hpp
#define rpl_visitor_dispatcher_hpp

#include "dispatcher.hpp"
#include "visitors.hpp"

template <typename Environment>
struct visitor_dispatcher : public dispatcher<std::string, skel_visitor>
{
    visitor_dispatcher(Environment& env) {
        add("show_default",  new printer());
        add("servicetime" ,  new servicetime<Environment>(env));
        add("latency"     ,  new latencytime<Environment>(env));
    }
};

#endif
