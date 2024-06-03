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

struct vertex_layout
{
    GLenum data_type;
    int type_size;
    int count;
};

struct array_mesh
{
    unsigned int vertex_buffer;
    unsigned int vertex_array;

    float* vertex_data;


    float* m_vertex_data;
    int m_float_count;
    vertex_layout* m_layouts;
    int m_layout_count;
    int m_layout_total;
    
    

    array_mesh( float* vertex_data, int float_count, vertex_layout* layouts, int layout_count, int layout_total )
    {

        m_vertex_data = vertex_data;
        m_float_count = float_count;
        m_layouts = layouts;
        m_layout_count = layout_count;
        m_layout_total = layout_total;

        generate_buffers();
    
    }

    void generate_buffers()
    {
        glGenVertexArrays(1, &vertex_array);
        glGenBuffers(1, &vertex_buffer);
        glBindVertexArray(vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, m_float_count*sizeof(float), m_vertex_data, GL_STATIC_DRAW);

        int bytes_stride = 0;
        for(int i = 0; i < m_layout_count; i++)
        {
            glEnableVertexAttribArray(i);
            
            glVertexAttribPointer(i, m_layouts[i].count, m_layouts[i].data_type, GL_FALSE, m_layout_total, (void*)bytes_stride);
            bytes_stride += m_layouts[i].count*m_layouts[i].type_size;

        }

        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glBindVertexArray(0);

        
    }

    ~array_mesh()
    {

    }


};


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

    unsigned int density_shader = CreateShader("MarchingCubes/examples/dens3.vs", "MarchingCubes/examples/dens3.gs","MarchingCubes/examples/dens3.fs");
    unsigned int density_viewer = CreateShader("MarchingCubes/shaders/density_view.vs", "MarchingCubes/shaders/density_view.fs"); 
    
    unsigned int density_to_geometry = CreateShaderWithIncludes("MarchingCubes/examples/custom_mesh1.vs", "MarchingCubes/examples/custom_mesh1.fs", "MarchingCubes/examples/custom_mesh1.gs");

    
    
    //single uv example
    float dummy_uv[] = {0.0, 0.0, 0.0, 
                        1.0/2.0, 1.0/2.0, 0.0,
                        1.0/2.0, 0.0, 0.0,
                        0.0, 1.0/2.0, 0.0,

                        0.0, 0.0, 1.0/2.0, 
                        1.0/2.0, 1.0/2.0, 1.0/2.0,
                        1.0/2.0, 0.0, 1.0/2.0,
                        0.0, 1.0/2.0, 1.0/2.0
                        };
    
    vertex_layout layout[] = { {GL_FLOAT, sizeof(float), 3} };
    array_mesh dummy_mesh = array_mesh(dummy_uv, sizeof(dummy_uv)/sizeof(float), layout, 1, 3*sizeof(float));
    int uv_count = sizeof(dummy_uv)/(sizeof(float) * 3);

    FirstPersonCamera mainCamera;
    mainCamera.super.MakeActiveCamera(&mainCamera, window);
    cameraBaseClass activeCamera = mainCamera.super;

    vaeb screen_quad = screenQuad();

    int dimx = 4;
    int dimy = 4;
    int dimz = 4;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, dimx, dimy,dimz, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_3D, 0);  

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int current_texture = 0;

    int input_frame_delay = 0;
    bool input_allowed = true;

    double delta_time = 0.0;

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        delta_time = glfwGetTime()-delta_time;

        activeCamera.proccessInput(&mainCamera, window);
        
        glBindVertexArray(screen_quad.vertexarray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad.elementbuffer);

        glUseProgram(density_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, dimx, dimy);
        setInt(density_shader, "depth", 0);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, dimz);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        if(input_allowed == false)
        {
            double delta_frame_time = input_frame_delay * delta_time;
            if(delta_frame_time > 40)
            {
                input_frame_delay = 0;
                input_allowed = true;
            }else
            {
                input_frame_delay ++;
            }   
            

        }

        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && input_allowed)
        {
               
            if(current_texture + 1 < dimz)
            {
                current_texture += 1;
                printf("%d",current_texture);
            }

            input_allowed = false;
        }

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && input_allowed)
        {
            
            if(current_texture -1 >= 0)
            {
                current_texture -=1;
                printf("%d",current_texture);
            }

            input_allowed = false;
        }

        //density viewer program
        //glUseProgram(density_viewer);
 
        //glBindTexture(GL_TEXTURE_3D, texture);
        //glActiveTexture(GL_TEXTURE0);

        //setInt(density_viewer, "density", 0);
        //setInt(density_viewer, "level", current_texture);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glViewport(0, 0, 1024, 768);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1024, 768);
        //geometry shader

        glUseProgram(density_to_geometry);
        setTexture3D(density_to_geometry, "density", 0, texture);
        setVec3(density_to_geometry, "chunk_lower", glm::vec3(0.0, 0.0, 0.0));
        setVec3(density_to_geometry, "chunk_upper", glm::vec3(1, 1, -1));
        
        setMat4(density_to_geometry, "view", mainCamera.matricies.view);
        setMat4(density_to_geometry, "projection", mainCamera.matricies.projection);
        

        glBindVertexArray(dummy_mesh.vertex_array);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDrawArrays(GL_POINTS, 0, uv_count); 
        


        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);

}