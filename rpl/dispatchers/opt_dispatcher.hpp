#ifndef rpl_opt_dispatcher_hpp
#define rpl_opt_dispatcher_hpp

#include "dispatchers/dispatcher.hpp"
#include "parameters.hpp"
#include "optimizers.hpp"
#include "rpl_environment.hpp"

struct opt_dispatcher : public dispatcher<std::string, optrule>
{
    opt_dispatcher(rpl_environment& env) {
        add(par::farmopt,      new farmopt(env));
        add(par::pipeopt,      new pipeopt(env));
        add(par::maxresources, new maxresources(env));
    }
};

#endif
