#ifndef rplsh_wrappers_hpp
#define rplsh_wrappers_hpp

#include <cstdlib>

template <typename Tout, typename Tin>
class seq_wrapper {
public:
  virtual Tout compute(Tin& input) = 0;
  // user should implement also the following methods
  // in order to generate data paralel skeletons
  // for map:    type_out op(type_in t)
  // for reduce: type op(type t1, type t2)
  // otherwise it will not compile
};

template <typename Tout>
class source {
public:
  virtual bool has_next() = 0;
  virtual Tout* next() = 0;
};

template <typename Tin>
class drain {
public:
  virtual void process(Tin * x) = 0;
};

/*template <typename T>
class composable {
public:
    virtual T&& operator[](size_t idx);
    virtual T& operator[](size_t idx);
}*/

// class type representing a set of values of type T
// that could be splitted in several sub-composable sets
// -> similiar concept to the TBB Range
template <typename T>
class composable {
public:
    virtual bool is_empty() const       = 0;
    virtual std::size_t size() const    = 0;
    virtual bool is_splittable() const  = 0;
};

#endif
