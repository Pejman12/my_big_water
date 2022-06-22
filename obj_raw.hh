#ifndef OBJECT_RAW_HEADER
#define OBJECT_RAW_HEADER

#include "obj_loader.hh"

#include <GL/glew.h>
#include <stdio.h>
#include <vector>

enum index {
    POSITION = 0,
    NORMAL = 1,
    KA = 2,
    KD = 3,
    KS = 4,
    NS = 5,
};

struct objRaw
{
    std::string name;
    std::vector<std::string> vbos_names{"position", "normal", "ka", "kd", "ks", "ns"};
    std::vector<std::vector<GLfloat>> vecs;
};

void getVectorFromMesh(const objl::Mesh &mesh, objRaw &obj);
void getMaterialFromMesh(const objl::Mesh &material, objRaw &obj);
std::vector<objRaw> addObjs(const std::string filename);

#endif