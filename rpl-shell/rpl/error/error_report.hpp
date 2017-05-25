///////////////////////////////////////////////////////////////////////////////
//
//  Simple classes using for error reporting
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_error_report_hpp
#define rpl_error_report_hpp

#include "style.hpp"
#include <algorithm>
#include <ostream>
#include <memory>
#include <vector>

struct error
{
    friend ostream& operator<<(ostream& os, error const& err);
    virtual void print(ostream& os) const;
    static string line;
};

struct error_unexp : public error
{
    error_unexp(const string word, const int pos);
    virtual void print(ostream& os) const;
    const string word;
    const int pos;
};

struct error_illegal : public error
{
    error_illegal(string word, int pos);
    virtual void print(ostream& os) const;
    const string word;
    const int pos;
};

struct error_not_exist : public error
{
    error_not_exist(string word);
    virtual void print(ostream& os) const;
    const string word;
};

struct error_container
{
    void add(shared_ptr<error> err);
    error& get(int pos);
    int size();
private:
    std::vector<std::shared_ptr<error>> errors;
};

#endif
