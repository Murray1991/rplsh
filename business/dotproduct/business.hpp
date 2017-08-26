// dot product of two vectors
// the multiplication operator in "mul op" is set to last 15 ms,
// the addition operator in "inc op" is set to last 5 ms

#ifndef dotproduct_business_hpp
#define dotproduct_business_hpp

#include "aux/wrappers.hpp"
#include "aux/aux.hpp"
#include <ctime>
#include <cstdlib>
#include <limits>
#include <vector>
#include <utility>
#include <iostream>
#include <chrono>
#include <thread>

typedef double size_type ;
typedef std::vector<size_type> vec;

struct vec_pair {

    vec_pair(vec& A, vec& B) : A(A), B(B) {}

    const std::pair<size_type,size_type> operator[] (int x) {
        return std::make_pair(A[x], B[x]);
    }

    std::size_t size() {
        return A.size();
    }

private:
    const vec A;
    const vec B;
};

constexpr int dimension = 1;
constexpr size_type inputsize = 1000;

template <typename Iterator>
void init(Iterator begin, Iterator end) {
  while (begin != end) {
    *(begin++) = (double) ( ( (float)rand() / (float)RAND_MAX ) * std::numeric_limits<int>::max() );
  }
}

class dsrc : public source<vec_pair> {
public:

    dsrc() : i(0) {
        srand(0);
    }

    bool has_next() {
        return i++ < dimension;
    }

    vec_pair* next() {
        vec A(inputsize);
        vec B(inputsize);
        init(A.begin(), A.end());
        init(B.begin(), B.end());
        return new vec_pair(A,B);
    }

private:
    int i;
};

class ddrn : public drain<size_type> {
public:
    void process(size_type* value) {
        std::cout << "result is: " << *value << std::endl;
    }
};

class mul : public seq_wrapper<vec, vec_pair> {
public:
    vec compute(vec_pair& vp) {
        vec v(vp.size());
        for (int i = 0; i < vp.size(); i++)
            v[i] = op(vp[i]);
        return v;
    }

    size_type op(const std::pair<size_type, size_type>& p) {
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        return p.first*p.second;
    }
};

class inc : public seq_wrapper<size_type, vec> {
public:
    size_type compute(vec& v) {
        size_type sum = identity;
        for (auto& e : v)
            sum = op(sum, e);
        return sum;
    }

    size_type op(size_type& a, size_type& b) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return a + b;
    }

    const size_type identity{0};
};

#endif
