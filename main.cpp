#include "cp_attributes.hpp"

int main() {
    attributes::run("./test", {".hpp", ".h"}, { "json" }, cppast::libclang_compile_config());
    return 0;
}
