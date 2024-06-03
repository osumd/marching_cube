#ifndef CAMERA_BASE_CLASS_H
#define CAMERA_BASE_CLASS_H

#include <glmmath.h>


typedef struct
{
    glm::mat4 projection;
    glm::mat4 view;
} cameraMatricies;

typedef struct 
{
    void (*proccessInput)(void* self, GLFWwindow* window);
    void (*mousecallback)(void* self, double xpos, double ypos);
    void (*MakeActiveCamera)(void* self, GLFWwindow* window);
    glm::vec2 (*ScreenToWorld)(void* self, GLFWwindow* window, float xpos, float ypos);
    cameraMatricies* matricies;
    void* self;
} cameraBaseClass;




#endif