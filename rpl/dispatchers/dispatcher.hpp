#ifndef rpl_dispatcher_hpp
#define rpl_dispatcher_hpp

#include <map>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//  Template class that dispatch a name of type K returning
//  the instantiated object associated to the name, or a
//  logic_error exception
///////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
struct dispatcher {
    V& operator[]( const K& id );                 // could throw logic_error exception
    void add( const K& id, const V& value );
    void add( const K& id, V&& value );
    std::map<K, V>& get_map();
private:
    std::map<K, V> dispatch;
};

template <typename K, typename V>
V& dispatcher<K,V>::operator[](const K& id) {
    return dispatch.at(id);
}

template <typename K, typename V>
void dispatcher<K, V>::add(const K& id, const V& value) {
    dispatch.insert(std::pair<K, V>(id, value));
}

template <typename K, typename V>
void dispatcher<K, V>::add(const K& id, V&& value) {
    dispatch.insert(std::pair<K, V>(id, move(value)));
}

template <typename K, typename V>
std::map<K, V>& dispatcher<K, V>::get_map() {
    return dispatch;
}

#endif
