#ifndef RELATIVE_OBJECT_H
#define RELATIVE_OBJECT_H 

#include "glmmath.h"

struct ArgumentPackage
{
    glm::vec3 Verticies[5];
    double Paramters[3];
};

struct RelativeObject
{
    void(*callback)(void* self, ArgumentPackage package);
    void* self;
};

#endif