#ifndef rpl_utils_hpp
#define rpl_utils_hpp

#include <string>

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

}

#endif
