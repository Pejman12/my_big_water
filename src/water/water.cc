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
    if (reflectionTexture != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    }
    if (refractionTexture != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, refractionTexture);
    }
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
