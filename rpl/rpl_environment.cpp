#include "rpl_environment.hpp"

rpl_environment::rpl_environment() {
    set_emitter_time(1);
    set_collector_time(1);
    set_scatter_time(1);
    set_gather_time(1);
}

void rpl_environment::set_emitter_time(double te) {
    sd_map["te"] = te;
}

void rpl_environment::set_collector_time(double tc) {
    sd_map["tc"] = tc;
}

void rpl_environment::set_scatter_time(double ts) {
    sd_map["ts"] = ts;
}

void rpl_environment::set_gather_time(double tg) {
    sd_map["tg"] = tg;
}

double rpl_environment::get_emitter_time() {
    return sd_map["te"];
}

double rpl_environment::get_collector_time() {
    return sd_map["tc"];
}

double rpl_environment::get_scatter_time() {
    return sd_map["ts"];
}

double rpl_environment::get_gather_time() {
    return sd_map["tg"];
}
