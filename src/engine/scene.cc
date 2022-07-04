#include <utility>

#include "scene.hh"
#include "obj_raw.hh"

scene::scene(const std::string &obj_file) noexcept
{
    //init programs
    const auto &matMap = obj_raw::getMap(obj_file);
    for (auto &[mat, meshes] : matMap) {
        progMap[mat->name] = program::make_program("shaders/vertex.vert", "shaders/fragment.frag",
                                                   "Matrix", mat);
        progMap[mat->name]->init_objects(meshes);
    }
}

void scene::draw() noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto &[name, prog]: progMap)
        prog->draw();

    glutSwapBuffers();
    TEST_OPENGL_ERROR();
}

