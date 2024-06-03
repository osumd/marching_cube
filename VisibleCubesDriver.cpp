#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "OpenGLTools/OpenGLToolBox.h"
#include "PerspectiveFPSCamera.h"
#include <GraphicVector.h>
#include <marchingCubesHeader.h>

#include <algorithm>

#include "graphicObjectsHeader.h"


void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
    throw("bad");
}
void FrameBufferResize(GLFWwindow *window, int width, int height)
{
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    glViewport(0, 0, width, height);
    windowContext->windowDimensions.vx = width;
    windowContext->windowDimensions.vy = height;
}
void WindowResize(GLFWwindow *window, int width, int height)
{
    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);

    windowContext->windowDimensions.wx = width;
    windowContext->windowDimensions.wy = height;
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    WindowContext* windowContext = (WindowContext*)glfwGetWindowUserPointer(window);
    if(!windowContext)
    {
        return;
    }
    cameraBaseClass* activeCamera = windowContext->activeCamera;
    double xpos,ypos;
    glfwGetCursorPos(window,  &xpos, &ypos);

    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //GlobalInputContainer.SendClick(window, activeCamera, xpos, ypos);
    }
}

GLFWwindow* CreateContext()
{
    bool debugO = true;
    if(!glfwInit()){return 0;}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //glfwWindowHint(GLFW_SAMPLES, 4);
    if(debugO)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        //throw std::range_error("Did not load context of window.");
    }

    WindowContext* windowContext = new WindowContext;
    windowContext->activeCamera = 0;
    windowContext->windowDimensions.wx = 1024;
    windowContext->windowDimensions.wy = 768;
    windowContext->windowDimensions.vx = 1024;
    windowContext->windowDimensions.vy = 768;

    glfwSetWindowUserPointer(window, windowContext);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    
    if(glewInit() != GLEW_OK)
    {
        printf("%s\n", "GLEW not initialized correctly.");
    }

    if(debugO)
    {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
     }

    
    //glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, FrameBufferResize);
    glfwSetWindowSizeCallback(window, WindowResize);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    


    return window;
}

int main()
{
    GLFWwindow* window = CreateContext();

    FirstPersonCamera alternativeCamera;
    alternativeCamera.FixCamera({0,4,4},{0,-1,-1}, window);

    FirstPersonCamera mainCamera;
    mainCamera.super.MakeActiveCamera(&mainCamera, window);
    //alternativeCamera.super.MakeActiveCamera(&alternativeCamera, window);

    cameraBaseClass activeCamera = mainCamera.super;
    cameraBaseClass altCamera = alternativeCamera.super;

    GraphicFrustrum cFrame;

    std::vector<GraphicVolume> worldBlocks;
    
    bool selectedCamera = false;

    GraphicVector  debugVectors[3]; graphicVectors = debugVectors;
    usedGraphicVectors = 1;
     
    GraphicVolume currentBox({{0.0,0.0,0.0},{1.0,1.0,1.0}});
    glm::vec3 cubeIndex = glm::vec3(global_cube_buffer.cubeIndex[0].x, global_cube_buffer.cubeIndex[0].y, global_cube_buffer.cubeIndex[0].z);
    currentBox.m_volume = {{ cubeIndex + glm::vec3(-0.5,0.5,0.5)},{cubeIndex + glm::vec3(0.5, -0.5, -0.5)}};

    GraphicVolume myBox({{0.0,0.0,0.0},{1.0,1.0,1.0}});

    FindWorldBlocks(mainCamera);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        activeCamera.proccessInput(&activeCamera, window);

        FindWorldBlocks(mainCamera);
        
        
        cubeIndex = glm::vec3(global_cube_buffer.cubeIndex[0].x, global_cube_buffer.cubeIndex[0].y, global_cube_buffer.cubeIndex[0].z);
        currentBox.m_volume = {{ cubeIndex + glm::vec3(-0.5,0.5,0.5)},{cubeIndex + glm::vec3(0.5, -0.5, -0.5)}};
        //Render quadrant camera is currently in.
        currentBox.ReconstructBox();
        currentBox.Draw(altCamera);

        cFrame.UpdateFrustrum(mainCamera.cameraPos, mainCamera.cameraFront, mainCamera.cameraUp, 0.1, 0.4, mainCamera.fov);
        cFrame.Draw(altCamera);
        
        
        cf_8vec cameraFrustrum = CreateFrustrum(mainCamera.cameraPos, mainCamera.cameraFront, mainCamera.cameraUp, mainCamera.fov, mainCamera.fov, 0.1, 0.4);
        
        FrustrumCube(cameraFrustrum, myBox.m_volume);
        myBox.ReconstructBox();
        myBox.Draw(altCamera);


        //myVector.Draw(altCamera);
        //myVector.ReconstructVector();
        
        //GraphicVectors[0].ReconstructVector();
        //GraphicVectors[0].Draw(altCamera);
        
        for(int i = 0; i < usedGraphicVectors; i++)
        {
            graphicVectors[i].Draw(altCamera);
        }

        


        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);
}