#include "obj_raw.hh"
#include <algorithm>

namespace obj_raw {

    shared_objRaw makeObjRawFromMesh(const objl::Mesh &mesh) {
        auto obj = std::make_shared<objRaw>();
        obj->name = mesh.MeshName;
        for (size_t i = 0; i < mesh.Vertices.size(); ++i) {
            obj->vecs["position"].push_back(mesh.Vertices[i].Position.X);
            obj->vecs["position"].push_back(mesh.Vertices[i].Position.Y);
            obj->vecs["position"].push_back(mesh.Vertices[i].Position.Z);

            obj->vecs["normal"].push_back(mesh.Vertices[i].Normal.X);
            obj->vecs["normal"].push_back(mesh.Vertices[i].Normal.Y);
            obj->vecs["normal"].push_back(mesh.Vertices[i].Normal.Z);
        }

        return obj;
    }

    shared_objRaw makeObjRawFromMat(const objl::Material &mat) {
        auto obj = std::make_shared<objRaw>();
        obj->name = mat.name;
        obj->vecs["Ka"].push_back(mat.Ka.X);
        obj->vecs["Ka"].push_back(mat.Ka.Y);
        obj->vecs["Ka"].push_back(mat.Ka.Z);

        obj->vecs["Kd"].push_back(mat.Kd.X);
        obj->vecs["Kd"].push_back(mat.Kd.Y);
        obj->vecs["Kd"].push_back(mat.Kd.Z);

        obj->vecs["Ks"].push_back(mat.Ks.X);
        obj->vecs["Ks"].push_back(mat.Ks.Y);
        obj->vecs["Ks"].push_back(mat.Ks.Z);

        obj->vecs["Ns"].push_back(mat.Ns);

        return obj;
    }

    matToMeshsMap getMap(const std::string &filename) {
        objl::Loader loader;
        loader.LoadFile(filename);

        auto &meshs = loader.LoadedMeshes;
        const auto &mats = loader.LoadedMaterials;

        matToMeshsMap res;

        for (const auto &mat: mats) {
            auto matObj = makeObjRawFromMat(mat);
            std::vector <std::string> seen;
            for (const auto &mesh: meshs) {
                if (mesh.MeshMaterial.name == mat.name) {
                    seen.push_back(mesh.MeshName);
                    res[matObj].push_back(makeObjRawFromMesh(mesh));
                }
            }
            meshs.erase(std::remove_if(meshs.begin(), meshs.end(), [&seen](const objl::Mesh &mesh) {
                return std::find(seen.begin(), seen.end(), mesh.MeshName) != seen.end();
            }), meshs.end());
        }

        return res;
    }
}