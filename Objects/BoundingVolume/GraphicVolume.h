#ifndef GRAPHIC_VOLUME
#define GRAPHIC_VOLUME

#include "OpenGLToolBox.h"
#include "FGeom.h"
#include "cameraBaseClass.h"
#include "extendedVectors.h"
#include "GraphicVector.h"


struct GraphicVolume
{
    cf_bvol m_volume;
    float* memoryData;
    unsigned int* elementData;
    unsigned int vertexBuffer, vertexArray;
    unsigned int elementBuffer;
    unsigned int program;



    int memoryDataBack;
    int elementDataBack;
    int vertices; 
    int indices;
    float CuboidThickness = 0.005;
    int j = 0;

    std::vector<GraphicVector> drawingVectors;

    GraphicVolume(cf_bvol volume) : m_volume(volume), memoryDataBack(0), vertices(0), indices(0), elementDataBack(0), memoryData(nullptr), elementData(nullptr), program(CreateShader("Objects/GraphicVector/GraphicVector.vs", "Objects/GraphicVector//GraphicVector.fs"))
    {
        ConstructBox();
        SetUpBuffers();
    }
    
    void AddRectoid(glm::vec3 a, glm::vec3 b)
    {

        const glm::vec3 axis = b-a;
        glm::vec3 xnorm = glm::cross(glm::vec3(0,1,0),axis);
        if(glm::length(xnorm) == 0)
        {
            xnorm = glm::cross(glm::vec3(1,0,0), axis);
        }
        

        
        xnorm = glm::normalize(xnorm);
        xnorm*=CuboidThickness;
        glm::vec3 ynorm = glm::cross(xnorm,  axis);
        ynorm = glm::normalize(ynorm);
        ynorm*=CuboidThickness;
        

        
        fTriQuad(a-xnorm-ynorm, a+xnorm-ynorm, a+xnorm+ynorm, a-xnorm+ynorm, memoryData,  memoryDataBack, elementData, j, indices);
        fTriQuad(b-xnorm-ynorm, b+xnorm-ynorm, b+xnorm+ynorm, b-xnorm+ynorm, memoryData,  memoryDataBack, elementData, j, indices);

        fTriQuad(a+xnorm-ynorm, b+xnorm-ynorm, b+xnorm+ynorm, a+xnorm+ynorm, memoryData,  memoryDataBack, elementData, j, indices);
        fTriQuad(a-xnorm-ynorm, b-xnorm-ynorm, b-xnorm+ynorm, a-xnorm+ynorm, memoryData,  memoryDataBack, elementData, j, indices);

        fTriQuad(a+ynorm-xnorm, b+ynorm-xnorm, b+ynorm+xnorm, a+ynorm+xnorm, memoryData,  memoryDataBack, elementData, j, indices);
        fTriQuad(a-ynorm-xnorm, b-ynorm-xnorm, b-ynorm+xnorm, a-ynorm+xnorm, memoryData,  memoryDataBack, elementData, j, indices);
    }

    void ConstructBox()
    {
        //bottom left front
        const glm::vec3 b0 = glm::vec3(m_volume.TopLeftFront.x, m_volume.BottomRightBack.y, m_volume.TopLeftFront.z);
        //bottom right front
        const glm::vec3 b1 = glm::vec3(m_volume.BottomRightBack.x, m_volume.BottomRightBack.y, m_volume.TopLeftFront.z);
        //bottom right back
        const glm::vec3 b2 = glm::vec3(m_volume.BottomRightBack.x, m_volume.BottomRightBack.y, m_volume.BottomRightBack.z);
        //bottom left back
        const glm::vec3 b3 = glm::vec3(m_volume.TopLeftFront.x, m_volume.BottomRightBack.y, m_volume.BottomRightBack.z);

       
        //top left front
        const glm::vec3 b4 = glm::vec3(m_volume.TopLeftFront.x, m_volume.TopLeftFront.y, m_volume.TopLeftFront.z);
        //top right front
        const glm::vec3 b5 = glm::vec3(m_volume.BottomRightBack.x, m_volume.TopLeftFront.y, m_volume.TopLeftFront.z);
        //top right back
        const glm::vec3 b6 = glm::vec3(m_volume.BottomRightBack.x, m_volume.TopLeftFront.y, m_volume.BottomRightBack.z);
        //top left back
        const glm::vec3 b7 = glm::vec3(m_volume.TopLeftFront.x, m_volume.TopLeftFront.y, m_volume.BottomRightBack.z);
        
        memoryData = new float[24*(12)*(sizeof(float)*8)];
        elementData = new unsigned int[6*6*(12)]; //6 indices per face

        glm::vec3 bt = b4-b0;
        bt = glm::normalize(bt);
        glm::vec3 ht = b1-b0;
        ht = glm::normalize(ht);
        glm::vec3 vt = b3-b0;
        vt = glm::normalize(vt);

        ht *= CuboidThickness;
        bt *= CuboidThickness;
        vt *= CuboidThickness;

        AddRectoid(b0+ht,b1-ht);
        AddRectoid(b0-vt,b3+vt);

        AddRectoid(b0+bt,b4-bt); //bt

        AddRectoid(b1-vt,b2+vt);
        AddRectoid(b2-ht,b3+ht);

        AddRectoid(b2+bt,b6-bt); //bt

        AddRectoid(b5-vt,b6+vt);
        AddRectoid(b7+ht,b6-ht);
        AddRectoid(b4+ht,b5-ht);


        AddRectoid(b4-vt,b7+vt);
        AddRectoid(b1+bt,b5-bt); //bt
        AddRectoid(b3+bt,b7-bt); //bt

        
    }

