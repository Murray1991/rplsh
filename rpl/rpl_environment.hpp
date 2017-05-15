///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Extends environment class by adding some useful getters and setters      //                                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef rpl_rplenvironment_hpp
#define rpl_rplenvironment_hpp

#include "environment.hpp"
#include "skeletons.hpp"
#include <map>

// TODO scatter and gather times should be something in
// function also of the input size and the number of workers
struct rpl_environment : public environment<std::string, skel_node>
{
    rpl_environment();

    void set_emitter_time(double te);
    void set_collector_time(double tc);
    void set_scatter_time(double ts);
    void set_gather_time(double tg);

    double get_emitter_time();
    double get_collector_time();
    double get_scatter_time();
    double get_gather_time();

private:
    std::map<std::string, double> sd_map;
};

#endif
