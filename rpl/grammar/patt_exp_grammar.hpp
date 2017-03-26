///////////////////////////////////////////////////////////////////////////////
//
//  Parallell pattern expression grammar in the rpl shell language.
//  It's implemented using Boost Spirit framework.
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include "patt_exp_tokens.hpp"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator, typename Skipper>
struct patt_exp_grammar : qi::grammar<Iterator, Skipper>
{
    patt_exp_grammar() : patt_exp_grammar::base_type(pattexp)
    {
        // tokens
        opentok = qi::lit('(');
        closetok = qi::lit(')');
        commatok = qi::lit(',');

        // sequential patterns: seq, comp, source, drain
        seqp
            =    seqtok
                    >> opentok
                        >> id
                        >> -( commatok                  //optional
                        >> qi::double_)                 //optional
                    >> closetok
                | comptok
                    >> opentok
                        >> (pattexp)
                        >> +(commatok
                        >> (pattexp))
                    >> closetok
                | sourcetok
                    >> opentok
                        >> id
                    >> closetok
                | draintok
                    >> opentok
                        >> id
                    >> closetok
            ;

        // stream patterns: pipe, farm
        streamp
            =    pipetok
                    >> opentok
                        >> (pattexp)
                        >> +(commatok
                        >> (pattexp))
                    >> closetok
                | farmtok
                    >> opentok
                        >> (pattexp)
                        >> -( commatok              //optional
                        >> integer)                 //optional
                    >> closetok
            ;

        // data parallel patterns: map, reduce
        datap
            =   maptok
                    >> opentok
                        >> (pattexp)
                        >> -( commatok              //optional
                        >> integer)                 //optional
                    >> closetok
                | reducetok
                    >> opentok
                        >> (pattexp)
                    >> closetok
            ;

        id = qi::lexeme[+( ascii::alpha >> *( ascii::alnum | qi::lit('_') ) )];
        num = qi::double_;
        integer = qi::int_;

        // pattern expression
        pattexp = seqp | streamp | datap | id;
    }

    qi::rule<Iterator, Skipper> pattexp;
    qi::rule<Iterator, Skipper> seqp, streamp, datap;

    qi::rule<Iterator, Skipper> id;
    qi::rule<Iterator, Skipper> num;
    qi::rule<Iterator, Skipper> integer;
    qi::rule<Iterator, Skipper> opentok;
    qi::rule<Iterator, Skipper> closetok;
    qi::rule<Iterator, Skipper> commatok;
};
