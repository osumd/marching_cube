#ifndef CUBE_BUFFER_H
#define CUBE_BUFFER_H 

#include <glmmath.h>

#define MAX_VISIBLE_CUBES 4

typedef struct 
{
    glm::ivec3 cubeIndex[MAX_VISIBLE_CUBES];
    int        n_visible_cubes;
} cubeBuffer;


#endif