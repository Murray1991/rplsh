#ifndef rpl_eval_dispatcher_hpp
#define rpl_eval_dispatcher_hpp

#include "dispatcher.hpp"
#include "visitors/visitors.hpp"
#include "visitors/evaluators/evaluators.hpp"
#include "parameters.hpp"
#include <memory>

struct eval_dispatcher : public dispatcher<std::string, std::unique_ptr<eval_visitor>>
{
    eval_dispatcher(rpl_environment& env) {
        add(par::servicetime    ,  std::unique_ptr<eval_visitor>(new servicetime(env)));
        add(par::latency        ,  std::unique_ptr<eval_visitor>(new latencytime(env)));
        add(par::compltime      ,  std::unique_ptr<eval_visitor>(new completiontime(env)));
        add(par::pardegree      ,  std::unique_ptr<eval_visitor>(new pardegree(env)));
        add(par::resources      ,  std::unique_ptr<eval_visitor>(new resources(env)));
    }
};

#endif
