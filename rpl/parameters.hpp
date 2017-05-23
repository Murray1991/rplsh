///////////////////////////////////////////////////////////////////////////////
//
//  This file contains all the parameter names for the optimization
//  rules, the rewriting rules and so long as constant strings
//
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_parameters_hpp
#define rpl_parameters_hpp

namespace par
{
    /* rewriting rules parameter names */
    const string farmintro = "farmintro";
    const string farmelim  = "farmelim";
    const string pipeintro = "pipeintro";
    const string pipeelim  = "pipeelim";
    const string pipeassoc = "pipeassoc";
    const string compassoc = "compassoc";
    const string mapofcomp = "mapofcomp";
    const string compofmap = "compofmap";
    const string mapofpipe = "mapofpipe";
    const string pipeofmap = "pipeofmap";

    /* optimization parameter names */
    const string farmopt      = "farmopt";
    const string pipeopt      = "pipeopt";
    const string maxresources = "maxresources";

    /* non functional name parameters used for performance evaluation */
    const string servicetime    = "servicetime";
    const string latency        = "latency";
    const string completiontime = "completiontime";
    const string pardegree      = "pardegree";
    const string resources      = "resources";

    /* other string constants */
    const string show_default   = "show_default";

}

#endif
