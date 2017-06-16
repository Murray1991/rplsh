#include "cpp_parser.hpp"
#include <fstream>
#include <regex>

using namespace std;

string trim ( const string& word ) {
    return regex_replace(word, std::regex("^ +"), "");
}

cpp_parser::cpp_parser( const string& path ) :
    path(path)
{}

pair<cpp_parser::iterator, cpp_parser::iterator> cpp_parser::parse() {

    // assumption: path to file exists
    ifstream file( path );

    // name: $3, typein: $9, typeout: $12
    regex seq_regex("(class|struct)([ ]*)(.+)([ ]*):([ ]*)public([ ]*)seq_wrapper([ ]*)<([ ]*)(.+)([ ]*),([ ]*)(.+)([ ]*)>");
    // name: $3, typeout: $9
    regex src_regex("(class|struct)([ ]*)(.+)([ ]*):([ ]*)public([ ]*)source([ ]*)<([ ]*)(.+)([ ]*)>");
    // name: $3, typein: $9
    regex drn_regex("(class|struct)([ ]*)(.+)([ ]*):([ ]*)public([ ]*)drain([ ]*)<([ ]*)(.+)([ ]*)>");

    std::string line;
    std::smatch match;
    while (std::getline(file, line)) {
        if( std::regex_search(line, match, seq_regex) ) {
            vec.push_back( wrapper_info( trim(match[3]), wrapper_info::seq,
                match[9], match[12]));
        } else if( std::regex_search(line, match, src_regex) ) {
            vec.push_back( wrapper_info( trim(match[3]), wrapper_info::source,
                "", match[9]));
        } else if( std::regex_search(line, match, drn_regex) ) {
            vec.push_back( wrapper_info( trim(match[3]), wrapper_info::source,
                match[9], ""));
        }
    }

    return {vec.begin(), vec.end()};
}

wrapper_info::wrapper_info(const string& name, wrapper_type wtype,
    const string& typein, const string& typeout) :
    name(name),
    wtype(wtype),
    typein(typein),
    typeout(typeout)
{}
