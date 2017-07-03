#include <iostream>
#include "business.hpp"

int main(int argc, char *argv[]) {

    vsource vsrc(argv[1]);
    vdrain vdrn;
    gblur gb;
    sobel s;

    double vsrctime = 0;
    double gbtime = 0;
    double stime = 0;
    int size = 0;

    auto tot1 = aux::now();

    while (true) {
        // get next frame
        auto t1 = aux::now();
        bool loop = vsrc.has_next();
        Mat tmp = *vsrc.next();
        auto t2 = aux::now();

        if (!loop) break;

        tmp = gb.compute(tmp);      // compute gaussian blur

        auto t3 = aux::now();
        tmp = s.compute(tmp);       // compute sobel
        auto t4 = aux::now();

        // compute times
        vsrctime += aux::time_elapsed<aux::milliseconds>(t2, t1);
        gbtime   += aux::time_elapsed<aux::milliseconds>(t3, t2);
        stime    += aux::time_elapsed<aux::milliseconds>(t4, t3);

        // not interesting measuring "drain time" (negligible)
        vdrn.process(&tmp);
        size++;

    }

    auto tot2 = aux::now();
    std::cout << "total time :" << aux::time_elapsed<aux::milliseconds>(tot2, tot1) << std::endl;
    std::cout << "vsource time :" << vsrctime << " (total), " << (vsrctime/size) << " (per item)" << std::endl;
    std::cout << "gbtime time :" << gbtime << " (total), " << (gbtime/size) << " (per item)" << std::endl;
    std::cout << "stime time :" << stime << " (total), " << (stime/size) << " (per item)" << std::endl;

    return 0;
}
