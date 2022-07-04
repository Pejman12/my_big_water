#pragma once

#include "program.hh"

class scene {
public:
    scene() = delete;
    explicit scene(const obj_raw::matToMeshsMap &matMap) noexcept;

    void draw() noexcept;

    std::map<std::string, shared_program> progMap;
};

using shared_scene = std::shared_ptr<scene>;
