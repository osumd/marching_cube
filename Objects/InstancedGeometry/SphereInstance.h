#ifndef Sphere_INSTANCE
#define Sphere_INSTANCE

#include "InstanceUniformBuffer.h"

struct SphereTransform{
    glm::vec3              location;
    float                  scale;
};

struct SphereInstance{

    InstanceUniformBuffer* uniformData;
    int*                   sphereInstanceCount;
    SphereTransform        transform;

    SphereInstance(InstanceUniformBuffer* _uniformData, int* _sphereInstanceCount ) : sphereInstanceCount(_sphereInstanceCount), uniformData(_uniformData)
    {

    }

    ~SphereInstance()
    {

    }

};


#endif