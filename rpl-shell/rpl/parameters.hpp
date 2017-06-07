///////////////////////////////////////////////////////////////////////////////
//
//  This file contains all the parameter names for the optimization
//  rules, the rewriting rules and so long as constant std::strings
//
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_parameters_hpp
#define rpl_parameters_hpp

#include <string>

namespace par
{
    /* rewriting rules parameter names */
    const std::string farmintro = "farmintro";
    const std::string farmelim  = "farmelim";
    const std::string pipeintro = "pipeintro";
    const std::string pipeelim  = "pipeelim";
    const std::string pipeassoc = "pipeassoc";
    const std::string compassoc = "compassoc";
    const std::string mapofcomp = "mapofcomp";
    const std::string compofmap = "compofmap";
    const std::string mapofpipe = "mapofpipe";
    const std::string pipeofmap = "pipeofmap";
    const std::string mapelim   = "mapelim";

    /* optimization parameter names */
    const std::string farmopt      = "farmopt";
    const std::string pipeopt      = "pipeopt";
    const std::string maxresources = "maxresources";

    /* non functional name parameters used for performance evaluation */
    const std::string servicetime    = "servicetime";
    const std::string efficiency     = "efficiency";
    const std::string latency        = "latency";
    const std::string compltime      = "compltime";
    const std::string pardegree      = "pardegree";
    const std::string resources      = "resources";

    /* environment variables */
    const std::string emitter_time   = "emitter_time";
    const std::string collector_time = "collector_time";
    const std::string scatter_time   = "scatter_time";
    const std::string gather_time    = "gather_time";
    const std::string dimension      = "dimension";
    const std::string arch           = "arch";

    /* other std::string constants */
    const std::string show_default   = "show_default";

}

#endif
