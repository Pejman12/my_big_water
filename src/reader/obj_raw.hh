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

    using shared_objRaw = std::shared_ptr<objRaw>;

    using matToMeshsMap = std::map<shared_objRaw , std::vector<shared_objRaw>>;

    shared_objRaw makeObjRawFromMesh(const objl::Mesh &mesh);

    shared_objRaw makeObjRawFromMat(const objl::Material &mat);

    matToMeshsMap getMap(const std::string &filename);
}

#endif