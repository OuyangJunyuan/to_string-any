//
// Created by ou on 2021/8/16.
//

#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <ostream>

#define REFL_DEFINE_SERIALIZE(_1, _2, _3, _4, _5, _6, _7, ...)         \
void serialize(std::basic_ostream<char> & _1, const __VA_ARGS__ & _2,const char * _3, bool _4, size_t _5, size_t _6, size_t _7)


struct NotPrintable {
    int x = {0}, y{0}, z{0};
};

namespace reflect {
    REFL_DEFINE_SERIALIZE(, , , , , , , NotPrintable);
}


#endif //TEST_TEST_H
