#ifndef OPEN_GL_TOOL_BOX
#define OPEN_GL_TOOL_BOX


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <WindowStats.h>

//PNG MODE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "OpenGLObjectTools.h"
#include <glmmath/glmmath.h>
#include <cameraBaseClass.h>


#include "OpenGLObjectTools.h"
#include "currentDirectory.h"

#include <string>

#pragma region PROGRAM_CREATION
unsigned int CreateShader(std::string vsS, std::string fsS)
{

    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    if(directoryFetchError != 0) return -1;
    char* fs = getCurrentDirectoryFile(fsS.c_str(), 1, &directoryFetchError);
    if(directoryFetchError != 0) return -1;
    int error = 0;
    
    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int nglFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    char* vertexSource = ParseShader(vs);
    char* fragmentSource = ParseShader(fs);

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
    glShaderSource(nglFragmentShader, 1, (const char**)&fragmentSource, NULL);
    glCompileShader(nglVertexShader);
    glCompileShader(nglFragmentShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("%s\n", VertexLog);
        free(VertexLog);
    }
    glGetShaderiv(nglFragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglFragmentShader, 0, &LogLength, 0);
        char* FragmentLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglFragmentShader, LogLength, NULL, FragmentLog);
        printf("%s\n", FragmentLog);
        free(FragmentLog);
    }
    glAttachShader(nglProgram, nglVertexShader);
    glAttachShader(nglProgram, nglFragmentShader);
    glLinkProgram(nglProgram);

    
    glDeleteShader(nglVertexShader);
    glDeleteShader(nglFragmentShader);
    GLint  linkStatus = 0;
    glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;
}

unsigned int CreateShader(std::string vsS, std::string gsS, std::string fsS)
{

    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    if(directoryFetchError != 0) return -1;
    char* fs = getCurrentDirectoryFile(fsS.c_str(), 1, &directoryFetchError);
    if(directoryFetchError != 0) return -1;
    char* gs = getCurrentDirectoryFile(gsS.c_str(), 1, &directoryFetchError);
    if(directoryFetchError != 0) return -1;


    int error = 0;
    
    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int nglFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int nglGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    char* vertexSource = ParseShader(vs);
    char* fragmentSource = ParseShader(fs);
    char* geometrySource = ParseShader(gs);

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
    glShaderSource(nglFragmentShader, 1, (const char**)&fragmentSource, NULL);
    glShaderSource(nglGeometryShader, 1, (const char**)&geometrySource, NULL);
    
    glCompileShader(nglVertexShader);
    glCompileShader(nglFragmentShader);
    glCompileShader(nglGeometryShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("%s\n", VertexLog);
        free(VertexLog);
    }
    glGetShaderiv(nglFragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglFragmentShader, 0, &LogLength, 0);
        char* FragmentLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglFragmentShader, LogLength, NULL, FragmentLog);
        printf("%s\n", FragmentLog);
        free(FragmentLog);
    }
    glGetShaderiv(nglGeometryShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglGeometryShader, 0, &LogLength, 0);
        char* GeometryLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglGeometryShader, LogLength, NULL, GeometryLog);
        printf("%s\n", GeometryLog);
        free(GeometryLog);
    }
    glAttachShader(nglProgram, nglVertexShader);
    glAttachShader(nglProgram, nglGeometryShader);
    glAttachShader(nglProgram, nglFragmentShader);
    glLinkProgram(nglProgram);

    
    glDeleteShader(nglVertexShader);
    glDeleteShader(nglFragmentShader);
    GLint  linkStatus = 0;
    glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;
}

