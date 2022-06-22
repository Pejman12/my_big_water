#include "obj_raw.hh"

void getVectorFromMesh(const objl::Mesh &mesh, objRaw &obj) {

    for (int i = 0; i < mesh.Vertices.size(); ++i) {
        obj.vecs[POSITION].push_back(mesh.Vertices[i].Position.X);
        obj.vecs[POSITION].push_back(mesh.Vertices[i].Position.Y);
        obj.vecs[POSITION].push_back(mesh.Vertices[i].Position.Z);

        obj.vecs[NORMAL].push_back(mesh.Vertices[i].Normal.X);
        obj.vecs[NORMAL].push_back(mesh.Vertices[i].Normal.Y);
        obj.vecs[NORMAL].push_back(mesh.Vertices[i].Normal.Z);
    }
}

void getMaterialFromMesh(const objl::Mesh &material, objRaw &obj) {

    obj.vecs[KA].push_back(material.MeshMaterial.Ka.X);
    obj.vecs[KA].push_back(material.MeshMaterial.Ka.Y);
    obj.vecs[KA].push_back(material.MeshMaterial.Ka.Z);

    obj.vecs[KD].push_back(material.MeshMaterial.Kd.X);
    obj.vecs[KD].push_back(material.MeshMaterial.Kd.Y);
    obj.vecs[KD].push_back(material.MeshMaterial.Kd.Z);

    obj.vecs[KS].push_back(material.MeshMaterial.Ks.X);
    obj.vecs[KS].push_back(material.MeshMaterial.Ks.Y);
    obj.vecs[KS].push_back(material.MeshMaterial.Ks.Z);

    obj.vecs[NS].push_back(material.MeshMaterial.Ns);
}

std::vector<objRaw> addObjs(const std::string filename) {
    objl::Loader loader;
    loader.LoadFile(filename);

    const auto &meshObjects = loader.LoadedMeshes;
    std::vector<objRaw> objs;

    for (const auto &mesh : meshObjects)
    {
        objRaw obj;
        obj.name = mesh.MeshName;
        getVectorFromMesh(mesh, obj);
        getMaterialFromMesh(mesh, obj);
        objs.push_back(obj);
    }

    return objs;
}