#ifndef rpl_dispatcher_hpp
#define rpl_dispatcher_hpp

#include <map>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//  Template class that dispatch a name of type K returning a shared_ptr of
//  the instantiated object associated to the name, or a logic_error
//  exception
///////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
struct dispatcher {
    std::shared_ptr<V> operator[](const K& id);         // could throw logic_error exception
    void add(const K& id, std::shared_ptr<V> value);
    void add(const K& id, V* value);
    std::map<K, std::shared_ptr<V>>& get_map();
private:
    std::map<K, std::shared_ptr<V>> dispatch;
};

template <typename K, typename V>
std::shared_ptr<V> dispatcher<K,V>::operator[](const K& id) {
    return dispatch.at(id);
}

template <typename K, typename V>
void dispatcher<K, V>::add(const K& id, std::shared_ptr<V> value) {
    dispatch.insert(std::pair<K, std::shared_ptr<V>>(id, value));
}

template <typename K, typename V>
void dispatcher<K, V>::add(const K& id, V* value) {
    dispatch.insert(std::pair<K, std::shared_ptr<V>>(id, value));
}

template <typename K, typename V>
std::map<K, std::shared_ptr<V>>& dispatcher<K, V>::get_map() {
    return dispatch;
}

#endif
