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

#include "DirectedSceneCamera.h"
#include "InstanceUniformBuffer.h"
#include "SphereInstance.h"


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

void GenerateUVSphere()
{
    //Meshing

    glm::vec3 center = glm::vec3(0,0,0);
    float r = 1;

    float subdivisions = 12;
    float phi = 0.0;
    float phip = 0.0;
    float theta = 0.0;
    float thetap = 0.0;

    float thetastep = (2*3.14)/subdivisions;
    float phistep = (3.14)/subdivisions;

    int memoryDataBack = 0;
    float* bufferData = (float*)malloc(4*((4*8*subdivisions) + (2*8*(subdivisions*subdivisions)))); //subdivisions
    unsigned int* indexData = (unsigned int*)malloc(4*6*(subdivisions)*(subdivisions));
    int elementDataBack = 0;
    int j = 0;

    int anchor0 = 0;
    int anchor1 = 0;

    for(int y = 0; y < subdivisions; y++)
    {
        phi = y*phistep;
        phip = (y+1)*phistep;

        for(int x = 0; x < subdivisions; x++)
        {
            theta = x*thetastep;
            thetap = (x+1)*thetastep;

            glm::vec3 c0 = center + glm::vec3(cosf(theta) * sinf(phi), sinf(theta) * sinf(phi), cosf(phi)); 
            glm::vec3 c1 = center + glm::vec3(cosf(thetap) * sinf(phi), sinf(thetap) * sinf(phi), cosf(phi));
            glm::vec3 c2 = center + glm::vec3(cosf(thetap) * sinf(phip), sinf(thetap) * sinf(phip), cosf(phip));
            glm::vec3 c3 = center + glm::vec3(cosf(theta) * sinf(phip), sinf(theta) * sinf(phip), cosf(phip));


            //fTriQuad(c0,c1,c2,c3, bufferData, memoryDataBack, indexData, j, elementDataBack);
            if(x == 0)
            {
                NHolder = glm::cross((c1-c0), (c3-c0));

                fPoint(c0, 0,0, bufferData, memoryDataBack);
                fPoint(c1, 1,0, bufferData, memoryDataBack);
                fPoint(c2, 1,1, bufferData, memoryDataBack);
                fPoint(c3, 0,1, bufferData, memoryDataBack);

                indexData[elementDataBack++] = j;
                indexData[elementDataBack++] = j+1;
                indexData[elementDataBack++] = j+2;

                indexData[elementDataBack++] = j;
                indexData[elementDataBack++] = j+2;
                indexData[elementDataBack++] = j+3;

                anchor0 = j+1;
                anchor1 = j+2;

                j+=4;
            }else
            {
                NHolder = glm::cross((c1-c0), (c3-c0));

                fPoint(c1, 1,0, bufferData, memoryDataBack);
                fPoint(c2, 1,1, bufferData, memoryDataBack);
                
                indexData[elementDataBack++] = anchor0;
                indexData[elementDataBack++] = j;
                indexData[elementDataBack++] = j+1;

                indexData[elementDataBack++] = anchor0;
                indexData[elementDataBack++] = j+1;
                indexData[elementDataBack++] = anchor1; 

                anchor0 = j;
                anchor1 = j+1;

                j+=2; 
            }
  
        }

    }


    int directoryFetchError = 0;
    FILE* mesh_eb = getFileWithDirectory("Meshes/uvsphere.mb", 1, "wb", &directoryFetchError);

    if(directoryFetchError != 0){
        free(bufferData);
        free(indexData);
        return;
    }

    
    fwrite(&memoryDataBack,sizeof(int), 1, mesh_eb);
    fwrite(bufferData, sizeof(float), memoryDataBack, mesh_eb);
    fwrite(&elementDataBack, sizeof(int), 1, mesh_eb);
    fwrite(indexData, sizeof(unsigned int), elementDataBack, mesh_eb);

    fclose(mesh_eb);

    free(bufferData);
    free(indexData);

    return;
}

vapeb ReopenUVSphere()
{
    unsigned int vao;
    unsigned int vb;
    unsigned int ebo;

    float* bufferData;
    unsigned int* indexData;
    int directoryFetchError = 0;
    
    FILE* mesh_eb = getFileWithDirectory("Meshes/uvsphere.mb", 1, "rb", &directoryFetchError);
    fseek(mesh_eb, 0, SEEK_SET);

    if (directoryFetchError != 0) {
        return {0,0,0,0};
    }

    // Read the sizes
    int readMemorySize = 0;
    int readElementSize = 0;

    fread(&readMemorySize, sizeof(int), 1, mesh_eb);
    if(readMemorySize == 0 || readMemorySize < 0){return {0,0,0,0}; }
    bufferData = (float*)malloc(4*readMemorySize);
    fread(bufferData, sizeof(float), readMemorySize, mesh_eb);
    fread(&readElementSize, sizeof(int), 1, mesh_eb);
    indexData = (unsigned int*)malloc(4*readElementSize);
    if(readElementSize == 0 || readElementSize < 0){return {0,0,0,0}; }
    fread(indexData, sizeof(unsigned int), readElementSize, mesh_eb);


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vb);
    glGenBuffers(1, &ebo);

    unsigned int program = CreateShader("Objects/InstancedGeometry/instance.vs", "Objects/InstancedGeometry/instance.fs");

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (readElementSize)*sizeof(unsigned int), indexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, (readMemorySize+1)*sizeof(float), bufferData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 5));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(bufferData);
    free(indexData);

    return {program, vao, ebo, readElementSize};

}


int main()
{
    GLFWwindow* window = CreateContext();

    FirstPersonCamera mainCamera;
    mainCamera.super.MakeActiveCamera(&mainCamera, window);
    
    DirectedSceneCamera DirectedCamera;
    cameraBaseClass activeCamera = mainCamera.super;
    GraphicVolume currentBox({{0.0,0.0,0.0},{1.0,1.0,1.0}});

    //Sphere Program Development
    vapeb  GraphicSphere =  ReopenUVSphere();

    //Sphere Transform Location
    SphereTransform sphereTransformBuffer[2] = {
        {glm::vec3(0.0,0.0,0.0),1.0},
        {glm::vec3(0.0,-0.5,0.0),1.0},
    };

    //Instance Practice
    

    //Instance Development
    InstanceUniformBuffer   SphereBuffer;
    SphereBuffer.m_type = sizeof(SphereTransform);
    SphereBuffer.AllocateUniformBuffer(sizeof(SphereTransform)*2);
    SphereBuffer.SetTransformsData(sphereTransformBuffer);
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        activeCamera.proccessInput(&activeCamera, window);
        currentBox.Draw(activeCamera);

        GraphicSphere.DrawInstanced(activeCamera, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);

    return 1;
}