unsigned int CreateShaderWithIncludes(std::string vsS, std::string fsS, std::string gsS)
{


    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    char* rootPath = stripLast(vs,'/',0);

    if(directoryFetchError != 0) return -1;
    std::string vsSource = ParseShaderWithIncludes(vs,rootPath);

    directoryFetchError = 0;
    char* fs = getCurrentDirectoryFile(fsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string fsSource = ParseShaderWithIncludes(fs,rootPath);

    directoryFetchError = 0;
    char* gs = getCurrentDirectoryFile(gsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string gsSource = ParseShaderWithIncludes(gs,rootPath);

    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int nglFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int nglGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vertexSource = vsSource.c_str();
    const char* fragmentSource = fsSource.c_str();
    const char* geometrySource = gsSource.c_str();

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
    glShaderSource(nglFragmentShader, 1, (const char**)&fragmentSource, NULL);
    glShaderSource(nglGeometryShader, 1, (const char**)&geometrySource, NULL);

    glCompileShader(nglVertexShader);
    glCompileShader(nglFragmentShader);
    glCompileShader(nglGeometryShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("vertex_log: %s\n", VertexLog);
        free(VertexLog);
    }
    glGetShaderiv(nglFragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglFragmentShader, 0, &LogLength, 0);
        char* FragmentLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglFragmentShader, LogLength, NULL, FragmentLog);
        printf("fragment_log: %s\n", FragmentLog);
        free(FragmentLog);
    }

    glGetShaderiv(nglGeometryShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglGeometryShader, 0, &LogLength, 0);
        char* GeometryLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglGeometryShader, LogLength, NULL, GeometryLog);
        printf("geometry_log: %s\n", GeometryLog);
        free(GeometryLog);
    }

    glAttachShader(nglProgram, nglVertexShader);
    glAttachShader(nglProgram, nglFragmentShader);
    glAttachShader(nglProgram, nglGeometryShader);

    glLinkProgram(nglProgram);

    glDeleteShader(nglVertexShader);
    glDeleteShader(nglFragmentShader);
    glDeleteShader(nglGeometryShader);
    GLint  linkStatus = 0;
    glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;





}

unsigned int CreateShaderWithIncludes(std::string vsS, std::string gsS)
{


    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    char* rootPath = stripLast(vs,'/',0);

    if(directoryFetchError != 0) return -1;
    std::string vsSource = ParseShaderWithIncludes(vs,rootPath);

    directoryFetchError = 0;
    char* gs = getCurrentDirectoryFile(gsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string gsSource = ParseShaderWithIncludes(gs,rootPath);

    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int nglGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vertexSource = vsSource.c_str();
    const char* geometrySource = gsSource.c_str();

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
 
    glShaderSource(nglGeometryShader, 1, (const char**)&geometrySource, NULL);

    glCompileShader(nglVertexShader);
 
    glCompileShader(nglGeometryShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("%s\n", VertexLog);
        free(VertexLog);
    }
 
    glGetShaderiv(nglGeometryShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglGeometryShader, 0, &LogLength, 0);
        char* GeometryLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglGeometryShader, LogLength, NULL, GeometryLog);
        printf("%s\n", GeometryLog);
        free(GeometryLog);
    }

    glAttachShader(nglProgram, nglVertexShader);
 
    glAttachShader(nglProgram, nglGeometryShader);

    glLinkProgram(nglProgram);

    glDeleteShader(nglVertexShader);
 
    glDeleteShader(nglGeometryShader);
    GLint  linkStatus = 0;
    glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;





}

unsigned int PrelinkShaderWithIncludes(std::string vsS, std::string fsS, std::string gsS)
{


    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    char* rootPath = stripLast(vs,'/',0);

    if(directoryFetchError != 0) return -1;
    std::string vsSource = ParseShaderWithIncludes(vs,rootPath);

    directoryFetchError = 0;
    char* fs = getCurrentDirectoryFile(fsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string fsSource = ParseShaderWithIncludes(fs,rootPath);

    directoryFetchError = 0;
    char* gs = getCurrentDirectoryFile(gsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string gsSource = ParseShaderWithIncludes(gs,rootPath);

    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int nglFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int nglGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vertexSource = vsSource.c_str();
    const char* fragmentSource = fsSource.c_str();
    const char* geometrySource = gsSource.c_str();

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
    glShaderSource(nglFragmentShader, 1, (const char**)&fragmentSource, NULL);
    glShaderSource(nglGeometryShader, 1, (const char**)&geometrySource, NULL);

    glCompileShader(nglVertexShader);
    glCompileShader(nglFragmentShader);
    glCompileShader(nglGeometryShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("%s\n", VertexLog);
        free(VertexLog);
    }
    glGetShaderiv(nglFragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglFragmentShader, 0, &LogLength, 0);
        char* FragmentLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglFragmentShader, LogLength, NULL, FragmentLog);
        printf("%s\n", FragmentLog);
        free(FragmentLog);
    }

    glGetShaderiv(nglGeometryShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglGeometryShader, 0, &LogLength, 0);
        char* GeometryLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglGeometryShader, LogLength, NULL, GeometryLog);
        printf("%s\n", GeometryLog);
        free(GeometryLog);
    }

    glAttachShader(nglProgram, nglVertexShader);
    glAttachShader(nglProgram, nglFragmentShader);
    glAttachShader(nglProgram, nglGeometryShader);

    //glLinkProgram(nglProgram);

    glDeleteShader(nglVertexShader);
    glDeleteShader(nglFragmentShader);
    glDeleteShader(nglGeometryShader);
    GLint  linkStatus = 0;

    // glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    // if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;





}

