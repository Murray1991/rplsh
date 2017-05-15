#ifndef rpl_opt_dispatcher_hpp
#define rpl_opt_dispatcher_hpp

#include "dispatcher.hpp"
#include "optimizers.hpp"
#include "rpl_environment.hpp"

struct opt_dispatcher : public dispatcher<std::string, optrule>
{
    opt_dispatcher(rpl_environment& env) {
        add("farmopt",  new farmopt(env));
    }
};

#endif
