#include <iostream>
#include <map>
#include "refl/refl.h"

using namespace std;

template<typename T>
void serialize(ostream &os, const T &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    refl::utils::decorate(os, obj, fieldName, cfg, [&]() {
        std::cout << "user-serialize-T";
    });
}

void serialize_int(ostream &os, const int &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    refl::utils::decorate(os, obj, fieldName, cfg, [&]() {
        std::cout << "user-serialize-int";
    });
}
template<typename T>
void print_map(ostream &os, const std::map<int, int> &obj, const char *fieldName, const refl::detail::Cfg &cfg);

template<typename, typename>
struct UserCls {
};


REFL(Obj,,
     (int, u, (serialize, T)),                             // 反射定义 int u,  使用自定义序列化 模板 函数
     (int, v, (serialize, T)),                             // 反射定义 int v,   使用自定义序列化 模板 函数
     (int, w, (serialize_int)),                         // 反射定义 int w,  使用自定义序列化          函数
     (int, (x, 1)),                                                 // 反射定义 int x,   使用默认序列化函数
     ((
             UserCls<int, int> bar;                    // 非反射定义，由于类型有逗号，使用双括号再书写语句
             double di;                                          // 非反射定义，用括号括起来后书写正常语句即可
             void menber_func(int a, int b);// 定义普通的函数
     ))
);


int main() {
    using namespace refl;
    Obj obj;
    utils::serialize(cout, obj, "obj", false);
    return 0;
}