unsigned int PrelinkShaderWithIncludes(std::string vsS, std::string gsS)
{


    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    char* rootPath = stripLast(vs,'/',0);

    if(directoryFetchError != 0) return -1;
    std::string vsSource = ParseShaderWithIncludes(vs,rootPath);

    directoryFetchError = 0;
    char* gs = getCurrentDirectoryFile(gsS.c_str(), 1, &directoryFetchError);

    if(directoryFetchError != 0) return -1;
    std::string gsSource = ParseShaderWithIncludes(gs,rootPath);

    unsigned int nglProgram = glCreateProgram();
    unsigned int nglVertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int nglGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vertexSource = vsSource.c_str();
    const char* geometrySource = gsSource.c_str();

    glShaderSource(nglVertexShader, 1, (const char**)&vertexSource, NULL);
 
    glShaderSource(nglGeometryShader, 1, (const char**)&geometrySource, NULL);

    glCompileShader(nglVertexShader);
 
    glCompileShader(nglGeometryShader);

    GLint isCompiled = 0;
    GLsizei LogLength = 0;
    glGetShaderiv(nglVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglVertexShader, 0, &LogLength, 0);
        char* VertexLog = (char*)malloc(LogLength);
        glGetShaderInfoLog(nglVertexShader, LogLength, NULL, VertexLog);
        printf("%s\n", VertexLog);
        free(VertexLog);
    }
 
    glGetShaderiv(nglGeometryShader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(nglGeometryShader, 0, &LogLength, 0);
        char* GeometryLog = (char*)malloc(LogLength);

        glGetShaderInfoLog(nglGeometryShader, LogLength, NULL, GeometryLog);
        printf("%s\n", GeometryLog);
        free(GeometryLog);
    }

    glAttachShader(nglProgram, nglVertexShader);
 
    glAttachShader(nglProgram, nglGeometryShader);

    //glLinkProgram(nglProgram);

    glDeleteShader(nglVertexShader);
 
    glDeleteShader(nglGeometryShader);

    // GLint  linkStatus = 0;
    // glGetProgramiv(nglProgram, GL_LINK_STATUS, &linkStatus);
    // if(linkStatus != GL_TRUE){ throw("fuck"); }

    return nglProgram;



            

}

unsigned int CreateShaderWithIncludes(std::string vsS)
{

    

    int directoryFetchError = 0;
    char* vs = getCurrentDirectoryFile(vsS.c_str(), 1, &directoryFetchError);
    char* rootPath = stripLast(vs,'/',0);
    if(directoryFetchError != 0) return -1;
    std::string vsSource = ParseShaderWithIncludes(vs,rootPath);

    
    std::cout << vsSource << std::endl;

    return 0;

}

#pragma endregion



#pragma region SETTING_UNIFORMS
void setMat4(unsigned int program, const char* name, glm::mat4 mat)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program, name);
    glProgramUniformMatrix4fv(program, modelLoc, 1, GL_FALSE, glm::value_ptr(mat));
}
void setMat4(unsigned int program, const char* name, float* mat)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program,name);
    glProgramUniformMatrix4fv(program,modelLoc, 1, GL_FALSE, mat);
}

void setVec2(unsigned int program, const char* name, glm::vec2 vec)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program,name);
    glProgramUniform2f(program,modelLoc, vec.x, vec.y);
}

void setVec3(unsigned int program, const char* name, float* a)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program,name);
    glProgramUniform3f(program,modelLoc, a[0], a[1], a[2]);

}

