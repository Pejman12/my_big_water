#include "obj_raw.hh"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto map = obj_raw::getMap(argv[1]);
    std::cout << map.size() << std::endl;
    for (const auto &[mat, meshes]: map) {
        std::cout << mat.name << " : " << meshes.size() << std::endl;
        std::cout << "  " << meshes[0].name << std::endl;
        std::cout << "  " << meshes.back().name << std::endl;
    }
    return 0;
}