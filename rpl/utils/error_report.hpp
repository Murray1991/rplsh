///////////////////////////////////////////////////////////////////////////////
//
//  Simple classes using for error reporting
//
///////////////////////////////////////////////////////////////////////////////
#ifndef error_report_hpp
#define error_report_hpp

#include "style.hpp"
#include <algorithm>
#include <ostream>
#include <memory>
#include <vector>

using namespace std;

struct error
{
    friend ostream& operator<<(ostream& os, error const& err)
    {
        err.print(os);
        return os;
    }

    virtual void print(ostream& os) const
    {
        os << "error default\n";
    }

    static string line;
};

string error::line = "";

struct error_unexp : public error
{
    error_unexp(const string word, const int pos)
        : word(word), pos(pos) {}

    virtual void print(ostream& os) const
    {
        style err(color::err, font::bold);
        style def(color::def, font::normal);
        int offset = rplsh::name.size() + pos;
        os << err << string(offset, ' ');
        os << err << string( max<int>(word.size(), 1), '^') << endl;
        os << def << rplsh::info;
        os << err << "error";
        os << def << ": token '" << word << "' unexpected" << endl;
    }

    const string word;
    const int pos;
};

struct error_illegal : public error
{
    error_illegal(string word, int pos)
        : word(word), pos(pos) {}

    virtual void print(ostream& os) const
    {
        style err(color::err, font::bold);
        style def(color::def, font::normal);
        int offset = rplsh::name.size() + pos;
        os << err << string(offset, ' ');
        os << err << string( max<int>(word.size(), 1), '^') << endl;
        os << def << rplsh::info;
        os << err << "error";
        os << def << ": token '" << word << "' is illegal" << endl;
    }

    const string word;
    const int pos;
};

struct error_container
{
    void add(unique_ptr<error> err)
    {
        errors.push_back(move(err));
    }

    error& get(int pos)
    {
        return *errors[pos];
    }

    int size()
    {
        return errors.size();
    }
private:
    vector<unique_ptr<error>> errors;
};

#endif
