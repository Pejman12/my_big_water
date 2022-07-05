#pragma once

#include "program.hh"
#include "obj_raw.hh"

class water {
public:
    enum class texture_type {
        REFRACTION,
        REFLECTION
    };
    water() = delete;
    explicit water(const obj_raw::matToMeshsMap &matMap) noexcept;

    void draw() noexcept;
    void add_texture(unsigned int texture, texture_type type) noexcept;

    shared_program prog;
    unsigned int refractionTexture;
    unsigned int reflectionTexture;
};

using shared_water = std::shared_ptr<water>;

