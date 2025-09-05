//
// Created by kali on 9/2/25.
//

#ifndef PARSE_COLOR_H
#define PARSE_COLOR_H

#include <cmath>
#include <experimental/optional>

template <class T>
using optional = std::experimental::optional<T>;
struct Color {
    inline Color() {
    }
    inline Color(unsigned char r_, unsigned char g_, unsigned char b_, float a_)
        : r(r_), g(g_), b(b_), a(a_ > 1 ? 1 : a_ < 0 ? 0 : a_) {
    }
    unsigned char r = 0, g = 0, b = 0;
    float a = 1.0f;
};
optional<Color> parseColor(const std::string& str);
#endif //PARSE_COLOR_H
