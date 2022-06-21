#include "obj_loader.hh"
#include <GL/glew.h>
#include <stdio.h>
#include <vector>




int main() {
    objl::Loader loader;
    loader.LoadFile("Erlaufsee.obj");
    auto vertices_vector = loader.LoadedMeshes[0].Vertices;
    std::vector<GLfloat> vertex_buff_data;
    std::vector<GLfloat> normal_buff_data;
    for (int i = 0; i < vertices_vector.size(); ++i) {
        vertex_buff_data.push_back(vertices_vector[i].Position.X);
        vertex_buff_data.push_back(vertices_vector[i].Position.Y);
        vertex_buff_data.push_back(vertices_vector[i].Position.Z);

        normal_buff_data.push_back(vertices_vector[i].Normal.X);
        normal_buff_data.push_back(vertices_vector[i].Normal.Y);
        normal_buff_data.push_back(vertices_vector[i].Normal.Z);
    }

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

    return 0;
}