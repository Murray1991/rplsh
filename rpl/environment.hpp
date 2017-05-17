///////////////////////////////////////////////////////////////////////////////
//
//  The environment contain the following type of bindings:
//  * K -> { V }
//
//  Environment implemented internally as a map of vectors
//  (multimap was an alternative but it's useful access by
//  position in a set)
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_environment_hpp
#define rpl_environment_hpp

#include <map>
#include <vector>
#include <utility>
#include <memory>

// useful typedefs
template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using set = std::vector<ptr<T>>;

template <typename T>
using set_iterator = typename set<T>::iterator;

template <typename K, typename V>
struct environment
{
    typedef set_iterator<V> it;

    /// returns a range containing all the elements with given key
    std::pair<it, it> range( const K& key );

    // get an element for the given key at position pos
    ptr<V> get(const K& key, std::size_t pos = 0);

    // substitutes old <key,value> pair with the new one
    void put(const K& key, V* value);

    // add the <key,value> pair in the environment
    void add(const K& key, V* value);

    // modify content at pos
    void modify(it pos, V* value);

    // clear content for given key
    void clear( const K& key );

private:
    std::map<K, set<V>> env;
};

template <typename K, typename V>
std::pair<set_iterator<V>, set_iterator<V>> environment<K,V>::range( const K& key )
{
    auto it = env.find(key);
    if (it == env.end())
        env[key] = {};
    return std::make_pair(env[key].begin(), env[key].end());
}

template <typename K, typename V>
ptr<V> environment<K,V>::get( const K& key, std::size_t pos )
{
    auto it = env.find(key);
    return it != env.end() ?
        it->second.at(pos) :
        nullptr;
}

template <typename K, typename V>
void environment<K,V>::put(const K& key, V* value)
{
    auto it = env.find(key);
    if (it != env.end())
        it->second.clear();
    add(key, value);
}

template <typename K, typename V>
void environment<K,V>::add(const K& key, V* value)
{
    auto it = env.find(key);
    if (it == env.end())
        env.insert({});
    env[key].push_back(ptr<V>(value));
}

template <typename K, typename V>
void environment<K,V>::clear(const K& k)
{
    env.erase(k);
}
#endif
