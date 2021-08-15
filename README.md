# 功能

实现变量的序列化，像to_string(int(1))一样，能够自动完成结构体成员的序列化。

# 例子
本例子已经被弃用，实际使用请参考main.cpp及其注释
```c++
#include <iostream>
#include "refl.h"

using namespace std;

template<typename T>
void serialize(ostream &os, const T &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    std::cout << "user-serialize-T";
}

void serialize_int(ostream &os, const int &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    std::cout << "user-serialize-int";
}

template<typename, typename>
struct UserCls {
};


REFL(Obj,
     (int, u, (serialize, T)),                             // 反射定义 int u,  使用自定义序列化 模板 函数
     (int, v, (serialize, T)),                             // 反射定义 int v,   使用自定义序列化 模板 函数
     (int, w, (serialize_int)),                         // 反射定义 int w,  使用自定义序列化          函数
     (int, (x, 1)),                                                 // 反射定义 int x,   使用默认序列化函数
     ((
             UserCls<int, int> bar;                    // 非反射定义，由于类型有逗号，使用双括号再书写语句
             double di;                                          // 非反射定义，用括号括起来后书写正常语句即可
             void menber_func(int a, int b);// 定义普通的函数
     ))
)

int main() {
    using namespace refl;
    Obj obj;
    utils::serialize(cout, obj, "obj", false);
    return 0;
}
```

输出结果：

```
obj: {
    user-serialize-T,
    user-serialize-T,
    user-serialize-int,
    x: 1
}
```

