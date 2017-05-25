#include "token.hpp"
#include "parameters.hpp"

using namespace std;

token::token(type kind, string data, int pos) :
    kind(kind),
    data(data),
    pos(pos)
{}

map<string, token::type> token::st_map =
{
   /* symbols */
   {"(",        token::open},
   {")",        token::close},
   {",",        token::comma},
   {"=",        token::equals},
   {"+",        token::plus },
   {"-",        token::minus },

   /* verb's keywords */
   {"with",     token::with},
   {"by",       token::by},

   /* verbs */
   {"show",     token::show},
   {"set",      token::set},
   {"annotate", token::annotate},
   {"rewrite",  token::rewrite},
   {"optimize", token::optimize},

   /* skeleton patterns */
   {"seq",      token::seq},
   {"Seq",      token::seq},
   {"comp",     token::comp},
   {"Comp",     token::comp},
   {"pipe",     token::pipe},
   {"Pipe",     token::pipe},
   {"farm",     token::farm},
   {"Farm",     token::farm},
   {"map",      token::map},
   {"Map",      token::map},
   {"reduce",   token::reduce},
   {"Reduce",   token::reduce},

   /* parameters */
   {par::servicetime,   token::parameter},
   {par::latency,       token::parameter},
   {par::efficiency,    token::parameter},
   {par::compltime,     token::parameter},
   {par::pardegree,     token::parameter},
   {par::resources,     token::parameter},

   {par::emitter_time,  token::parameter},
   {par::collector_time,token::parameter},
   {par::scatter_time,  token::parameter},
   {par::gather_time,   token::parameter},
   {par::dimension,     token::parameter},
   {par::arch,          token::parameter},
};

map<token::type, string> token::ts_map =
{
   {token::open,    "("},
   {token::close,   ")"},
   {token::comma,   ","},
   {token::seq,     "seq"},
   {token::comp,    "comp"},
   {token::pipe,    "pipe"},
   {token::farm,    "farm"},
   {token::map,     "map"},
   {token::reduce,  "reduce"}
};
