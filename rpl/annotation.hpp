#ifndef rpl_annotation_hpp
#define rpl_annotation_hpp

#include "node_forwards.hpp"

struct annotation
{
    // non functional properties of interest for performance
    const static std::string latency        = "latency";
    const static std::string servicetime    = "servicetime";

    // TODO non functional properties of interest for the wrapped seq code
    // e.g. typeIn, typeOut, funname, ...

    annotation(seq_node& n);
    annotation(comp_node& n);
    annotation(pipe_node& n);
    annotation(farm_node& n);
    annotation(map_node& n);
    annotation(reduce_node& n);

    std::map<std::string, double> sd_map;
    std::map<std::string, std::string> ss_map;
};

#endif
