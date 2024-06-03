#ifndef GRAPHIC_FRUSTRUM_H
#define GRAPHIC_FRUSTRUM_H

#include "OpenGLToolBox.h"
#include "FGeom.h"
#include "cameraBaseClass.h"
#include "extendedVectors.h"
#include "RelativeObject.h"




struct GraphicFrustrum
{
private:
    int memoryDataBack;
    unsigned int* elementData;
    int indices;
    int elementDataBack;
    unsigned int vertexBuffer, vertexArray, elementBuffer;
    unsigned int program;
    void* memoryData;
    int writeRegion;
    int size;
public:
    std::vector<RelativeObject> OnUpdates;

    GraphicFrustrum() : writeRegion(0), size(0),
    memoryDataBack(0), indices(0), elementDataBack(0), memoryData(nullptr), elementData(nullptr), program(CreateShader("Objects/GraphicVector/GraphicVector.vs", "Objects/GraphicVector//GraphicVector.fs"))
    {
        CreateFrustrum({0.0f, 0, 0}, {0,0,-1}, {0,1,0});
    }

    void CreateFrustrum(glm::vec3 camera_pos, glm::vec3 Forward, glm::vec3 up_vector)
    {
        int VertexSize = 8*8;
        int ElementSize = 24;
        elementData = new unsigned int[ElementSize];

        SetUpMemoryMap(VertexSize);

        float hfov = 90;
        float vfov = 90;
        float near_dist = 0.1;
        float far_dist = 0.5;

        glm::vec3 right = glm::cross(Forward, up_vector);
        glm::vec3 up = glm::cross(right,Forward);

        float hhnear = near_dist * tan((hfov*(3.14159265359/180))/2);
        float hhfar = far_dist * tan(((hfov)*(3.14159265359/180))/2);

        glm::vec3 near_center = camera_pos+ normalize(Forward)*near_dist;
        glm::vec3 far_center = camera_pos+ normalize(Forward)*far_dist;
        

        glm::vec3 near_top_left = near_center + up * hhnear - right * hhnear;
        glm::vec3 near_top_right = near_center + up * hhnear + right * hhnear;
        glm::vec3 near_bottom_left = near_center - up * hhnear - right * hhnear;
        glm::vec3 near_bottom_right = near_center - up * hhnear + right * hhnear;

        glm::vec3 far_top_left = far_center + up * hhfar - right * hhfar;
        glm::vec3 far_top_right = far_center + up * hhfar + right * hhfar;
        glm::vec3 far_bottom_left = far_center - up * hhfar - right * hhfar;
        glm::vec3 far_bottom_right = far_center - up * hhfar + right * hhfar;


        fPoint(near_bottom_left, 0,0, (float*)memoryData, memoryDataBack);
        fPoint(near_bottom_right, 1,0, (float*)memoryData, memoryDataBack);
        fPoint(near_top_right, 1,1, (float*)memoryData, memoryDataBack);
        fPoint(near_top_left, 0,1, (float*)memoryData, memoryDataBack);
        
        
        fPoint(far_bottom_left, 0,0, (float*)memoryData, memoryDataBack);
        fPoint(far_bottom_right, 1,0, (float*)memoryData, memoryDataBack);
        fPoint(far_top_right, 1,1, (float*)memoryData, memoryDataBack);
        fPoint(far_top_left, 0,1, (float*)memoryData, memoryDataBack);
        
        elementData[indices++] = 0;
        elementData[indices++] = 1;

        elementData[indices++] = 1;
        elementData[indices++] = 2;

        elementData[indices++] = 2;
        elementData[indices++] = 3;

        elementData[indices++] = 3;
        elementData[indices++] = 0;

        elementData[indices++] = 0;
        elementData[indices++] = 4;

        elementData[indices++] = 1;
        elementData[indices++] = 5;

        elementData[indices++] = 2;
        elementData[indices++] = 6;
        
        elementData[indices++] = 3;
        elementData[indices++] = 7;
        

        elementData[indices++] = 4;
        elementData[indices++] = 5;

        elementData[indices++] = 5;
        elementData[indices++] = 6;

        elementData[indices++] = 6;
        elementData[indices++] = 7;
        
        elementData[indices++] = 7;
        elementData[indices++] = 4;

        SetUpStaticIndexBuffer();
    }