void setVec3(unsigned int program, const char* name, glm::vec3 X)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program,name);
    glProgramUniform3f(program,modelLoc, X.x, X.y, X.z);

}

void set2Vec3Array(unsigned int program, const char* name, float* array)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program, name);
    glProgramUniform3fv(program,modelLoc, 2, array);
}

void setVec4(unsigned int program, const char* name, glm::vec4 vec)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program, name);
    glProgramUniform4f(program,modelLoc, vec.x, vec.y, vec.z, vec.w);
}

void setFloat(unsigned int program, const char* name, float value)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program, name);
    glProgramUniform1f(program,modelLoc, value);
}

void setInt(unsigned int program, const char* name, GLint value)
{
    int modelLoc;
    modelLoc = glGetUniformLocation(program, name);
    glProgramUniform1i(program,modelLoc, value);
}

void setUniformBlock(unsigned int program, const char* name, int binding)
{
    GLuint index;
    index = glGetUniformBlockIndex(program,name);
    glUniformBlockBinding(program, index, binding);
}

void setTexture3D(unsigned int program, const char* name, int binding, unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0 + binding);
    glBindTexture(GL_TEXTURE_3D,texture);
    setInt(program, name, binding);
}

#pragma endregion


#pragma region BUFFERS_TEXTURES_GENERATION
unsigned int CreateUniformBuffer(int size, void* data, int index)
{
    unsigned int uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, uniformBuffer);
    return uniformBuffer;
}



unsigned int volTexture(const char* filePath)
{
    float* data = nullptr;
    int width = 0;
    int height = 0;
    int depth = 0;

    parseVol(filePath, &width, &height, &depth, data);


    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;

}

unsigned int GenerateTexture3D(int xdim, int ydim, int zdim, GLenum internal_format, GLenum internal_format2, GLenum type)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_3D, 0, internal_format, xdim, ydim, zdim, 0, internal_format2, type, nullptr);
    

    return textureID;
}

unsigned int GenerateTexture2D(int xdim, int ydim, GLenum internal_format, GLenum internal_format2, GLenum type, void* data)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, xdim, ydim, 0, internal_format2, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}
#pragma endregion

#pragma region GEOMETRY_HELPERS

struct vaeb
{
    unsigned int vertexarray;
    unsigned int elementbuffer;
};

struct vapeb{
    unsigned int program;
    unsigned int vertexarray;
    unsigned int elementbuffer;
    int indices;
    
    void Draw(cameraBaseClass& camera)
    {
        glBindVertexArray(vertexarray);

        setMat4(program, "projection", camera.matricies->projection);
        setMat4(program, "view", camera.matricies->view);
        // setFloat(program, "time", glfwGetTime());
        glUseProgram(program);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glUseProgram(program);

        glDrawElementsInstanced(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0,2);
        // glDrawElements(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0);
    }
    
    void DrawInstanced(cameraBaseClass& camera, int modelBinding)
    {
        glBindVertexArray(vertexarray);

        setMat4(program, "projection", camera.matricies->projection);
        setMat4(program, "view", camera.matricies->view);
        setUniformBlock(program, "instanceTransforms", modelBinding);

        // setFloat(program, "time", glfwGetTime());
        glUseProgram(program);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glUseProgram(program);

        glDrawElementsInstanced(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0,2);
        // glDrawElements(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0);
    }

};

vaeb screenQuad()
{
    unsigned int vbRenderPortal;
    unsigned int vaRenderPortal;
    unsigned int ebRenderPortal;

    float screenVertices[16] = 
    {
       -1.0,-1.0,  0.0,0.0,
        1.0,-1.0,  1.0, 0.0,
        1.0, 1.0,  1.0, 1.0,
       -1.0, 1.0,  0.0, 1.0,

    };

    unsigned int screenIndices[6] =
    {
        0,1,2,
        0,2,3
    };

    glGenVertexArrays(1, &vaRenderPortal);
    glBindVertexArray(vaRenderPortal);

    glGenBuffers(1,&vbRenderPortal);
    glGenBuffers(1,&ebRenderPortal);

    glBindBuffer(GL_ARRAY_BUFFER, vbRenderPortal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebRenderPortal);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return {vaRenderPortal,ebRenderPortal};
}



#pragma endregion


#endif

