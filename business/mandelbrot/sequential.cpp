#include <iostream>
#include "business.hpp"

int main(int argc, char *argv[]) {
    // the wrappers
    matrixsource msrc;
    mandelbrot benoit;
    colorize applycol;
    matrixdrain mdrn;

    // some "time" variables
    double tot21  = 0;
    double tot32a = 0;
    double tot32b = 0;
    double tot43  = 0;
    double tot54  = 0;

    // dimension measures
    std::size_t streamsize = 0;
    std::size_t inputsize  = 0;

    auto ta = aux::now();
    while (msrc.has_next()) {
        auto t1 = aux::now();
        auto tmp = *msrc.next();
        auto t2 = aux::now();

        streamsize++;
        inputsize += tmp.size();

        // we now that has inner 'op' method
        for ( int i = 0; i < tmp.size(); i++ )
            benoit.op(tmp[i]);

        auto t3 = aux::now();
        tmp = applycol.compute(tmp);
        auto t4 = aux::now();
        mdrn.process(&tmp);
        auto t5 = aux::now();

        tot21 += aux::time_elapsed<aux::milliseconds>(t2, t1);
        tot32a += aux::time_elapsed<aux::milliseconds>(t3, t2);
        tot32b += (aux::time_elapsed<aux::milliseconds>(t3, t2)/tmp.size());
        tot43  += aux::time_elapsed<aux::milliseconds>(t4, t3);
        tot54 += aux::time_elapsed<aux::milliseconds>(t5, t4);

    }
    auto tb = aux::now();

    std::cout << "total time :" << aux::time_elapsed<aux::milliseconds>(tb, ta) << std::endl;
    std::cout << "total source: " << tot21 << ", per task: " << (tot21/streamsize) << std::endl;
    std::cout << "total mandelbrot: " << tot32a << ", per task: " << (tot32a/streamsize) << ", per subtask: " << (tot32b/streamsize) << std::endl;
    std::cout << "total colorize: " << tot43 << ", per task: " << (tot43/streamsize) << std::endl;
    std::cout << "total drain: " << tot54 << ", per task: " << (tot54/streamsize) << std::endl;
    std::cout << "stream size: " << streamsize << std::endl;
    std::cout << "input size: " << (inputsize/streamsize) << std::endl;

    return 0;

}
