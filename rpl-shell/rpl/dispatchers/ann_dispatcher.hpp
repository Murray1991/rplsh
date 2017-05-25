#ifndef rpl_ann_dispatcher_hpp
#define rpl_ann_dispatcher_hpp

#include "dispatcher.hpp"
#include "parameters.hpp"
#include "visitors/annotators/annotators.hpp"
#include <memory>

struct ann_dispatcher : public dispatcher<std::string, std::unique_ptr<ann_visitor>>
{
    ann_dispatcher(rpl_environment& env) {
        add(par::servicetime    ,  std::unique_ptr<ann_visitor>(new ann_servicetime(env)));
        add(par::latency        ,  std::unique_ptr<ann_visitor>(new ann_servicetime(env)));
        add(par::pardegree      ,  std::unique_ptr<ann_visitor>(new ann_pardegree(env)));
    }
};

#endif
