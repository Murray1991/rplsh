#include "aux/wrappers.hpp"
#include "aux/types.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>

void tsleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

int get_random() {
    return rand() % 10;
}

template <typename T>
void print(const Container<T>& cont, const std::string& info) {
    std::cout << info << " : { ";
    for (std::size_t i = cont.lower; i < cont.upper; i++)
        std::cout << cont.get(i) << " ";
    std::cout << " }" << std::endl;
}

///////////////////// Source wrapper /////////////////////
class cont_source : public source<Container<int>> {
public:
    cont_source() : start(0) , end(10) {}
    bool has_next() { return start < end; }
    Container<int>* next();
private:
    std::size_t start;
    std::size_t end;
};

Container<int>* cont_source::next() {
    Container<int>* out = new Container<int>();
    for ( std::size_t i = 0; i < 100; i++ )
        out->add( get_random() );
    start++;
    return out;
}

///////////////////// Drain wrapper /////////////////////
class cont_drain : public drain<float> {
public:
    void process(float* res) {
        std::cout << "res: " << *res << std::endl;
    }
};

///////////////////// Inc seq_wrapper /////////////////////
class inc : public seq_wrapper<Container<int>, Container<float>> {
public:
    Container<float> compute( Container<int>& input );
    float op(int& num);
};

Container<float> inc::compute( Container<int>& input ) {
    Container<float> output;
    for ( std::size_t i = 0; i < input.size(); i++ )
        output.add( op(input[i]) );
    return output;
}

float inc::op( int& num ) {
    tsleep(5);
    return num+1;
}

///////////////////// Square seq_wrapper /////////////////////
class square : public seq_wrapper<Container<float>, Container<float>> {
public:
    Container<float> compute( Container<float>& input );
    float op( float& num );
};

Container<float> square::compute( Container<float>& input ) {
    for ( std::size_t i = 0; i < input.size(); i++ )
        input[i] = op(input[i]);
    return input;
}

float square::op( float& num ) {
    tsleep(10);
    return num*num;
}

///////////////////// Reduce seq_wrapper /////////////////////
class reduce_plus : public seq_wrapper<Container<float>, float> {
public:
    float compute( Container<float>& input );
    float op( float& f1, float& f2 );
    const float identity{0.0};
};

float reduce_plus::compute( Container<float>& input ) {
    float sum = 0;
    for ( std::size_t i = 0; i < input.size(); i++ )
        sum = op(sum, input[i]);
    return sum;
}

float reduce_plus::op(float& f1, float& f2) {
    tsleep(5);
    return f1+f2;
}
