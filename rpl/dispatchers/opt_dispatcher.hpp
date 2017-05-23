#ifndef rpl_opt_dispatcher_hpp
#define rpl_opt_dispatcher_hpp

#include "dispatchers/dispatcher.hpp"
#include "parameters.hpp"
#include "optimizers.hpp"
#include "rpl_environment.hpp"
#include <memory>

struct opt_dispatcher : public dispatcher<std::string, std::unique_ptr<optrule>>
{
    opt_dispatcher(rpl_environment& env) {
        add(par::farmopt,      std::unique_ptr<optrule>( new farmopt(env) ));
        add(par::pipeopt,      std::unique_ptr<optrule>( new pipeopt(env) ));
        add(par::maxresources, std::unique_ptr<optrule>( new maxresources(env)));
    }
};

#endif
