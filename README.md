# 功能

实现轻量化变量的序列化，像to_string(int(1))一样，能够自动完成结构体成员的序列化，没有支持xml/yaml/json的打算。

# 例子

## 第一步:

head only library，包含头文件即可

```c++
#include "refl/refl.h"
```

## 第二步:

结构体定义：

```c++
REFL(结构体类型名,
    L_(子结构体类型名,变量名,
        L(字段类型,字段名,默认值),
        ....
        L(字段类型,字段名,默认值) )
    ... 
    L(字段类型,字段名,默认值)
)
```

例如：

```c++
REFL(Config,                                                                                                                           // 顶层结构体声明
     L_(Con, container_example,                                                                                      // 嵌套结构体声明：结构体名：Con，变量名：container_example
        L(vector<float>, voxel_size, (0.1f, 0.1f, 0.1f)),                                                   // 字段声明：L(字段类型, 字段名, 初始化值)
        L(list<int>, id, (1, 2, 3)),
        L((array<string, 3>), lidar_name, ("RS80_1", "RS80_2", "VLP32_1")),
        L((map<string, vector<double>>), roi_size,                                                     // 多参数模板类字段类型  需要额外加()
          ({ "RS80_1", vector<double>{1.0, 2.0, 3.0}},                                                   // 默认参数多个值时      需要额外加()
          {"RS80_2", vector<double>{1.0, 2.0, 3.0}},
          {"VLP32_1", vector<double>{1.0, 2.0, 3.0}}))),
     L_(Arg, arg,
        L(string, odom_topics, "/excavator_odom"),
        L(int, max_num, 1000),
        L(size_t, num_lidar, 3),
        L(float, voxel_size, 0.1f),
        L(double, roi_range_max, 300.0f),
        L(bool, is_ok, false),
        L(int * , int_p, nullptr),
        L(NotPrintable, not_p, (1, 2, 3))),

     __(
          int int_unreflectable;                                                                                                // __(...)语句 用于正常结构体内容声明
          void func(){std::cout << "hello world!"<<std::endl; }} 
       )                                       
);
```

## 第三步：

除了重载操作符<<外，对于无法修改其内容的类型（比如库提供的类型），可以自定义结构序列化函数。为了方便使用，先定义如下宏：

```c++
#define REFL_DEFINE_SERIALIZE(_1, _2, _3, _4, _5, _6, _7, ...)         \
void serialize(std::basic_ostream<char> & _1, const __VA_ARGS__ & _2,const char * _3, bool _4, size_t _5, size_t _6, size_t _7)
```

然后在调用序列化函数前声明如下内容：在reflect名称空间内，声明用于类型NotPrintable序列化的函数。

```c++
namespace reflect {
    REFL_DEFINE_SERIALIZE(, , , , , , , NotPrintable);
}
```

然后再任意地方实现它：

```c++
namespace reflect {
    REFL_DEFINE_SERIALIZE(os, obj, name, compact, idx, cnt, depth, NotPrintable) {
        decorate(os, compact, idx, cnt, depth, [&] {
            os << name << ": {";
            os << "x: " << obj.x << ", ";
            os << "y: " << obj.y << ", ";
            os << "z: " << obj.z << " }";
        });
    }
}
```

## 第四步：

序列化：调用 `reflect::serialize`来进行序列化，输出流为cout、被序列化类型为cfg、此外还有第三个字符串参数用于指定cfg的名称(默认"uname")、第四个参数bool用来觉得输出格式是紧凑形式还是宽松形式(默认false为宽松)。

```c++
auto cfg = Config();
reflect::serialize(cout, cfg);
std::cout<<"指定名称"<<std::endl;
reflect::serialize(cout, cfg,"config");
std::cout<<"紧凑形式"<<std::endl;
reflect::serialize(cout, cfg,"config",true);
===
uname: {
    container_example: {
        ...
    },
    arg: {
        ...
}
指定名称
uname: {
    container_example: {
        ...
    },
    arg: {
        ...
}
紧凑形式
config: { container_example: { ... }, arg: { ... } }
```

此外还可以指定字符串流为输出流：

```c++
std::stringstream ss;
reflect::serialize(ss,cfg);
std::cout <<ss.str()<<std::endl;
```

或者使用reflect::to_string: 使用方法与serialize一致。

```C++
cout << reflect::to_string(cfg) << endl;
```

## 最终效果:

自动遍历STL容器：其中map以`[key]: val`形式而其他按`[idx]: val` 来表示。对于指针类型，自动打印出其解引用后的内容并在变量名前加*表示。

```c++
uname: {
    container_example: {
        voxel_size: {
            [0]: 0.1,
            [1]: 0.1,
            [2]: 0.1
        },
        id: {
            [0]: 1,
            [1]: 2,
            [2]: 3
        },
        lidar_name: {
            [0]: RS80_1,
            [1]: RS80_2,
            [2]: VLP32_1
        },
        roi_size: {
            [RS80_1]: {
                [0]: 1,
                [1]: 2,
                [2]: 3
            },
            [RS80_2]: {
                [0]: 1,
                [1]: 2,
                [2]: 3
            },
            [VLP32_1]: {
                [0]: 1,
                [1]: 2,
                [2]: 3
            }
        }
    },
    arg: {
        odom_topics: /excavator_odom,
        max_num: 1000,
        num_lidar: 3,
        voxel_size: 0.1,
        roi_range_max: 300,
        is_ok: false,
        int_p: nullptr,
        not_p: {x: 1, y: 2, z: 3 }
    }
}
```

## 注意：

序列化执行顺序：

* 指针判断：空打印nullptr，非空解引用后序列化
* 判断是否可调用自定义序列化函数，是则执行
* 判断是否为bool：false打"false"，true打"true"
* 判断是否是REFL定义的可反射结构体类型：是则遍历其所有可反射字段
* 判断是否是容器类：是容器类则遍历
* 判断是否是重载<<操作符的类型：是则使用os<<obj
* 最终确定该类型无法打印：输出 unprintable

