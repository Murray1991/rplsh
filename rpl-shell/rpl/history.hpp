#ifndef rpl_history_hpp
#define rpl_history_hpp

#include <string>
#include <vector>

struct history {
    history() = default;
    history( const std::string& id, const history& h );
    void add( const std::string& line );
    void print( bool reverse = false ) const;
    void merge( const history& h );
protected:
    std::vector<std::pair<std::size_t, std::string>> commands;
};

#endif
