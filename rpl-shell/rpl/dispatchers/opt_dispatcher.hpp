#ifndef rpl_opt_dispatcher_hpp
#define rpl_opt_dispatcher_hpp

#include "parameters.hpp"
#include "dispatcher.hpp"
#include "environment/rpl_environment.hpp"
#include "visitors/optimizers/optimizers.hpp"
#include <memory>

struct opt_dispatcher : public dispatcher<std::string, std::unique_ptr<optrule>>
{
    opt_dispatcher(rpl_environment& env) {
        add(par::farmopt,      std::unique_ptr<optrule>( new farmopt(env) ));
        add(par::mapopt,       std::unique_ptr<optrule>( new mapopt(env)));
        add(par::reduceopt,    std::unique_ptr<optrule>( new reduceopt(env)));
        add(par::pipeopt,      std::unique_ptr<optrule>( new pipeopt(env) ));
        add(par::maxresources, std::unique_ptr<optrule>( new maxresources(env)));
    }
};

#endif
