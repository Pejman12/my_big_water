#ifndef OBJECT_RAW_HEADER
#define OBJECT_RAW_HEADER

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

void getVectorFromMesh(objl::Mesh mesh, objRaw obj);
void getMaterialFromMesh(objl::Mesh material, objRaw obj);
std::vector<objRaw> addObjs(const std::string filename);

#endif