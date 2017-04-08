#include "token.hpp"

token::token(type kind, string data, int pos)
    : kind(kind), data(data), pos(pos) {}

map<string, token::type> token::st_map =
{
   {"(",        token::open},
   {")",        token::close},
   {",",        token::comma},
   {"=",        token::equals},
   {"with",     token::with},
   {"show",     token::show},
   {"set",      token::set},
   {"annotate", token::annotate},
   {"rewrite",  token::rewrite},
   {"optimize", token::optimize},
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
   {"servicetime",  token::parameter},
   {"efficiency",   token::parameter},
   {"pardegree",    token::parameter},
   {"resources",    token::parameter},
   {"compltime",    token::parameter}
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
