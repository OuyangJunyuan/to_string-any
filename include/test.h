//
// Created by ou on 2021/8/16.
//

#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <ostream>

struct NotPrintable {
    int x = {0}, y{0}, z{0};
};

namespace reflect{
    void serialize(std::basic_ostream<char> &os, const NotPrintable &x, const char *name,
                   bool compact, size_t idx, size_t cnt, size_t depth);
}


#endif //TEST_TEST_H
