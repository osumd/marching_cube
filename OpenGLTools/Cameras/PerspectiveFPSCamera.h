#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H


#include <OpenGLToolBox.h>


void fps_mousecallback(void* self, double xpos, double ypos);
void fps_proccessInput(void* self, GLFWwindow* window);
void fps_MakeActiveCamera(void* self, GLFWwindow* window);
glm::vec2 fps_ScreenToWorld(void* self, GLFWwindow* window, float xpos, float ypos);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

struct FirstPersonCamera
{

    cameraBaseClass super;

    cameraMatricies matricies;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    
    float cameraSpeed = 0.5f;
    float lastX = 320, lastY = 240;
    float yaw = -90, pitch = 0;
    float fov = 45;
    bool firstMouse = true;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    double curPosx = 0.0f;
    double curPosy = 0.0f;
    float currentFrame = 0.0f;
    float lastFrame = 0.0;
    float deltaTime = 0.0;
    float near_dist = 0.1f;
    float far_dist = 100.0f;
    float cameraZoom = 1.0f;

    FirstPersonCamera()
    {
        super.self = this;
        super.matricies = &matricies;
        super.MakeActiveCamera = fps_MakeActiveCamera;
        super.proccessInput = fps_proccessInput;
        super.ScreenToWorld = fps_ScreenToWorld;
        super.mousecallback = fps_mousecallback;
    }

    void FixCamera(glm::vec3 pos, glm::vec3 direction, GLFWwindow* window)
    {
        WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

        cameraPos = pos;
        cameraFront = direction;

        // if (firstMouse   )
        // {
        //     activeCamera->lastX = xpos;
        //     activeCamera->lastY = ypos;
        //     activeCamera->firstMouse = false;
        // }

        matricies.projection = glm::perspective(glm::radians(fov), windowContext->windowDimensions.vx / windowContext->windowDimensions.vy, 0.1f, 100.0f);
        matricies.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }




    ~FirstPersonCamera()
    {

    }

};

void fps_proccessInput(void* self, GLFWwindow* window)
{
    FirstPersonCamera* s = (FirstPersonCamera*)((cameraBaseClass*)self)->self;
    
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    s->currentFrame = glfwGetTime();
    s->deltaTime = s->currentFrame - s->lastFrame;
    s->lastFrame = s->currentFrame; 
    s->cameraSpeed = 2.0f * s->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        s->cameraPos += s->cameraSpeed * s->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        s->cameraPos -= s->cameraSpeed * s->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        s->cameraPos -= glm::normalize(glm::cross(s->cameraFront, s->cameraUp)) * s->cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        s->cameraPos += glm::normalize(glm::cross(s->cameraFront, s->cameraUp)) * s->cameraSpeed;

    glfwGetCursorPos(window, &s->curPosx, &s->curPosy);

    s->matricies.projection = glm::perspective(glm::radians(s->fov), windowContext->windowDimensions.vx / windowContext->windowDimensions.vy, 0.1f, 100.0f);
    s->matricies.view = glm::lookAt(s->cameraPos, s->cameraPos + s->cameraFront, s->cameraUp);
    
}

void fps_mousecallback(void* self, double xpos, double ypos)
{
    FirstPersonCamera* activeCamera = (FirstPersonCamera*)((cameraBaseClass*)self)->self;

    if(activeCamera == nullptr){return;}

    if (activeCamera->firstMouse)
    {
        activeCamera->lastX = xpos;
        activeCamera->lastY = ypos;
        activeCamera->firstMouse = false;
    }

    float xoffset = xpos - activeCamera->lastX;
    float yoffset = activeCamera->lastY - ypos; 
    activeCamera->lastX = xpos;
    activeCamera->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    activeCamera->yaw   += xoffset;
    activeCamera->pitch += yoffset;

    if(activeCamera->pitch > 89.0f)
        activeCamera->pitch = 89.0f;
    if(activeCamera->pitch < -89.0f)
        activeCamera->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(activeCamera->yaw)) * cos(glm::radians(activeCamera->pitch));
    direction.y = sin(glm::radians(activeCamera->pitch));
    direction.z = sin(glm::radians(activeCamera->yaw)) * cos(glm::radians(activeCamera->pitch));
    activeCamera->cameraFront = glm::normalize(direction);
}

void fps_MakeActiveCamera(void* self, GLFWwindow* window)
{
    FirstPersonCamera* s = (FirstPersonCamera*)self;
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);
    windowContext->activeCamera = &s->super;
    glfwSetCursorPosCallback(window, mouse_callback);
}

glm::vec2 fps_ScreenToWorld(void* self, GLFWwindow* window, float xpos, float ypos)
{
    FirstPersonCamera* s = (FirstPersonCamera*)self;
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    float ndcX = (2.0f * xpos) / windowContext->windowDimensions.vx - 1.0f;
    float ndcY = 1.0f - (2.0f * ypos) / windowContext->windowDimensions.vy;
    glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::mat4 iV = glm::inverse(s->matricies.view);
    glm::mat4 invProjection = glm::inverse(s->matricies.projection);
    glm::vec4 rayEye = invProjection * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Z points forward
    glm::mat4 invView = glm::inverse(s->matricies.view);
    glm::vec4 rayWorld = invView * rayEye;
    return glm::vec2(0,0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);
    if(windowContext == nullptr){return;}
    cameraBaseClass* activeCamera = windowContext->activeCamera;

    activeCamera->mousecallback(activeCamera, xpos, ypos);
}





#endif