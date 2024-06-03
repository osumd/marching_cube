#ifndef GRAPHIC_VECTOR_H
#define GRAPHIC_VECTOR_H

#include <OpenGLToolBox.h>
#include "FGeom.h"
#include <cameraBaseClass.h>

struct GraphicVector    
{
    glm::vec3 m_Start;
    glm::vec3 m_End;
    float* memoryData;
    unsigned int* elementData;

    unsigned int vertexBuffer, vertexArray;
    unsigned int elementBuffer;
    unsigned int program;

    int memoryDataBack;
    int elementDataBack;
    int vertices; 
    int indices;

    float arrowHeight = 0.05;
    float arrowThickness = 0.05;

    float shaftLength = 0.05;
    float tipLength = 0.2;
    float tipRadius = 0.00;
    float baseRadius = 0.08;
    float shaftRadius = 0.05;

    float SubdivisionFaces = 8;
    float HorizontalDivisions = 1;

    float borderRadius = 0.04;
    float lipLength = 0.02;

    GraphicVector() : memoryDataBack(0), vertices(0), indices(0), elementDataBack(0), memoryData(nullptr), elementData(nullptr), program(CreateShader("Objects/GraphicVector/GraphicVector.vs", "Objects/GraphicVector/GraphicVector.fs"))
    {

    }

    GraphicVector(glm::vec3 Start, glm::vec3 End) : memoryDataBack(0), vertices(0), indices(0), elementDataBack(0), memoryData(nullptr), elementData(nullptr), program(CreateShader("Objects/GraphicVector/GraphicVector.vs", "Objects/GraphicVector/GraphicVector.fs"))
    { 
        m_Start = Start;
        m_End = End;
        ConstructSquaredAxis();
        SetUpBuffers();
    }


    void InitializeVectors()
    {
        ConstructSquaredAxis();
        SetUpBuffers();
    }
    

    GraphicVector(const GraphicVector& vector) : memoryDataBack(vector.memoryDataBack), vertices(vector.vertices), indices(vector.indices), elementDataBack(vector.elementDataBack), memoryData(vector.memoryData), elementData(vector.elementData), program(vector.program)
    {
        vertexArray = vector.vertexArray;
        vertexBuffer = vector.vertexBuffer;
        elementBuffer = vector.elementBuffer;
    }

