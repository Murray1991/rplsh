#include "business.hpp"
#include <iostream>

using namespace std;

int main() {

    dsrc src;
    ddrn drn;
    diffdirA ddmap;
    diffdirB ddred;
    size_t size;

    auto t1 = aux::now();
    bool loop = src.has_next();
    if (!loop) std::cout << "something wrong\n";
    auto tmp = src.next();

    size = tmp->size();
    std::cout << "size: " << size << endl;

    auto t2 = aux::now();
    auto v1 = ddmap.compute(*tmp);
    auto t3 = aux::now();
    auto b = ddred.compute(v1);
    auto t4 = aux::now();

    // compute total times
    auto tsrc = aux::time_elapsed<aux::milliseconds>(t2, t1);
    auto tmap = aux::time_elapsed<aux::milliseconds>(t3, t2);
    auto tred = aux::time_elapsed<aux::milliseconds>(t4, t3);
    auto ttot = aux::time_elapsed<aux::milliseconds>(t4, t1);

    // not interesting measuring "drain time" (negligible)
    drn.process(&b);

    std::cout << "total time :" << ttot << std::endl;
    std::cout << "source time :" << tsrc << std::endl;
    std::cout << "map time :" << tmap << " (total), " << (tmap/size) << " (per item)" << std::endl;
    std::cout << "red time :" << tred << " (total), " << (tred/size) << " (per item)" << std::endl;
    std::cout << "size: " << size << std::endl;

    return 0;
}
