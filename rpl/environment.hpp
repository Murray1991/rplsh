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
#include <memory>

template <typename T>
using snode_ptr = std::shared_ptr<T>;

template <typename K, typename V>
struct environment {
    snode_ptr<V> get(K& key);
    void put(K& id, V* value);
private:
    std::map<K, snode_ptr<V>> env;
};

template <typename K, typename V>
snode_ptr<V> environment<K,V>::get(K& key) {
    return env.at(key);
}

template <typename K, typename V>
void environment<K,V>::put(K& id, V* value) {
    snode_ptr<V> sptr(value);
    env[id] = sptr;
}

#endif
