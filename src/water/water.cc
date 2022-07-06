#include "water.hh"

water::water(const obj_raw::matToMeshsMap &matMap) noexcept
    : refractionTexture(nullptr)
    , reflectionTexture(nullptr)
    , depthTexture(0)
    , time(0.0)
    , camPos(5.0f, 30.0f, 5.0f)
{
    for (auto &[mat, meshes] : matMap) {
        if (mat->name == "Material.009") {
            prog = program::make_program("shaders/waterVertex.vert", "shaders/waterFragment.frag",
                                         "Matrix", mat);
            prog->init_objects(meshes);
            break;
        }
    }
    dudvTexture = std::make_shared<texture>("textures/dudv1.png", false);
}

void water::draw() noexcept {
    prog->setTexture("refraction", 0);
    refractionTexture->bind(0);
    prog->setTexture("reflection", 1);
    reflectionTexture->bind(1);
    prog->setTexture("dudv", 2);
    dudvTexture->bind(2);
    prog->setTexture("depth", 3);
    glActiveTexture(GL_TEXTURE0 + 3);TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, depthTexture);TEST_OPENGL_ERROR();
    glEnable(GL_BLEND);TEST_OPENGL_ERROR();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);TEST_OPENGL_ERROR();
    time += 0.0015f;
    time = time > 1.0 ? 0.0 : time;
    prog->setTime(time);
    prog->setCamPos(camPos);
    prog->draw();
    glDisable(GL_BLEND);TEST_OPENGL_ERROR();
}

void water::add_texture(shared_texture texture, texture_type type) noexcept {
    if (type == texture_type::REFRACTION) {
        prog->setTexture("refraction", 0);
        refractionTexture = texture;
    } else if (type == texture_type::REFLECTION) {
        prog->setTexture("reflection", 1);
        reflectionTexture = texture;
    }
}
