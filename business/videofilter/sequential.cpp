#include <iostream>
#include "business.hpp"

int main(int argc, char *argv[]) {

    vsource vsrc("prova.mp4");
    vdrain vdrn;
    gblur gb;
    togray tg;
    sobel s;

    double vsrctime = 0;
    double gbtime = 0;
    double tgtime = 0;
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

        tmp = tg.compute(tmp);      // compute togray

        auto t3a = aux::now();
        tmp = gb.compute(tmp);      // compute gblur

        auto t3 = aux::now();
        tmp = s.compute(tmp);       // compute sobel
        auto t4 = aux::now();

        // compute total times
        vsrctime += aux::time_elapsed<aux::milliseconds>(t2, t1);
        tgtime   += aux::time_elapsed<aux::milliseconds>(t3a, t2);
        gbtime   += aux::time_elapsed<aux::milliseconds>(t3, t3a);
        stime    += aux::time_elapsed<aux::milliseconds>(t4, t3);

        // not interesting measuring "drain time" (negligible)
        vdrn.process(&tmp);
        size++;

    }

    auto tot2 = aux::now();
    std::cout << "total time :" << aux::time_elapsed<aux::milliseconds>(tot2, tot1) << std::endl;
    std::cout << "vsource time :" << vsrctime << " (total), " << (vsrctime/size) << " (per item)" << std::endl;
    std::cout << "gblur time :" << gbtime << " (total), " << (gbtime/size) << " (per item)" << std::endl;
    std::cout << "togray time :" << tgtime << " (total), " << (tgtime/size) << " (per item)" << std::endl;
    std::cout << "sobel time :" << stime << " (total), " << (stime/size) << " (per item)" << std::endl;
    std::cout << "size: " << size << std::endl;
    return 0;
}
