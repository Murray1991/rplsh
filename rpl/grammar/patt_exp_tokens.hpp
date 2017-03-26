#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct seqtok_ : qi::symbols<char>
{
    seqtok_()
    {
        add
            ("seq")
            ("Seq")
        ;
    }

} seqtok;

struct comptok_ : qi::symbols<char>
{
    comptok_()
    {
        add
            ("comp")
            ("Comp")
        ;
    }

} comptok;

struct sourcetok_ : qi::symbols<char>
{
    sourcetok_()
    {
        add
            ("source")
            ("Source")
        ;
    }

} sourcetok;

struct draintok_ : qi::symbols<char>
{
    draintok_()
    {
        add
            ("drain")
            ("Drain")
        ;
    }

} draintok;

struct pipetok_ : qi::symbols<char>
{
    pipetok_()
    {
        add
            ("pipe")
            ("Pipe")
        ;
    }

} pipetok;

struct farmtok_ : qi::symbols<char>
{
    farmtok_()
    {
        add
            ("farm")
            ("Farm")
        ;
    }

} farmtok;

struct maptok_ : qi::symbols<char>
{
    maptok_()
    {
        add
            ("map")
            ("Map")
        ;
    }

} maptok;

struct reducetok_ : qi::symbols<char>
{
    reducetok_()
    {
        add
            ("drain")
            ("Drain")
        ;
    }

} reducetok;
