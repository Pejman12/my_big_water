#include "obj_loader.hh"

#include <GL/glew.h>
#include <stdio.h>
#include <vector>

std::vector<GLfloat> vertex_buff_data;
std::vector<GLfloat> normal_buff_data;

void getVectorFromMesh(objl::Mesh mesh) {

    std::cout << mesh.MeshName << std::endl;

    for (int i = 0; i < mesh.Vertices.size(); ++i) {
        vertex_buff_data.push_back(mesh.Vertices[i].Position.X);
        vertex_buff_data.push_back(mesh.Vertices[i].Position.Y);
        vertex_buff_data.push_back(mesh.Vertices[i].Position.Z);

        normal_buff_data.push_back(mesh.Vertices[i].Normal.X);
        normal_buff_data.push_back(mesh.Vertices[i].Normal.Y);
        normal_buff_data.push_back(mesh.Vertices[i].Normal.Z);
    }
}

int main() {
    objl::Loader loader;
    loader.LoadFile("Erlaufsee.obj");

    for (int i = 0; i < loader.LoadedMeshes.size(); i++)
    {
        getVectorFromMesh(loader.LoadedMeshes[i]);
    }

    /*
    // Display vertex_buff_data
    std::cout << "vertex_buff_data :" << std::endl;
    for (int i = 0; i < vertex_buff_data.size(); ++i) {
        std::cout << vertex_buff_data[i] << ", ";
    }
    // Display normal_buff_data
    std::cout << '\n' << "normal_buff_data :" << std::endl;
    for (int i = 0; i < normal_buff_data.size(); ++i) {
        std::cout << normal_buff_data[i] << ", ";
    }
    */
    return 0;
}