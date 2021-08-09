#include "refl.h"
#include <vector>
#include <array>
#include <map>
#include <sstream>


using namespace std;


void serialize(std::ostream &os, const std::map<int, int> &obj, const char *fieldName, const refl::detail::Cfg &cfg) {
    os << fieldName << ": [\n";
    auto new_cfg{cfg};
    new_cfg.depth += 1;
    for (auto &&it:obj) {
        refl::detail::indent(os, new_cfg);
        os << "key: " << it.first << ", " << "val :" << it.second << endl;
    }
    refl::detail::indent(os, cfg);
    os << "]";
}

REFL(Obj,
     ((map<int, int>), (x, pair{1, 1},pair{2, 1},pair{3, 1}), serialize)
)
int main() {
    Obj obj;
    refl::utils::serialize(cout, obj, "obj", false);
    return 0;
}
