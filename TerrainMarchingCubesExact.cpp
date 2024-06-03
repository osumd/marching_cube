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


    //Load textures.
    unsigned int noiseVol0 = volTexture("MarchingCubes/textures/noise_half_16cubed_mips_00.vol");
    unsigned int noiseVol1 = volTexture("MarchingCubes/textures/noise_half_16cubed_mips_01.vol");
    unsigned int noiseVol2 = volTexture("MarchingCubes/textures/noise_half_16cubed_mips_02.vol");
    unsigned int noiseVol3 = volTexture("MarchingCubes/textures/noise_half_16cubed_mips_03.vol");
    unsigned int packedNoiseVol0 = volTexture("MarchingCubes/textures/packednoise_half_16cubed_mips_00.vol");
    unsigned int packedNoiseVol1 = volTexture("MarchingCubes/textures/packednoise_half_16cubed_mips_01.vol");
    unsigned int packedNoiseVol2 = volTexture("MarchingCubes/textures/packednoise_half_16cubed_mips_02.vol");
    unsigned int packedNoiseVol3 = volTexture("MarchingCubes/textures/packednoise_half_16cubed_mips_03.vol");

    unsigned int densityShader = CreateShaderWithIncludes("MarchingCubes/shaders/build_density_vol.vs","MarchingCubes/shaders/build_density_vol.fs","MarchingCubes/shaders/build_density_vol.gs");

    unsigned int nonempty_list_shader = PrelinkShaderWithIncludes("MarchingCubes/shaders/list_nonempty_cells.vs","MarchingCubes/shaders/list_nonempty_cells.gs");
    const char* nonempty_list_shader_varying[1] = {"outData.z8_y8_x8_case8"};
    StreamOutputBuffer(200, nonempty_list_shader_varying, 1, nonempty_list_shader);

    unsigned int list_vertices_to_generate_shader = PrelinkShaderWithIncludes("MarchingCubes/shaders/list_vertices_to_generate.vs","MarchingCubes/shaders/list_vertices_to_generate.gs");
    const char* list_vertices_to_generate_shader_varying[1] = {"outData.z8_y8_x8_null4_edgeNum4"};
    StreamOutputBuffer(200, list_vertices_to_generate_shader_varying, 1, list_vertices_to_generate_shader);

    setTexture3D(densityShader, "noiseVol0", 0, noiseVol0);
    setTexture3D(densityShader, "noiseVol1", 1, noiseVol1);
    setTexture3D(densityShader, "noiseVol2", 2, noiseVol2);
    setTexture3D(densityShader, "noiseVol3", 3, noiseVol3);
    setTexture3D(densityShader, "packedNoiseVol0", 4, packedNoiseVol0);
    setTexture3D(densityShader, "packedNoiseVol1", 5, packedNoiseVol1);
    setTexture3D(densityShader, "packedNoiseVol2", 6, packedNoiseVol2);
    setTexture3D(densityShader, "packedNoiseVol3", 7, packedNoiseVol3);

    //Set up uniform buffers
    LodCB lodCB = {
        8,
        7,
        glm::vec2(1/61),
        1.0,
        glm::vec2(1/8,1/8),
        glm::vec2(1/7,1/7),
        0.1,
        0.1,
        0.1,
        glm::vec2(1/8 + 0.1, 1/8 + 0.1),
        glm::vec2(1/8 + 0.1, 1/7 + 0.1)
    };

    float voxel_uvs[32*2]
    {
        0/32,0/32,
        1/32,1/32,
        2/32,2/32,
        3/32,3/32,
        4/32,4/32,
        5/32,5/32,
        6/32,6/32,
        7/32,7/32,
        8/32,8/32,
        9/32,9/32,
        10/32,10/32,
        11/32,11/32,
        12/32,12/32,
        13/32,13/32,
        14/32,14/32,
        15/32,15/32,
        16/32,16/32,
        17/32,17/32,
        18/32,18/32,
        19/32,19/32,
        20/32,20/32,
        21/32,21/32,
        22/32,22/32,
        23/32,23/32,
        24/32,24/32,
        25/32,25/32,
        26/32,26/32,
        27/32,27/32,
        28/32,28/32,
        29/32,29/32,
        30/32,30/32,
        31/32,31/32,
        
    };

    unsigned int lodCBBuffer = 0;
    glGenBuffers(1, &lodCBBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lodCBBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LodCB), &lodCB, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_UNIFORM_BUFFER, lodCBBuffer);
    glBindBufferBase(lodCBBuffer, GL_UNIFORM_BUFFER, 1);
    setUniformBlock(densityShader, "LodCB", 8);
    

    //Set up cameras however. wtf?? why did I do this
    FirstPersonCamera mainCamera;
    mainCamera.super.MakeActiveCamera(&mainCamera, window);
    cameraBaseClass activeCamera = mainCamera.super;

    glUseProgram(densityShader);

    vaeb scrqd = screenQuad();

    unsigned int vDensityTexture = GenerateTexture3D(33,33,33,GL_RGBA, GL_RGBA, GL_FLOAT);
    
    shad_world_block currentWorldBlock;
    unsigned int worldBlockUniform;
    glGenBuffers(1, &worldBlockUniform);
    glBindBuffer(GL_ARRAY_BUFFER, worldBlockUniform);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shad_world_block),  &currentWorldBlock,  GL_STATIC_DRAW);

    glm::vec4 viewportDim = glm::vec4(1024, 768, 0,0);

    unsigned int viewPortDimUniform;
    glGenBuffers(1, &viewPortDimUniform);
    glBindBuffer(GL_ARRAY_BUFFER, viewPortDimUniform);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &viewPortDimUniform, GL_STATIC_DRAW);

    FindWorldBlocks(mainCamera);

    glm::vec3 prevCameraPos = glm::vec3(0,0,0);
    glm::vec3 prevCameraDir = glm::vec3(0,0,0);

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(1.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        //If camera has moved

        activeCamera.proccessInput(&mainCamera, window);

        if(prevCameraDir != mainCamera.cameraFront || prevCameraPos != mainCamera.cameraPos)
        {
            FindWorldBlocks(mainCamera);
            SortWorldBlocks(mainCamera);

            glBindVertexArray(scrqd.vertexarray);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scrqd.elementbuffer);

            for(int i = 0; i < global_cube_buffer.n_visible_cubes; i++)
            {
                currentWorldBlock.wcoord = glm::vec4(global_cube_buffer.cubeIndex[i].x,global_cube_buffer.cubeIndex[i].y,global_cube_buffer.cubeIndex[i].z, 1.0);
                
                glBindBuffer(GL_ARRAY_BUFFER,  worldBlockUniform);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shad_world_block), &currentWorldBlock);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_UNIFORM_BUFFER, worldBlockUniform);
                glBindBufferBase(worldBlockUniform, GL_UNIFORM_BUFFER, 9);
                setUniformBlock(densityShader, "ChunkCB", 9);

                glBindBuffer(GL_ARRAY_BUFFER,  viewPortDimUniform);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4), &viewportDim);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_UNIFORM_BUFFER, viewPortDimUniform);
                glBindBufferBase(viewPortDimUniform, GL_UNIFORM_BUFFER, 10);
                setUniformBlock(densityShader, "MyCB", 10);

                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 32);


                //Density texture has been filled for

                //Now to list non empty cells
                //
            }

            

            //deploy a vertex shader invocation 33 times for each layer
            //iterate 33x33, 

            prevCameraDir = mainCamera.cameraFront;
            prevCameraPos = mainCamera.cameraPos;



        }

        




        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);

}