#ifndef OBJECT_RAW_HEADER
#define OBJECT_RAW_HEADER

#include "obj_loader.hh"

#include <GL/glew.h>
#include <vector>
#include <map>
#include <memory>

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

    using objRawPtr = std::shared_ptr<objRaw>;

    using matToMeshsMap = std::map<objRawPtr , std::vector<objRawPtr>>;

    objRawPtr makeObjRawFromMesh(const objl::Mesh &mesh);

    objRawPtr makeObjRawFromMat(const objl::Material &mat);

    matToMeshsMap getMap(const std::string &filename);
}

#endif