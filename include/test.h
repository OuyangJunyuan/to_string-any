//
// Created by ou on 2021/8/16.
//

#ifndef TEST_TEST_H
#define TEST_TEST_H
struct NotPrintable {
    int x = {0}, y{0}, z{0};
};

void debug(NotPrintable x) {
    std::cout << "a func was called by serialized"<<std::endl;
}
#endif //TEST_TEST_H
