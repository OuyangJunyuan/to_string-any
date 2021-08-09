# 功能

实现变量的序列化，像to_string(int(1))一样，能够自动完成结构体成员的序列化。

# 例子

```c++
#include "refl.h"
#include <vector>
#include <array>
#include <map>
#include <sstream>


using namespace std;


void serialize(std::ostream &os, const std::map<int, int> &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    os << fieldName << ".size: " << obj.size();
}

REFL(Cls1,
     ((map<int, int>), (x, pair{1, 1}, pair{2, 1}, pair{3, 1}), serialize)
)

REFL(Cls2,
     ((int), (int_, 1)),
     ((float), (float_, 1.0f)),
     ((double), (double_, 1.0))
)

int main() {
    Cls1 ins1;
    refl::utils::serialize(cout, ins1, "ins1", false);
    std::cout << "---" << std::endl;

    std::stringstream ss;
    refl::utils::serialize(ss, ins1, "ins1", true);
    std::cout << ss.str() << std::endl;
    std::cout << "---" << std::endl;

    refl::utils::serialize(cout, 1, "int", true);
    std::cout << "---" << std::endl;

    Cls2 ins2;
    refl::utils::serialize(cout, ins2, "ins2", true);
    std::cout << "---" << std::endl;

    refl::utils::serialize(cout, ins2, "ins2", false);
    return 0;
}
```

输出结果：

```
ins1: {
    x.size: 3
}
---
ins1: { x.size: 3 }

---
int: 1
---
ins2: { int_: 1, float_: 1, double_: 1 }
---
ins2: {
    int_: 1,
    float_: 1,
    double_: 1
}

Process finished with exit code 0

```

