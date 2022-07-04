#include "water.hh"

water::water(const obj_raw::matToMeshsMap &matMap) noexcept
{
    for (auto &[mat, meshes] : matMap) {
        if (mat->name == "Material.009") {
            prog = program::make_program("shaders/vertex.vert", "shaders/fragment.frag",
                                         "Matrix", mat);
            prog->init_objects(meshes);
            break;
        }
    }
}

void water::draw() noexcept {
    prog->draw();
}
