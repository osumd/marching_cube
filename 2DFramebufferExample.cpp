#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "OpenGLTools/OpenGLToolBox.h"
#include "PerspectiveFPSCamera.h"
#include <GraphicVector.h>
#include <marchingCubesHeader.h>

#include <algorithm>



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
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }   
        
        
    
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

struct FeedbackBuffer
{
    unsigned int buffer;
    unsigned int transfromFeedback;
};

FeedbackBuffer StreamOutputBuffer(int size, const char** varyings, int varyingCount, unsigned int program)
{
    GLuint feedbackBuffer;
    glGenBuffers(1, &feedbackBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, feedbackBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
    

    GLuint transformFeedback;
    glGenTransformFeedbacks(1, &transformFeedback);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);

    // Bind the buffer to the first binding point
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackBuffer);

    // Specify the output variables
    const char* feedbackVaryings[] = { "position", "color" }; // Add other output variables as needed

    glTransformFeedbackVaryings(program, varyingCount, feedbackVaryings, GL_SEPARATE_ATTRIBS);

    // Link the program after setting transform feedback varyings
    glLinkProgram(program); //Possibly used on pre linked program
    GLint  linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){ throw("fuck"); }

    // Unbind transform feedback
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    return {transformFeedback, transformFeedback};

}

struct LodCB
{
  float  VoxelDim ;          // # of cell corners
  float  VoxelDimMinusOne ;  // # of cells
  glm::vec2 wsVoxelSize ;  // could be 1/63, 1/31, 1/15 depending on LOD
  float  wsChunkSize ;                  // 1.0, 2.0, or 4.0 depending on LOD
  glm::vec2 InvVoxelDim ; 
  glm::vec2 InvVoxelDimMinusOne ; 
  float  Margin                      ;
  float  VoxelDimPlusMargins         ;
  float  VoxelDimPlusMarginsMinusOne ;
  glm::vec2 InvVoxelDimPlusMargins         ;
  glm::vec2 InvVoxelDimPlusMarginsMinusOne ;
};

int main()
{
    
    GLFWwindow* window = CreateContext();

    unsigned int density_shader = CreateShader("MarchingCubes/examples/dens.vs", "MarchingCubes/examples/dens.fs");
    
    unsigned int density_viewer = CreateShader("MarchingCubes/shaders/density_view.vs", "MarchingCubes/shaders/density_view.fs"); 

    vaeb screen_quad = screenQuad();

    int dimx = 48;
    int dimy = 48;

    unsigned char* data = (unsigned char*)malloc(dimx*dimy*3);
    for(int i = 0; i < dimx*dimy*3; i++)
    {
        data[i] = 255;
    }

    // unsigned int fbo;
    // glGenFramebuffers(1, &fbo);
    // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimx, dimy, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // attach it to currently bound framebuffer object

    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    // {
    //     printf("Bad texture\n");
    // }
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(screen_quad.vertexarray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad.elementbuffer);

        glUseProgram(density_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, dimx, dimy);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glUseProgram(density_viewer);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE0);
        setInt(density_viewer, "density", 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1024, 768);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);

}