// wordcount application:
// - the wc_src stage reads a file and build a vector of words
// - the wc_map stage receives the vector of words and build a vector of maps
// - the wc_reduce stage receives a vector of maps and reduce it to a single map
// - the wc_drn stage receives the map and reads the word-value

#ifndef wordcount_business_hpp
#define wordcount_business_hpp

#include "aux/wrappers.hpp"
#include "aux/aux.hpp"
#include <ctime>
#include <cstdlib>
#include <limits>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <iostream>
#include <chrono>
#include <thread>

typedef std::vector<std::string> vecWords;
typedef std::map<std::string,int> mapWI;
typedef std::vector<mapWI> vecMapWIs;

class wc_src : public source<vecWords> {
public:

    wc_src() : filename("business.hpp") {}

    bool has_next() {
        return i++ == 0; // just once = one file
    }

    vecWords* next() {
        std::ifstream file(filename);
        vecWords* words = new vecWords();
        std::copy(std::istream_iterator<std::string>(file),
            std::istream_iterator<std::string>(),
            std::back_inserter(*words));
        return words;
    }

private:
    std::string filename;
    int i;
};

// The implementation takes into account a limitation of the gencode module of rplsh:
// the map stage MUST produce in output a vector of the SAME DIMENSION of the input vector
// TODO improve the generation code and modify this example to perform local
//      aggregation in the map phase

class wc_map : public seq_wrapper<vecMapWIs, vecWords> {
public:
    vecMapWIs compute(vecWords& words) {
        vecMapWIs maps;
        for (int i = 0; i < words.size(); i++)
            maps.push_back( op(words[i]) );
        return maps;
    }

    mapWI op(std::string& w) {
        mapWI mymap = {{w,1}};
        return mymap;
    }
};

class wc_reduce : public seq_wrapper<mapWI, vecMapWIs> {
public:
    mapWI compute(vecMapWIs& maps) {
        mapWI sum = identity;
        for (auto& entry : maps)
            sum = op(sum, entry);
        return sum;
    }

    mapWI op(const mapWI& a, const mapWI& b) {
        mapWI res = a;
        for (auto& w : b)
            res[w.first] += w.second;
        return res;
    }

    const mapWI identity{};
};

class wc_drn : public drain<mapWI> {
public:
    void process(mapWI* res) {
        for (auto& w : *res)
            std::cout << w.first << ": " << w.second << std::endl;
    }
};



#endif