    void ReconstructBox()
    {
        memoryDataBack = 0; vertices = 0; indices = 0; elementDataBack = 0; j = 0;

        //bottom left front
        const glm::vec3 b0 = glm::vec3(m_volume.TopLeftFront.x, m_volume.BottomRightBack.y, m_volume.TopLeftFront.z);
        //bottom right front
        const glm::vec3 b1 = glm::vec3(m_volume.BottomRightBack.x, m_volume.BottomRightBack.y, m_volume.TopLeftFront.z);
        //bottom right back
        const glm::vec3 b2 = glm::vec3(m_volume.BottomRightBack.x, m_volume.BottomRightBack.y, m_volume.BottomRightBack.z);
        //bottom left back
        const glm::vec3 b3 = glm::vec3(m_volume.TopLeftFront.x, m_volume.BottomRightBack.y, m_volume.BottomRightBack.z);

       
        //top left front
        const glm::vec3 b4 = glm::vec3(m_volume.TopLeftFront.x, m_volume.TopLeftFront.y, m_volume.TopLeftFront.z);
        //top right front
        const glm::vec3 b5 = glm::vec3(m_volume.BottomRightBack.x, m_volume.TopLeftFront.y, m_volume.TopLeftFront.z);
        //top right back
        const glm::vec3 b6 = glm::vec3(m_volume.BottomRightBack.x, m_volume.TopLeftFront.y, m_volume.BottomRightBack.z);
        //top left back
        const glm::vec3 b7 = glm::vec3(m_volume.TopLeftFront.x, m_volume.TopLeftFront.y, m_volume.BottomRightBack.z);
        
        glm::vec3 bt = b4-b0;
        bt = glm::normalize(bt);
        glm::vec3 ht = b1-b0;
        ht = glm::normalize(ht);
        glm::vec3 vt = b3-b0;
        vt = glm::normalize(vt);

        ht *= CuboidThickness;
        bt *= CuboidThickness;
        vt *= CuboidThickness;


        AddRectoid(b0+ht,b1-ht);
        AddRectoid(b0-vt,b3+vt);

        AddRectoid(b0+bt,b4-bt); //bt

        AddRectoid(b1-vt,b2+vt);
        AddRectoid(b2-ht,b3+ht);

        AddRectoid(b2+bt,b6-bt); //bt

        AddRectoid(b5-vt,b6+vt);
        AddRectoid(b7+ht,b6-ht);
        AddRectoid(b4+ht,b5-ht);


        AddRectoid(b4-vt,b7+vt);
        AddRectoid(b1+bt,b5-bt); //bt
        AddRectoid(b3+bt,b7-bt); //bt

        UpdateBuffers();
        
    }

    void Draw(cameraBaseClass& camera)
    {
        glBindVertexArray(vertexArray);
        setMat4(program, "projection", camera.matricies->projection);
        setMat4(program, "view", camera.matricies->view);
        setFloat(program, "time", glfwGetTime());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glUseProgram(program);
        glDrawElements(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0);

        // for(int i = 0; i < drawingVectors.size(); i++)
        // {
        //     drawingVectors[i].Draw(camera);
        // }
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
    void UpdateBuffers()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (memoryDataBack+1)*sizeof(float), memoryData);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (indices)*sizeof(unsigned int), elementData);
    }

    

    ~GraphicVolume()
    {
        if(memoryData != nullptr)
        {
            delete[] memoryData;
            memoryData = nullptr;
        }

        if(elementData != nullptr)
        {
            delete[] elementData;
            elementData = nullptr;
        }
    }

};




#endif