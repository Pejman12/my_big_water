#include "obj_loader.hh"

#include <GL/glew.h>
#include <stdio.h>
#include <vector>

struct objRaw
{
    std::vector<GLfloat> positionVect;
    std::vector<GLfloat> normalVect;
};


objRaw getVectorFromMesh(objl::Mesh mesh) {

    std::cout << mesh.MeshName << std::endl;
    objRaw obj;

    for (int i = 0; i < mesh.Vertices.size(); ++i) {
        obj.positionVect.push_back(mesh.Vertices[i].Position.X);
        obj.positionVect.push_back(mesh.Vertices[i].Position.Y);
        obj.positionVect.push_back(mesh.Vertices[i].Position.Z);

        obj.normalVect.push_back(mesh.Vertices[i].Normal.X);
        obj.normalVect.push_back(mesh.Vertices[i].Normal.Y);
        obj.normalVect.push_back(mesh.Vertices[i].Normal.Z);
    }
    return obj;
}

int main() {
    objl::Loader loader;
    loader.LoadFile("Erlaufsee.obj");

    for (int i = 0; i < loader.LoadedMeshes.size(); i++)
    {
        objRaw obj = getVectorFromMesh(loader.LoadedMeshes[i]);
    }

    return 0;
}