#include "obj_raw.hh"

int main(int argc, char const *argv[])
{
    auto objs = addObjs(argv[1]);
    for (auto &e : objs[0].vecs[KA]) {
        std::cout << e << std::endl;
    }
    return 0;
}