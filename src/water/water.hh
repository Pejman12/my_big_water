#pragma once

#include "program.hh"
#include "obj_raw.hh"

class water {
public:
    water() = delete;
    explicit water(const obj_raw::matToMeshsMap &matMap) noexcept;

    void draw() noexcept;

    shared_program prog;
};

using shared_water = std::shared_ptr<water>;

