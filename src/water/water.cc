#include "water.hh"

water::water(const obj_raw::matToMeshsMap &matMap) noexcept
    : refractionTexture(nullptr)
    , reflectionTexture(nullptr)
{
    for (auto &[mat, meshes] : matMap) {
        if (mat->name == "Material.009") {
            prog = program::make_program("shaders/waterVertex.vert", "shaders/waterFragment.frag",
                                         "Matrix", mat);
            prog->init_objects(meshes);
            break;
        }
    }
    dudvTexture = std::make_shared<texture>("textures/dudv2.png", false);
}

void water::draw() noexcept {
    prog->setTexture("refraction", 0);
    refractionTexture->bind(0);
    prog->setTexture("reflection", 1);
    reflectionTexture->bind(1);
    prog->setTexture("dudv", 2);
    dudvTexture->bind(2);
    prog->draw();
}

void water::add_texture(shared_texture texture, texture_type type) noexcept {
    if (type == texture_type::REFRACTION) {
        prog->setTexture("refraction", 0);
        refractionTexture = texture;
    } else if (type == texture_type::REFLECTION) {
        prog->setTexture("reflection", 1);
        reflectionTexture = texture;
    } else if (type == texture_type::DUDV) {
        prog->setTexture("dudv", 2);
        dudvTexture = texture;
    }
}
