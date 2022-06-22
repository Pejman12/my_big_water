#include "obj_raw.hh"

void getVectorFromMesh(objl::Mesh mesh, objRaw obj) {

    for (int i = 0; i < mesh.Vertices.size(); ++i) {
        obj.positionVect.push_back(mesh.Vertices[i].Position.X);
        obj.positionVect.push_back(mesh.Vertices[i].Position.Y);
        obj.positionVect.push_back(mesh.Vertices[i].Position.Z);

        obj.normalVect.push_back(mesh.Vertices[i].Normal.X);
        obj.normalVect.push_back(mesh.Vertices[i].Normal.Y);
        obj.normalVect.push_back(mesh.Vertices[i].Normal.Z);
    }
}

void getMaterialFromMesh(objl::Mesh material, objRaw obj) {

    obj.kaVect.push_back(material.MeshMaterial.Ka.X);
    obj.kaVect.push_back(material.MeshMaterial.Ka.Y);
    obj.kaVect.push_back(material.MeshMaterial.Ka.Z);

    obj.ksVect.push_back(material.MeshMaterial.Ks.X);
    obj.ksVect.push_back(material.MeshMaterial.Ks.Y);
    obj.ksVect.push_back(material.MeshMaterial.Ks.Z);

    obj.kdVect.push_back(material.MeshMaterial.Kd.X);
    obj.kdVect.push_back(material.MeshMaterial.Kd.Y);
    obj.kdVect.push_back(material.MeshMaterial.Kd.Z);

    obj.nsVect.push_back(material.MeshMaterial.Ns);
   
}

std::vector<objRaw> addObjs(const std::string filename) {
    objl::Loader loader;
    loader.LoadFile(filename);

    std::vector<objl::Mesh> meshObjects = loader.LoadedMeshes;
    std::vector<objRaw> objs;

    for (const auto &mesh : meshObjects)
    {
        objRaw obj;
        getVectorFromMesh(mesh, obj);
        getMaterialFromMesh(mesh, obj);
        objs.push_back(obj);
    }

    return objs;
}