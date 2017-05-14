///////////////////////////////////////////////////////////////////////////////
//
//  The environment contain the following type of bindings:
//  * K -> { V }
//
//  Environment implemented internally as a multimap
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_environment_hpp
#define rpl_environment_hpp

#include <map>
#include <utility>
#include <memory>

// useful typedefs
template <typename T>
using ptr = std::shared_ptr<T>;

template <typename K, typename V>
using iterator = typename std::multimap<K, V>::iterator;

template <typename K, typename V>
struct environment
{
    typedef ::iterator<K, ptr<V>> it;

    /// returns a range containing all the elements with given key
    std::pair<it, it> range( const K& key );

    // get an element for the given key
    ptr<V> get(const K& key);

    // substitutes old <key,value> pair with the new one
    void put(const K& key, V* value);

    // add the <key,value> pair in the environment
    void add(const K& key, V* value);

    // modify content at pos
    void modify(it pos, V* value);

    // clear content for given key
    void clear( const K& key );

private:
    std::multimap<K, ptr<V>> env;
};

template <typename K, typename V>
std::pair<::iterator<K, ptr<V>>, ::iterator<K, ptr<V>>> environment<K,V>::range( const K& key )
{
    return env.equal_range( key );
}

template <typename K, typename V>
ptr<V> environment<K,V>::get( const K& key )
{
    auto it = env.find(key);
    return (it != env.end()) ? it->second : nullptr;
}

template <typename K, typename V>
void environment<K,V>::put(const K& key, V* value)
{
    env.erase( key );
    env.insert({ key, ptr<V>(value) });
}

template <typename K, typename V>
void environment<K,V>::add(const K& key, V* value)
{
    env.insert({ key, ptr<V>(value) });
}

template <typename K, typename V>
void environment<K,V>::modify(::iterator<K, ptr<V>> pos, V* value)
{
    if (value != nullptr)
        pos->second = ptr<V>(value);
}

template <typename K, typename V>
void environment<K,V>::clear(const K& k)
{
    env.erase(k);
}
#endif