    void ConstructSquaredAxis()
    {

        memoryData = (float*)malloc(((48*(SubdivisionFaces))*(HorizontalDivisions+2) ) * sizeof(float));

        glm::vec3 axis = m_End- m_Start;
        glm::vec3 axisnorm = glm::cross(axis, glm::vec3(1,1,1)); 
        glm::vec3 axisnnorm = glm::cross(axisnorm, axis);

        int numberOfFaces = (SubdivisionFaces*(HorizontalDivisions+2));
        
        float axisLength = axis.length();
        float axisdx = axisLength/HorizontalDivisions;
        float dr = (2*3.14159265359)/SubdivisionFaces;

        
        glm::vec3 tip_loc = m_End;
        m_End = m_End - (glm::normalize(axis)*tipLength);
        axis = glm::normalize(axis);
        glm::vec3 p_left = m_Start, p_right = m_End;

        

        glm::vec3 c1,c2,c3,c4, c5;

        for(int i = 0; i < HorizontalDivisions; i++)
        {
            for(float j = 0; j < SubdivisionFaces; j++)
            {
                
                c1 = p_left;
                c2 = {(cos(j*dr)*axisnorm.x) + (sin(j*dr)*axisnnorm.x), (cos(j*dr)*axisnorm.y) + (sin(j*dr)*axisnnorm.y), (cos(j*dr)*axisnorm.z) + (sin(j*dr)*axisnnorm.z)};
                c1 = c1+(glm::normalize(c2)*borderRadius);

                c2 = p_right;
                c3 = glm::vec3((cos(j*dr)*axisnorm.x) + (sin(j*dr)*axisnnorm.x), (cos(j*dr)*axisnorm.y) + (sin(j*dr)*axisnnorm.y), (cos(j*dr)*axisnorm.z) + (sin(j*dr)*axisnnorm.z));
                
                c2 = c2 + glm::normalize(c3) * borderRadius;
                
                c3 = p_right;
                c4 = {(cos((j+1)*dr)*axisnorm.x) + (sin((j+1)*dr)*axisnnorm.x), (cos((j+1)*dr)*axisnorm.y) + (sin((j+1)*dr)*axisnnorm.y), (cos((j+1)*dr)*axisnorm.z) + (sin((j+1)*dr)*axisnnorm.z)};
                c3 = c3+ glm::normalize(c4)* borderRadius;

                c4 = p_left;
                c5 = {(cos((j+1)*dr)*axisnorm.x) + (sin((j+1)*dr)*axisnnorm.x), (cos((j+1)*dr)*axisnorm.y) + (sin((j+1)*dr)*axisnnorm.y), (cos((j+1)*dr)*axisnorm.z) + (sin((j+1)*dr)*axisnnorm.z)};
                c4 = p_left+ glm::normalize(c5)* borderRadius;

                vertices += 6;

                fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);

                if(i == (HorizontalDivisions-1))
                {
                    c1 = m_End;
                    c2 = {(borderRadius*cos(j*dr)*axisnorm.x) + (borderRadius*sin(j*dr)*axisnnorm.x), (borderRadius*cos(j*dr)*axisnorm.y) + (borderRadius*sin(j*dr)*axisnnorm.y), (borderRadius*cos(j*dr)*axisnorm.z) + (borderRadius*sin(j*dr)*axisnnorm.z)};
                    c1 = c1+ glm::normalize(c2)* borderRadius;
                    c3 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c2 = m_End;
                    c2 = c2+ glm::normalize(c3)* baseRadius;
                    c4 = {(baseRadius*cos((j+1)*dr)*axisnorm.x) + (baseRadius*sin((j+1)*dr)*axisnnorm.x), (baseRadius*cos((j+1)*dr)*axisnorm.y) + (baseRadius*sin((j+1)*dr)*axisnnorm.y), (baseRadius*cos((j+1)*dr)*axisnorm.z) + (baseRadius*sin((j+1)*dr)*axisnnorm.z)};
                    c3 = m_End;
                    c3 = c3+ glm::normalize(c4)* baseRadius;
                    c4 = m_End;
                    c5 = {(borderRadius*cos((j+1)*dr)*axisnorm.x) + (borderRadius*sin((j+1)*dr)*axisnnorm.x), (borderRadius*cos((j+1)*dr)*axisnorm.y) + (borderRadius*sin((j+1)*dr)*axisnnorm.y), (borderRadius*cos((j+1)*dr)*axisnorm.z) + (borderRadius*sin((j+1)*dr)*axisnnorm.z)};                  
                    c4 = c4+ glm::normalize(c5)* borderRadius;

                    vertices += 6;

                    fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);

                    c1 = m_End;
                    c2 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c1 = c1+ glm::normalize(c2)* baseRadius;
                    c3 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c2 = tip_loc;
                    c2 = c2+ glm::normalize(c3)* tipRadius;
                    c4 = {(baseRadius*cos((j+1)*dr)*axisnorm.x) + (baseRadius*sin((j+1)*dr)*axisnnorm.x), (baseRadius*cos((j+1)*dr)*axisnorm.y) + (baseRadius*sin((j+1)*dr)*axisnnorm.y), (baseRadius*cos((j+1)*dr)*axisnorm.z) + (baseRadius*sin((j+1)*dr)*axisnnorm.z)};
                    c3 = tip_loc;
                    c3 = c3+ glm::normalize(c4)* tipRadius;
                    c4 = m_End;
                    c5 = {(borderRadius*cos((j+1)*dr)*axisnorm.x) + (borderRadius*sin((j+1)*dr)*axisnnorm.x), (borderRadius*cos((j+1)*dr)*axisnorm.y) + (borderRadius*sin((j+1)*dr)*axisnnorm.y), (borderRadius*cos((j+1)*dr)*axisnorm.z) + (borderRadius*sin((j+1)*dr)*axisnnorm.z)};                  
                    c4 = c4+ glm::normalize(c5)* baseRadius;

                    vertices += 6;

                    fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);
                }

            }
        }
    }

    void ReconstructSquaredAxis()
    {

        glm::vec3 axis = m_End-m_Start;
        glm::vec3 axisnorm = glm::cross(axis, glm::vec3(1,1,1)); 
        glm::vec3 axisnnorm = glm::cross(axisnorm, axis);

        int numberOfFaces = (SubdivisionFaces*(HorizontalDivisions+2));
        
        float axisLength = axis.length();
        float axisdx = axisLength/HorizontalDivisions;
        float dr = (2*3.14159265359)/SubdivisionFaces;

        
        glm::vec3 tip_loc = m_End;
        m_End = m_End - (glm::normalize(axis)*tipLength);
        axis = glm::normalize(axis);
        glm::vec3 p_left = m_Start, p_right = m_End;

        glm::vec3 c1,c2,c3,c4, c5;

        for(int i = 0; i < HorizontalDivisions; i++)
        {
            for(float j = 0; j < SubdivisionFaces; j++)
            {
                
                c1 = p_left;
                c2 = {(cos(j*dr)*axisnorm.x) + (sin(j*dr)*axisnnorm.x), (cos(j*dr)*axisnorm.y) + (sin(j*dr)*axisnnorm.y), (cos(j*dr)*axisnorm.z) + (sin(j*dr)*axisnnorm.z)};
                c1 = c1+(glm::normalize(c2)*borderRadius);

                c2 = p_right;
                c3 = glm::vec3((cos(j*dr)*axisnorm.x) + (sin(j*dr)*axisnnorm.x), (cos(j*dr)*axisnorm.y) + (sin(j*dr)*axisnnorm.y), (cos(j*dr)*axisnorm.z) + (sin(j*dr)*axisnnorm.z));
                
                c2 = c2 + glm::normalize(c3) * borderRadius;
                
                c3 = p_right;
                c4 = {(cos((j+1)*dr)*axisnorm.x) + (sin((j+1)*dr)*axisnnorm.x), (cos((j+1)*dr)*axisnorm.y) + (sin((j+1)*dr)*axisnnorm.y), (cos((j+1)*dr)*axisnorm.z) + (sin((j+1)*dr)*axisnnorm.z)};
                c3 = c3+ glm::normalize(c4)* borderRadius;

                c4 = p_left;
                c5 = {(cos((j+1)*dr)*axisnorm.x) + (sin((j+1)*dr)*axisnnorm.x), (cos((j+1)*dr)*axisnorm.y) + (sin((j+1)*dr)*axisnnorm.y), (cos((j+1)*dr)*axisnorm.z) + (sin((j+1)*dr)*axisnnorm.z)};
                c4 = p_left+ glm::normalize(c5)* borderRadius;

                vertices += 6;

                fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);

                if(i == (HorizontalDivisions-1))
                {
                    c1 = m_End;
                    c2 = {(borderRadius*cos(j*dr)*axisnorm.x) + (borderRadius*sin(j*dr)*axisnnorm.x), (borderRadius*cos(j*dr)*axisnorm.y) + (borderRadius*sin(j*dr)*axisnnorm.y), (borderRadius*cos(j*dr)*axisnorm.z) + (borderRadius*sin(j*dr)*axisnnorm.z)};
                    c1 = c1+ glm::normalize(c2)* borderRadius;
                    c3 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c2 = m_End;
                    c2 = c2+ glm::normalize(c3)* baseRadius;
                    c4 = {(baseRadius*cos((j+1)*dr)*axisnorm.x) + (baseRadius*sin((j+1)*dr)*axisnnorm.x), (baseRadius*cos((j+1)*dr)*axisnorm.y) + (baseRadius*sin((j+1)*dr)*axisnnorm.y), (baseRadius*cos((j+1)*dr)*axisnorm.z) + (baseRadius*sin((j+1)*dr)*axisnnorm.z)};
                    c3 = m_End;
                    c3 = c3+ glm::normalize(c4)* baseRadius;
                    c4 = m_End;
                    c5 = {(borderRadius*cos((j+1)*dr)*axisnorm.x) + (borderRadius*sin((j+1)*dr)*axisnnorm.x), (borderRadius*cos((j+1)*dr)*axisnorm.y) + (borderRadius*sin((j+1)*dr)*axisnnorm.y), (borderRadius*cos((j+1)*dr)*axisnorm.z) + (borderRadius*sin((j+1)*dr)*axisnnorm.z)};                  
                    c4 = c4+ glm::normalize(c5)* borderRadius;

                    vertices += 6;

                    fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);

                    c1 = m_End;
                    c2 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c1 = c1+ glm::normalize(c2)* baseRadius;
                    c3 = {(baseRadius*cos(j*dr)*axisnorm.x) + (baseRadius*sin(j*dr)*axisnnorm.x), (baseRadius*cos(j*dr)*axisnorm.y) + (baseRadius*sin(j*dr)*axisnnorm.y), (baseRadius*cos(j*dr)*axisnorm.z) + (baseRadius*sin(j*dr)*axisnnorm.z)};
                    c2 = tip_loc;
                    c2 = c2+ glm::normalize(c3)* tipRadius;
                    c4 = {(baseRadius*cos((j+1)*dr)*axisnorm.x) + (baseRadius*sin((j+1)*dr)*axisnnorm.x), (baseRadius*cos((j+1)*dr)*axisnorm.y) + (baseRadius*sin((j+1)*dr)*axisnnorm.y), (baseRadius*cos((j+1)*dr)*axisnorm.z) + (baseRadius*sin((j+1)*dr)*axisnnorm.z)};
                    c3 = tip_loc;
                    c3 = c3+ glm::normalize(c4)* tipRadius;
                    c4 = m_End;
                    c5 = {(borderRadius*cos((j+1)*dr)*axisnorm.x) + (borderRadius*sin((j+1)*dr)*axisnnorm.x), (borderRadius*cos((j+1)*dr)*axisnorm.y) + (borderRadius*sin((j+1)*dr)*axisnnorm.y), (borderRadius*cos((j+1)*dr)*axisnorm.z) + (borderRadius*sin((j+1)*dr)*axisnnorm.z)};                  
                    c4 = c4+ glm::normalize(c5)* baseRadius;

                    vertices += 6;

                    fTriQuad(c1, c2, c3, c4, memoryData, memoryDataBack);
                }

            }
        }

    }
    void ConstructSquaredAxisElement()
    {

        memoryData = new float[20*6*20];
        elementData = new unsigned int[20*6*20];

        glm::vec3 axis = glm::normalize(m_End-m_Start);
        glm::vec3 axisnorm = glm::cross(axis, {1,0,0}); 
        glm::vec3 axisnnorm = glm::cross(axisnorm, axis);

        int numberOfFaces = (SubdivisionFaces*(HorizontalDivisions+2));


        float axisLength = axis.length();
        float axisdx = axisLength/HorizontalDivisions;
        float dr = (2*3.14159265359)/SubdivisionFaces;

        glm::vec3 p_left = m_Start, p_right = m_End;
        glm::vec3 m_lip = m_End+ axis* lipLength;


        glm::vec3 tip_loc = m_lip+ axis* tipLength-lipLength;

        glm::vec3 c1,c2,c3,c4, c5;

        for(int i = 0; i < HorizontalDivisions; i++)
        {
            for(float j = 0; j < SubdivisionFaces; j++)
            {
                
                // vertices += 6;
                // fCircQuad(m_Start, m_End, borderRadius, borderRadius, axisnorm, axisnnorm, j, dr, memoryData, memoryDataBack, elementData, elementDataBack, indices);
                vertices += 6;
                fCircQuad(m_Start, m_End, borderRadius, borderRadius, axisnorm, axisnnorm, j, dr, memoryData, memoryDataBack, elementData, elementDataBack, indices);
                
                if(i == 0)
                {
                    // vertices += 6;
                    // fCircQuad(m_End, m_End, borderRadius, baseRadius, axisnorm, axisnnorm, j, dr, memoryData, memoryDataBack, elementData, elementDataBack, indices);
                    vertices += 6;
                    fCircQuad(m_End, m_End, borderRadius, baseRadius, axisnorm, axisnnorm, j, dr, memoryData, memoryDataBack, elementData, elementDataBack, indices);
                    

                }

                
            }
        }
    }

    void ConstructSquaredAxisElementAppended()
    {
        float arrowHeight = 0.05;
        float arrowThickness = 0.05;

        float shaftLength = 0.05;
        float tipLength = 0.5;
        float tipRadius = 0.00;
        float baseRadius = 0.4;
        float shaftRadius = 0.05;

        float SubdivisionFaces = 10;
        float HorizontalDivisions = 1;

        memoryData = new float[20*6*20];
        elementData = new unsigned int[20*6*20];

        float borderRadius = 0.2;

        glm::vec3 axis = glm::normalize(m_End-m_Start);
        glm::vec3 axisnorm = glm::cross(axis, {1,0,0}); 
        glm::vec3 axisnnorm = glm::cross(axisnorm, axis);

        int numberOfFaces = (SubdivisionFaces*(HorizontalDivisions+2));

        float axisLength = axis.length();
        glm::vec3 axisdx = axis * (axisLength/HorizontalDivisions);

        float dr = (2*3.14159265359)/SubdivisionFaces;

        glm::vec3 tip_loc = m_End+ axis* tipLength;

        glm::vec3 c1,c2,c3,c4,c5;

        int CurrentIndice = 0;

        //Use the trajectory axis rotation by margin zero to start sequence.
        glm::vec3 Rotator, Origin;
        Origin = m_Start;
        

        int OriginIndice = -1;
        
        for(int i = 0; i <= HorizontalDivisions+1; i++)
        {
            if(i < HorizontalDivisions+1)
            {
                Origin += axisdx;
            }
            

            if(i == 0 || i == HorizontalDivisions+1)
            {
                fPoint(Origin, 0,0, memoryData, memoryDataBack);
                OriginIndice = CurrentIndice;
            }

            for(int j = 0; j < SubdivisionFaces; j++)
            {
                rotateFrame(Rotator, axisnorm, axisnnorm, (j+(j>0))*dr);
                Rotator *= borderRadius;

                if(i == 0 || i == HorizontalDivisions+1)
                {
                    
                    c1 = Origin;
                    c1 += Rotator;
                    
                    
                    fPoint(c1, 1,0, memoryData, memoryDataBack); //origin already in

                    if(j == 0)
                    {
                        c2 = Origin;
                        rotateFrame(Rotator, axisnorm, axisnnorm, (j+1)*dr);
                        Rotator *= borderRadius;
                        c2 += Rotator;

                        fPoint(c2, 1,1, memoryData, memoryDataBack);
                        elementData[indices++] = CurrentIndice++;
                        elementData[indices++] = CurrentIndice++;
                        elementData[indices++] = CurrentIndice++;


                    }else{
                        
                        elementData[indices++] = OriginIndice;
                        elementData[indices++] = CurrentIndice-1;
                        elementData[indices++] = CurrentIndice++;   
                    }
                }else
                {   

                    c1 = Origin;
                    c1 += Rotator;
                    c2 = Origin;

                    fPoint(c1, 1,0, memoryData, memoryDataBack);

                    if(j == 0)
                    {
                        rotateFrame(Rotator, axisnorm, axisnnorm, (j+1)*dr);
                        Rotator *= borderRadius;
                        c2 += Rotator;
                        fPoint(c2, 1,1, memoryData, memoryDataBack);



                        elementData[indices++] = (CurrentIndice)-(SubdivisionFaces+1);
                        elementData[indices++] = CurrentIndice++;
                        elementData[indices++] = CurrentIndice;

                        elementData[indices++] = (CurrentIndice-1)-(SubdivisionFaces+1);
                        elementData[indices++] = CurrentIndice;
                        elementData[indices++] = (CurrentIndice)-(SubdivisionFaces+1);
                        CurrentIndice++;
                    }else{

                        elementData[indices++] = (CurrentIndice-1)-(SubdivisionFaces+1);
                        elementData[indices++] = CurrentIndice-1;
                        elementData[indices++] = CurrentIndice;

                        
                        elementData[indices++] = (CurrentIndice-1)-(SubdivisionFaces+1);
                        elementData[indices++] = CurrentIndice;

                        elementData[indices++] = (CurrentIndice)-(SubdivisionFaces+1);
                        CurrentIndice++;
                    }

                }

            }
        }
    }

    void Draw(cameraBaseClass& camera)
    {
        if(memoryData == nullptr)
        {
            return;
        }
        glBindVertexArray(vertexArray);

        setMat4(program, "projection", camera.matricies->projection);
        setMat4(program, "view", camera.matricies->view);
        setFloat(program, "time", glfwGetTime());
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, vertices);
    }

    void DrawElements(cameraBaseClass& camera)
    {
        glBindVertexArray(vertexArray);
        //setMat4(program, "projection", camera.projection);
        //setMat4(program, "view", camera.view);
        setFloat(program, "time", glfwGetTime());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glUseProgram(program);
        glDrawElements(GL_TRIANGLES, indices+1, GL_UNSIGNED_INT, 0);
    }

    void SetUpBuffers()
    {
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, memoryDataBack*sizeof(float), memoryData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (GLvoid*)(sizeof(float) * 5));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void SetUpBuffersElement()
    {
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        unsigned int vertexBuffer;
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

    void ReconstructVector()
    {
        memoryDataBack = 0; vertices = 0;

        if(memoryData == nullptr)
        {
            InitializeVectors();
        }else
        {
            ReconstructSquaredAxis();
        }
        

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (memoryDataBack)*sizeof(float), memoryData);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (indices)*sizeof(unsigned int), elementData);
    }

    ~GraphicVector()
    {
        if(memoryData != nullptr)
        {
            delete[] memoryData;
        }

        if(elementData != nullptr)
        {
            delete[] elementData;
        }
    }
};





#endif