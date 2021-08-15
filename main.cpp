#include <vector>
#include <array>
#include <list>
#include <map>
#include <iostream>
#include "test.h" // include other files before refl
#include "refl/refl.h"

using namespace std;


REFL(Config,                                                    // 顶层结构体声明
     L_(Con, container_example,                                 // 嵌套结构体声明：结构体名：Con，变量名：container_example
        L(vector<float>, voxel_size, (0.1f, 0.1f, 0.1f)),       // 字段声明：L(字段类型, 字段名, 初始化值)
        L(list<int>, id, (1, 2, 3)),
        L((array<string, 3>), lidar_name, ("RS80_1", "RS80_2", "VLP32_1")),
        L((map<string, vector<double>>), roi_size,              // 多参数模板类字段类型  需要额外加()
          ({ "RS80_1", vector<double>{1.0, 2.0, 3.0}},          // 默认参数多个值时      需要额外加()
          {"RS80_2", vector<double>{1.0, 2.0, 3.0}},
          {"VLP32_1", vector<double>{1.0, 2.0, 3.0}}))),

     L_(Arg, arg,
        L(string, odom_topics, "/excavator_odom"),
        L(int, max_num, 1000),
        L(size_t, num_lidar, 3),
        L(float, voxel_size, 0.1f),
        L(double, roi_range_max, 300.0f),
        L(const char*, title, "lidar_perception_sys"),
             L(NotPrintable, not_p)),

     __(void func1() { std::cout << "reserve format"; }),      // __(...)用于正常的声明
     __(void func2(int) {})
);

int main() {
    auto cfg = Config();
    reflect::serialize(cout, cfg, "config");
//    cout << reflect::to_string(cfg) << endl;
    return 0;
}