    void UpdateFrustrum(glm::vec3 camera_pos, glm::vec3 Forward, glm::vec3 up_vector, float near_dist, float far_dist, float fov)
    {
        int mback = 0;
        int eback = 0;
        float hfov = fov;
        float vfov = fov;

        glm::vec3 right = glm::cross(Forward, up_vector);
        right = glm::normalize(right);
        glm::vec3 up = glm::cross(right,Forward);
        up = glm::normalize(up);
    
        float hhnear = near_dist * tan((hfov*(3.14159265359/180))/2);
        float hhfar = far_dist * tan(((hfov)*(3.14159265359/180))/2);

        glm::vec3 near_center = camera_pos+normalize(Forward)*near_dist;
        glm::vec3 far_center = camera_pos+normalize(Forward)*far_dist;
        
        glm::vec3 near_top_left = near_center + up * hhnear - right * hhnear;
        glm::vec3 near_top_right = near_center + up * hhnear + right * hhnear;
        glm::vec3 near_bottom_left = near_center - up * hhnear - right * hhnear;
        glm::vec3 near_bottom_right = near_center - up * hhnear + right * hhnear;

        glm::vec3 far_top_left = far_center + up * hhfar - right * hhfar;
        glm::vec3 far_top_right = far_center + up * hhfar + right * hhfar;
        glm::vec3 far_bottom_left = far_center - up * hhfar - right * hhfar;
        glm::vec3 far_bottom_right = far_center - up * hhfar + right * hhfar;
        

        CallOnUpdates(camera_pos, Forward, up_vector, near_dist, far_dist, fov);


        fPoint(near_bottom_left, 0,0, (float*)memoryData, mback);
        fPoint(near_bottom_right, 1,0, (float*)memoryData, mback);
        fPoint(near_top_right, 1,1, (float*)memoryData, mback);
        fPoint(near_top_left, 0,1, (float*)memoryData, mback);
        
        
        fPoint(far_bottom_left, 0,0, (float*)memoryData, mback);
        fPoint(far_bottom_right, 1,0, (float*)memoryData, mback);
        fPoint(far_top_right, 1,1, (float*)memoryData, mback);
        fPoint(far_top_left, 0,1, (float*)memoryData, mback);
        
        elementData[eback++] = 0;
        elementData[eback++] = 1;

        elementData[eback++] = 1;
        elementData[eback++] = 2;

        elementData[eback++] = 2;
        elementData[eback++] = 3;

        elementData[eback++] = 3;
        elementData[eback++] = 0;

        elementData[eback++] = 0;
        elementData[eback++] = 4;

        elementData[eback++] = 1;
        elementData[eback++] = 5;

        elementData[eback++] = 2;
        elementData[eback++] = 6;
        
        elementData[eback++] = 3;
        elementData[eback++] = 7;
        

        elementData[eback++] = 4;
        elementData[eback++] = 5;

        elementData[eback++] = 5;
        elementData[eback++] = 6;

        elementData[eback++] = 6;
        elementData[eback++] = 7;
        
        elementData[eback++] = 7;
        elementData[eback++] = 4;


    }

    void CallOnUpdates(glm::vec3 camera_pos, glm::vec3 Forward, glm::vec3 up_vector, float near_dist, float far_dist, float fov)
    {
        float hfov = fov;
        float vfov = fov;

        float hhnear = near_dist * tan((hfov*(3.14159265359/180))/2);
        float hhfar = far_dist * tan(((hfov)*(3.14159265359/180))/2);

        glm::vec3 near_center = camera_pos+normalize(Forward)*near_dist;
        glm::vec3 far_center = camera_pos+normalize(Forward)*far_dist;
        
        ArgumentPackage p;
        p.Paramters[0] = hhnear*2;
        p.Paramters[1] = hhfar*2;
        p.Verticies[0] = near_center;
        p.Verticies[1] = far_center;
        
        glm::vec3 right = glm::cross(Forward, up_vector);
        glm::vec3 up = glm::cross(right,Forward);

        p.Verticies[2] = right;
        p.Verticies[3] = up;
        p.Verticies[4] = Forward;

        void* s = nullptr;

        for(int i = 0; i < OnUpdates.size(); i++)
        {
            s = OnUpdates[i].self;
            if(OnUpdates[i].callback != nullptr)
            {
                
                OnUpdates[i].callback(s, p);
            }
        }

    }

    void SetUpMemoryMap(int MemoryUsage)
    {
        glGenVertexArrays(1, &vertexArray);
        glGenBuffers(1, &vertexBuffer);
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferStorage(GL_ARRAY_BUFFER, (MemoryUsage)*sizeof(float), 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
        memoryData = glMapBufferRange(GL_ARRAY_BUFFER, 0, (MemoryUsage)*sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);


        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 5));
        glBindVertexArray(0);
    }

    void SetUpStaticIndexBuffer()
    {
        glBindVertexArray(vertexArray);
        glGenBuffers(1, &elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices)*sizeof(unsigned int), elementData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }   

    void SetUpBuffers()
    {
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices)*sizeof(unsigned int), elementData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (memoryDataBack+1)*sizeof(float), memoryData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 5));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw(cameraBaseClass& camera)
    {
        setMat4(program, "projection", camera.matricies->projection);
        setMat4(program, "view", camera.matricies->view);
        setFloat(program, "time", glfwGetTime());

        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glUseProgram(program);
        glDrawElements(GL_LINES, indices+1, GL_UNSIGNED_INT, 0);
    }

    ~GraphicFrustrum()
    {

    }
};


#endif