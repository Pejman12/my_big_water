#ifndef OBJECT_RAW_HEADER
#define OBJECT_RAW_HEADER

#include "obj_loader.hh"

#include <GL/glew.h>
#include <vector>
#include <map>

namespace obj_raw {
    struct objRaw {
        std::string name;
        std::map<std::string, std::vector<GLfloat>> vecs;

        bool operator==(const objRaw &other) const {
            return name == other.name;
        }

        bool operator<(const objRaw &other) const {
            return name < other.name;
        }
    };

    using matToMeshsMap = std::map<objRaw, std::vector<objRaw>>;

    objRaw makeObjRawFromMesh(const objl::Mesh &mesh);

    objRaw makeObjRawFromMat(const objl::Material &mat);

    matToMeshsMap getMap(const std::string &filename);
}

#endif