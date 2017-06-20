#include "aux/wrappers.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>

//////////////// Some utils functions ///////////////////////

template <typename Iterator>
void print(Iterator begin, Iterator end, const std::string& info) {
    std::cout << info << " : { ";
    for (auto it = begin; it != end; it++)
        std::cout << *it << std::endl;
    std::cout << " }" << std::endl;
}

void tsleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

int get_random() {
    return rand() % 10;
}

///////////////////// task alias template ////////////////
template <typename T>
using task = std::vector<T>;

///////////////////// Source wrapper /////////////////////
class vec_source : public source<task<int>> {
public:
    bool has_next() {
        return start < end;
    }

    task<int>* next() {
        task<int>* out = new task<int>();
        out->reserve(dim);
        for (auto i = 0; i < dim; i++ )
            out->push_back( get_random() );
        start++;
        return out;
    }

private:
    std::size_t start{0};
    std::size_t end{50};
    std::size_t dim{100};
};

///////////////////// Drain wrapper /////////////////////
class vec_drain : public drain<float> {
public:
    void process(float* res) {
        std::cout << "res: " << *res << std::endl;
    }
};

///////////////////// Inc seq_wrapper /////////////////////
class inc : public seq_wrapper<task<int>, task<float>> {
public:
    task<float> compute( task<int>& input ) {
        task<float> out;
        out.reserve(input.size());
        for (auto& t : input)
            out.push_back( op(t) );
        return out;
    }

    // this method MUST provided if the wrapper wants to be used
    // as a functional parameter for a data parallel skeleton (map)
    float op(int& n) {
        tsleep(5); // add some delay
        return n+1;
    }
};

///////////////////// Square seq_wrapper /////////////////////
class square : public seq_wrapper<task<float>, task<float>> {
public:
    task<float> compute( task<float>& input ) {
        for ( std::size_t i = 0; i < input.size(); i++ )
            input[i] = op(input[i]);
        return input;
    }

    float op( float& num ) {
        tsleep(10);
        return num*num;
    }
};

///////////////////// Reduce seq_wrapper /////////////////////
class redplus : public seq_wrapper<task<float>, float> {
public:
    float compute( task<float>& input ) {
        float sum = 0;
        for ( std::size_t i = 0; i < input.size(); i++ )
            sum = op(sum, input[i]);
        return sum;
    }

    // this method MUST provided if the wrapper wants to be used
    // as a functional parameter for a reduce skeleton
    float op( float& f1, float& f2 ) {
        tsleep(5);      // arbitrary delay
        return f1+f2;
    }

    // this member MUST provided if the wrapper wants to be used
    // as a functional parameter for a reduce skeleton
    const float identity{0.0};
};
