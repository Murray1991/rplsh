#ifndef style_hpp
#define style_hpp

#include <ostream>

using namespace std;

namespace rplsh
{
    const string name = "rplsh> ";
    const string info = "rplsh: ";
}

enum color
{
    err = 31,
    def = 39
};

enum font
{
    normal  = 0,
    bold = 1
};

struct style
{
    style(color color_code, font font_code)
        : color_code(color_code), font_code(font_code) {}

    friend ostream& operator<<(ostream& os, style& s)
    {
        os  << "\033["
            << static_cast<int>(s.font_code) << ";"
            << static_cast<int>(s.color_code)  << "m";
        return os;
    }

private:
    color color_code;
    font font_code;
};

#endif
