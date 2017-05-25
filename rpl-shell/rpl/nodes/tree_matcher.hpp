#ifndef tree_matcher_hpp
#define tree_matcher_hpp

#include <vector>

/**
    implements subtree pattern matching
    T:   type of the tree nodes: it must have the same iterable
         interface exposed for a skel_node
    S:   special type for a node: it indicates that a node
         with this type matches with the entire rooted subtree
*/
template <typename T, typename S>
struct tree_matcher
{
    bool match ( T* root, T *rule);
protected:
    bool match ( T* root, T* rule, bool reset );
    std::vector<T*> leaves;
};

template <typename T, typename S>
bool tree_matcher<T,S>::match(T* root, T* rule)
{
    return match(root, rule, true);
}

template <typename T, typename S>
bool tree_matcher<T,S>::match(T* root, T* rule, bool reset)
{
    if (reset)
        leaves.clear();

    if ( dynamic_cast<S*>(rule) ) {
        leaves.push_back(root);
        return true;
    }

    std::size_t sizel = root->size();
    std::size_t sizer = rule->size();
    bool res = *root == *rule && sizel == sizer;
    for (std::size_t i = 0; res && i < sizel; i++) {
        T* n1 = root->get(i);
        T* n2 = rule->get(i);
        res = res && match(n1, n2, false);
    }
    return res;
}

#endif
