///////////////////////////////////////////////////////////////////////////////
//
//  The environment should contain the following bindings:
//  * name - skeleton tree
//  * name - annotation                 <--- TODO ?
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_environment_hpp
#define rpl_environment_hpp

#include <map>

template <typename K, typename V>
struct environment
{
    V get(K& key)
    {
        return env.at(key);
    }

    void put(K& id, V value)
    {
        env[id] = value;
    }

private:
    std::map<K, V> env;
};

#endif
