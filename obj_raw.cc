#include "obj_loader.hh"

#include <GL/glew.h>
#include <stdio.h>
#include <vector>

struct objRaw
{
    std::vector<GLfloat> positionVect;
    std::vector<GLfloat> normalVect;

    std::vector<GLfloat> kaVect;
    std::vector<GLfloat> kdVect;
    std::vector<GLfloat> ksVect;
    std::vector<GLfloat> nsVect;
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

    std::vector<objl::Mesh> meshObjects = loader.LoadedMeshes;
    std::vector<objl::Material> materialObjects = loader.LoadedMaterials;

    objRaw obj;

    // MeshObjects
    for (int i = 0; i < meshObjects.size(); i++)
    {
        obj = getVectorFromMesh(meshObjects[i]);
    }

   // MaterialObjects
   for (int i = 0; i < materialObjects.size(); i++)
   {
       obj.kaVect.push_back(materialObjects[i].Ka.X);
       obj.kaVect.push_back(materialObjects[i].Ka.Y);
       obj.kaVect.push_back(materialObjects[i].Ka.Z);

       obj.ksVect.push_back(materialObjects[i].Ks.X);
       obj.ksVect.push_back(materialObjects[i].Ks.Y);
       obj.ksVect.push_back(materialObjects[i].Ks.Z);

       obj.kdVect.push_back(materialObjects[i].Kd.X);
       obj.kdVect.push_back(materialObjects[i].Kd.Y);
       obj.kdVect.push_back(materialObjects[i].Kd.Z);

       obj.nsVect.push_back(materialObjects[i].Ns);
   }
   
    return 0;
}