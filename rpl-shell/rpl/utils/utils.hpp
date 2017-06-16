#ifndef rpl_utils_hpp
#define rpl_utils_hpp

#include <string>
#include <cmath>
#include <climits>
#include <cstdlib>

namespace utils {

    template <typename T>
    std::string to_string(const T& t) {
        std::string str{ std::to_string(t) };
        int offset = 1;

        if ( str.find_last_not_of('0') == str.find('.') ) {
            offset = 0;
        }
        str.erase( str.find_last_not_of('0') + offset, std::string::npos);
        return str;
    }

    template <typename T>
    std::size_t count_digits(const T& n) {
        if ( n == 0 ) return 1;
        return std::floor( std::log10(n) + 1 );
    }

    template <typename T>
    std::string get_spaces(const T& num) {
        return num > 0 ? std::string(num, ' ') : "";
    }

    template <typename T>
    std::string get_idx(const T& idx, const T& max_idx) {
        std::size_t num_spaces = count_digits(max_idx) - count_digits(idx);
        return get_spaces(num_spaces) + "[" + std::to_string(idx) + "]";
    }


    std::string get_real_path(const std::string& str) {
        // unix method
        char * full_path = realpath(str.c_str(), NULL);
        std::string fstr(full_path);
        free(full_path);
        return fstr;
    }


}

#endif
