#include "water.hh"

water::water(const obj_raw::matToMeshsMap &matMap) noexcept
    : refractionTexture(0)
    , reflectionTexture(0)
{
    for (auto &[mat, meshes] : matMap) {
        if (mat->name == "Material.009") {
            prog = program::make_program("shaders/waterVertex.vert", "shaders/waterFragment.frag",
                                         "Matrix", mat);
            prog->init_objects(meshes);
            break;
        }
    }
}

void water::draw() noexcept {
    prog->setTexture("refraction", 0);
    glActiveTexture(GL_TEXTURE0);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, refractionTexture);TEST_OPENGL_ERROR();
    prog->setTexture("reflection", 1);
    glActiveTexture(GL_TEXTURE1);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);TEST_OPENGL_ERROR();
    prog->draw();
}

void water::add_texture(unsigned int texture, texture_type type) noexcept {
    if (type == texture_type::REFRACTION) {
        prog->setTexture("refraction", 0);
        refractionTexture = texture;
    } else if (type == texture_type::REFLECTION) {
        prog->setTexture("reflection", 1);
        reflectionTexture = texture;
    }
}
