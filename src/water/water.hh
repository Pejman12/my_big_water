#pragma once

#include "program.hh"
#include "obj_raw.hh"
#include "texture.hh"

class water {
public:
    enum class texture_type {
        REFRACTION,
        REFLECTION
    };
    water() = delete;
    explicit water(const obj_raw::matToMeshsMap &matMap) noexcept;

    void draw() noexcept;
    void add_texture(shared_texture texture, texture_type type) noexcept;

    shared_program prog;
    shared_texture refractionTexture;
    shared_texture reflectionTexture;
    shared_texture dudvTexture;
    GLuint depthTexture;
    float time;
    glm::vec3 camPos;
};

using shared_water = std::shared_ptr<water>;

