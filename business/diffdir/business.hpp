#ifndef diffdir_business_hpp
#define diffdir_business_hpp

#include "aux/wrappers.hpp"
#include "aux/aux.hpp"
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>

typedef std::vector<bool> vB;
typedef std::pair<std::string, std::string> pSS;
typedef std::vector<std::pair<pSS, pSS>> vppSS;

const std::string dir1 = "A"; // put first directory name
const std::string dir2 = "B"; // put second directory name

// some util functions
std::string get_path(const std::string& name, const std::string& dir) {
    std::string s = dir + (dir.back() != '/' ? "/" : "") + name;
    return s;
}

std::vector<std::string> get_names_from_dir(const std::string& dir) {
    std::cout << "read from " << dir << std::endl;
    struct dirent *dp;
    std::vector<std::string> vec;
    auto dirp = opendir(dir.c_str());
    while ((dp = readdir(dirp)) != NULL)
        if (dp->d_type != DT_DIR)
            vec.push_back( std::string(dp->d_name) );
    return vec;
}

pSS get_name_content(const std::string& name, const std::string& dir) {
    if (name != "") {
        std::ifstream ifs( get_path(name, dir) );
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                           (std::istreambuf_iterator<char>()    ) );
        return pSS(name, content);
    }
    return pSS("","");
}

class dsrc : public source<vppSS> {
public:
    dsrc(const std::string& dir1, const std::string& dir2)
        : d1(dir1), d2(dir2), vec(nullptr) {}

    dsrc() : dsrc(dir1, dir2) {}

    bool has_next() {
        return (vec == nullptr);
    }

    vppSS* next() {
        vec = new vppSS();
        auto v1 = get_names_from_dir(d1);
        auto v2 = get_names_from_dir(d2);
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        auto N = std::max(v1.size(), v2.size());
        for (std::size_t i = 0; i < N; i++) {
            std::string s1 = i < v1.size() ? v1[i] : "";
            std::string s2 = i < v1.size() ? v1[i] : "";
            vec->emplace_back(get_name_content(s1, d1),
                            get_name_content(s2, d2));
        }
        return vec;
    }

private:
    const std::string d1;
    const std::string d2;
    vppSS* vec;
};

class ddrn : public drain<bool> {
public:
    void process(bool* value) {
        std::cout << "directories are " << (*value ? "" : "not ") << "equal" << std::endl;
    }
};

class diffdirA : public seq_wrapper<vB, vppSS> {
public:
    vB compute(vppSS& v) {
        vB vec;
        for (auto p : v)
            vec.push_back(op(p));
        return vec;
    }

    bool op(std::pair<pSS, pSS>& p) {
        return p.first == p.second;
    }
};

class diffdirB : public seq_wrapper<bool, vB> {
public:
    bool compute(vB& vec) {
        bool b = identity;
        for (auto p : vec)
            b = op(b, p);
        return b;
    }

    bool op(bool b1, bool b2) {
        return b1 && b2;
    }

    const bool identity{true};
};

#endif
