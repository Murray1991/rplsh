#include "history.hpp"
#include "parser/lexer.hpp"
#include <algorithm>

history::history( const std::string& id, const history& h ) {
    error_container err_repo;
    for (auto rit = h.commands.rbegin() ; rit != h.commands.rend(); rit++) {
        auto cmd = *rit;
        std::vector<std::string> ids;
        lexer scanner(cmd.second, err_repo);
        auto tok = scanner.next();
        if (tok.kind == token::word && tok.data == id) {
            while (scanner.has_next()) {
                tok = scanner.next();
                if (tok.kind == token::word)
                    ids.push_back(tok.data);
            }
            commands.push_back(cmd);
            if (ids.size() == 0) break;
        } else if (tok.kind == token::optimize || tok.kind == token::rewrite ||
             tok.kind == token::annotate ) {
            while (scanner.has_next() && tok.kind != token::word)
                tok = scanner.next();
            if (tok.data == id)
                commands.push_back(cmd);
        }
        if (ids.size() > 0) {
            for ( auto rid : ids ) {
                history rh(rid, h);
                merge(rh);
            }
            break;
        }

    }
}

void history::add( const std::string& line ) {
    std::size_t index = commands.size();
    commands.push_back({ index, line });
}

void history::merge(const history& h) {
    commands.insert( commands.end(), h.commands.begin(), h.commands.end() );
    std::sort(commands.begin(), commands.end(), [](const pair<std::size_t, std::string> &a, const pair<std::size_t, std::string> &b){
        return a.first < b.first;
    });
    std::unique(commands.begin(), commands.end(), [](const pair<std::size_t, std::string> &a, const pair<std::size_t, std::string> &b){
        return a.first == b.first;
    });
}

void history::print( bool reverse ) const {
    if (reverse)
        for (auto rit = commands.rbegin(); rit != commands.rend(); rit++)
            std::cout << rit->first << "\t" << rit->second << std::endl;
    else
        for (auto it = commands.begin(); it != commands.end(); it++)
            std::cout << it->first << "\t" << it->second << std::endl;
}
