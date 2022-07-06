#include "scene.hh"

scene::scene(const obj_raw::matToMeshsMap &matMap) noexcept
{
    //init programs
    for (auto &[mat, meshes] : matMap) {
        if (mat->name == "Material.009")
            continue;
        progMap[mat->name] = program::make_program("shaders/vertex.vert", "shaders/fragment.frag",
                                                   "Matrix", mat);
        progMap[mat->name]->init_objects(meshes);
    }
}

void scene::draw(const glm::vec4 &plane) noexcept {
    for (auto &[name, prog]: progMap) {
        prog->setPlane(plane);
        prog->draw();
    }
}

