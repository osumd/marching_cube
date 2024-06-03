#ifndef DIRECTED_SCENE_CAMERA_H
#define DIRECTED_SCENE_CAMERA_H 

#include <OpenGLObjectTools.h>

void dsc_mousecallback(void* self, double xpos, double ypos);
void dsc_proccessInput(void* self, GLFWwindow* window);
void dsc_MakeActiveCamera(void* self, GLFWwindow* window);
glm::vec2 dsc_ScreenToWorld(void* self, GLFWwindow* window, float xpos, float ypos);
void dsc_mouse_callback(GLFWwindow* window, double xpos, double ypos);

struct DirectedSceneCamera
{
    cameraBaseClass super;

    cameraMatricies matricies;

    cf_bvol         trackedObject;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    
    float cameraSpeed = 0.5f;

    float fov = 45;

    float currentFrame = 0.0f;
    float lastFrame = 0.0;
    float deltaTime = 0.0;

    float near_dist = 0.1f;
    float far_dist = 100.0f;
    float cameraZoom = 1.0f;

    DirectedSceneCamera() : trackedObject({glm::vec3(0,0,0), glm::vec3(0,0,0)})
    {
        super.self = this;
        super.matricies = &matricies;
        super.MakeActiveCamera = dsc_MakeActiveCamera;
        super.proccessInput = dsc_proccessInput;
        super.ScreenToWorld = dsc_ScreenToWorld;
        super.mousecallback = dsc_mousecallback;
    }

    //Track new object.

    void TrackObject(cf_bvol _trackedObject)       //Determine which object is currently being tracked.
    {

    }

    void SetRelativeLocation(const glm::vec3 displacement)
    {   

        glm::vec3 trackedCenter;
        bvol_center(trackedObject, trackedCenter);

        cameraPos = trackedCenter + displacement;
    
    }

    void SetAngularLocation(const glm::vec3 radians)
    {
        glm::vec3 trackedCenter;
        bvol_center(trackedObject, trackedCenter);


    }

    void UpdateCameraMetrics()
    {
        
    }
    
    ~DirectedSceneCamera()
    {

    }

};

void dsc_proccessInput(void* self, GLFWwindow* window)
{
    DirectedSceneCamera* s = (DirectedSceneCamera*)((cameraBaseClass*)self)->self;
    
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    //s->cameraPos

    //glm::mat4



    // s->currentFrame = glfwGetTime();
    // s->deltaTime = s->currentFrame - s->lastFrame;
    // s->lastFrame = s->currentFrame; 
    // s->cameraSpeed = 2.0f * s->deltaTime;
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     s->cameraPos += s->cameraSpeed * s->cameraFront;
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     s->cameraPos -= s->cameraSpeed * s->cameraFront;
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     s->cameraPos -= glm::normalize(glm::cross(s->cameraFront, s->cameraUp)) * s->cameraSpeed;
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     s->cameraPos += glm::normalize(glm::cross(s->cameraFront, s->cameraUp)) * s->cameraSpeed;

    // glfwGetCursorPos(window, &s->curPosx, &s->curPosy);

    // s->matricies.projection = glm::perspective(glm::radians(s->fov), windowContext->windowDimensions.vx / windowContext->windowDimensions.vy, 0.1f, 100.0f);
    //s->matricies.view = glm::lookAt(s->cameraPos, s->cameraPos + s->cameraFront, s->cameraUp);
    
}

void dsc_mousecallback(void* self, double xpos, double ypos)
{
    DirectedSceneCamera* activeCamera = (DirectedSceneCamera*)((cameraBaseClass*)self)->self;

    // if(activeCamera == nullptr){return;}

    // if (activeCamera->firstMouse)
    // {
    //     activeCamera->lastX = xpos;
    //     activeCamera->lastY = ypos;
    //     activeCamera->firstMouse = false;
    // }

    // float xoffset = xpos - activeCamera->lastX;
    // float yoffset = activeCamera->lastY - ypos; 
    // activeCamera->lastX = xpos;
    // activeCamera->lastY = ypos;

    // float sensitivity = 0.1f;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // activeCamera->yaw   += xoffset;
    // activeCamera->pitch += yoffset;

    // if(activeCamera->pitch > 89.0f)
    //     activeCamera->pitch = 89.0f;
    // if(activeCamera->pitch < -89.0f)
    //     activeCamera->pitch = -89.0f;

    // glm::vec3 direction;
    // direction.x = cos(glm::radians(activeCamera->yaw)) * cos(glm::radians(activeCamera->pitch));
    // direction.y = sin(glm::radians(activeCamera->pitch));
    // direction.z = sin(glm::radians(activeCamera->yaw)) * cos(glm::radians(activeCamera->pitch));
    // activeCamera->cameraFront = glm::normalize(direction);
}

void dsc_MakeActiveCamera(void* self, GLFWwindow* window)
{
    DirectedSceneCamera* s = (DirectedSceneCamera*)self;
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);
    windowContext->activeCamera = &s->super;

    glfwSetCursorPosCallback(window, mouse_callback);
}

glm::vec2 dsc_ScreenToWorld(void* self, GLFWwindow* window, float xpos, float ypos)
{
    DirectedSceneCamera* s = (DirectedSceneCamera*)self;
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    // float ndcX = (2.0f * xpos) / windowContext->windowDimensions.vx - 1.0f;
    // float ndcY = 1.0f - (2.0f * ypos) / windowContext->windowDimensions.vy;
    // glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    // glm::mat4 iV = glm::inverse(s->matricies.view);
    // glm::mat4 invProjection = glm::inverse(s->matricies.projection);
    // glm::vec4 rayEye = invProjection * rayClip;
    // rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Z points forward
    // glm::mat4 invView = glm::inverse(s->matricies.view);
    // glm::vec4 rayWorld = invView * rayEye;
    // return glm::vec2(0,0);
}

void dsc_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);
    if(windowContext == nullptr){return;}
    cameraBaseClass* activeCamera = windowContext->activeCamera;

    activeCamera->mousecallback(activeCamera, xpos, ypos);
}
#endif