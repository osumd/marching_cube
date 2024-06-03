#ifndef F_GEOM_H
#define F_GEOM_H

#include <vector>

#include <glmmath.h>

static const float QuadUvs[] =
{
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
};

static glm::vec3 NHolder;


void fPoint(const glm::vec3& A, const int quadsIndex, std::vector<float>& memoryData)
{
    memoryData.push_back(A.x);
    memoryData.push_back(A.y);
    memoryData.push_back(A.z);

    memoryData.push_back(QuadUvs[quadsIndex*2]);
    memoryData.push_back(QuadUvs[(quadsIndex*2)+1]);

    memoryData.push_back(NHolder.x);
    memoryData.push_back(NHolder.y);
    memoryData.push_back(NHolder.z);
}

void fPoint(const glm::vec3& A, float uvx, float uvy, float* memoryData, int& i)
{
    memoryData[i++] = A.x;
    memoryData[i++] = A.y;
    memoryData[i++] = A.z;

    memoryData[i++] = uvx;
    memoryData[i++] = uvy;

    memoryData[i++] = NHolder.x;
    memoryData[i++] = NHolder.y;
    memoryData[i++] = NHolder.z;
}
void fTriQuad(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, std::vector<float>& memoryData)
{
    NHolder = glm::cross(B-A, D-A);

    fPoint(A, 0, memoryData);
    fPoint(B, 1, memoryData);
    fPoint(C, 2, memoryData);

    fPoint(A, 0, memoryData);
    fPoint(C, 2, memoryData);
    fPoint(D, 3, memoryData);
}
void fTriQuad(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, float* memoryData, int& i)
{
    NHolder = glm::cross((B-A), (D-A));

    fPoint(A, 0,0, memoryData, i);
    fPoint(B, 1,0, memoryData, i);
    fPoint(C, 1,1, memoryData, i);

    fPoint(A, 0,0, memoryData, i);
    fPoint(C, 1,1, memoryData, i);
    fPoint(D, 0,1, memoryData, i);

}
void fTriQuadE(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, float* memoryData, int& i)
{

    NHolder = glm::cross((B-A), (D-A));
    fPoint(A, 0,0, memoryData, i);
    fPoint(B, 1,0, memoryData, i);
    fPoint(C, 1,1, memoryData, i);
    fPoint(D, 0,1, memoryData, i);
}

void fTriQuad(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, float* memoryData, int& i, unsigned int* indexBuffer, int& j, int& indices)
{
    NHolder = glm::cross((B-A), (D-A));

    fPoint(A, 0,0, memoryData, i);
    fPoint(B, 1,0, memoryData, i);
    fPoint(C, 1,1, memoryData, i);
    fPoint(D, 0,1, memoryData, i);

    indexBuffer[indices++] = j;
    indexBuffer[indices++] = j+1;
    indexBuffer[indices++] = j+2;

    indexBuffer[indices++] = j;
    indexBuffer[indices++] = j+2;
    indexBuffer[indices++] = j+3;

    j+=4;
}

// void fTriQuadFile(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, float* memoryData, int& i, unsigned int* indexBuffer, int& j, int& indices)
// {
//     s
// }

void fTriQuad(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, float uvs[8], float* memoryData, int& i, unsigned int* indexBuffer, int& j, int& indices)
{
    NHolder = glm::cross((B-A), (D-A));
    
    fPoint(A, uvs[0],uvs[1], memoryData, i);
    fPoint(B, uvs[2],uvs[3], memoryData, i);
    fPoint(C, uvs[4],uvs[5], memoryData, i);
    fPoint(D, uvs[6],uvs[7], memoryData, i);

    indexBuffer[indices++] = j;
    indexBuffer[indices++] = j+1;
    indexBuffer[indices++] = j+2;

    indexBuffer[indices++] = j;
    indexBuffer[indices++] = j+2;
    indexBuffer[indices++] = j+3;

    j+=4;
}




void fCircQuad(const glm::vec3 Start, const glm::vec3 End, float radius0,  float radius1, const glm::vec3 Normal, const glm::vec3 Binormal, float j, float dr, float* memoryData, int& i, unsigned int* indexBuffer, int& b, int& indices)
{
    glm::vec3 c1,c2,c3,c4;
    glm::vec3 LowerRotation, UpperRotation;
    
    rotateFrame(LowerRotation, Normal, Binormal, j*dr);
    rotateFrame(UpperRotation, Normal, Binormal, (j+1)*dr);

    LowerRotation *= radius0;
    UpperRotation *= radius0;
    c1 = Start;
    c1 += LowerRotation;
    c4 = Start;
    c4 += UpperRotation;

    LowerRotation = glm::normalize(LowerRotation);

    UpperRotation = glm::normalize(UpperRotation);
    LowerRotation *= radius1;
    UpperRotation *= radius1;
    c2 = End;
    c2 += LowerRotation;
    c3 = End;
    c3 += UpperRotation;
    

    fTriQuad(c1, c2, c3, c4, memoryData, i, indexBuffer, b, indices);
}





void fTri(const glm::vec3 A, const glm::vec3 B, const glm::vec3 C, std::vector<float>& memoryData)
{
    NHolder = glm::cross((B-A), (C-A));

    fPoint(A, 0, memoryData);
    fPoint(B, 1, memoryData);
    fPoint(B, 2, memoryData);
}




struct SubPlane
{
    float* data;
    int byteSize;
    int verts;
};

SubPlane EqualSubdividedPlane(int subDivisions)
{
    float widthPlane = 2.0;
    float heightPlane = 2.0;
    float dx = widthPlane/subDivisions;
    float dy = heightPlane/subDivisions;

    glm::vec3 Origin = {-1,-1,0};

    float* data = (float*)malloc(32*6*subDivisions*subDivisions);
    int k = 0;
    for(int i = 0; i < subDivisions; i++)
    {
        for(int j = 0; j < subDivisions; j++)
        {
            fTriQuad(Origin+glm::vec3(dx*j, dy*i, 0), Origin+ glm::vec3(dx*(j+1), dy*i, 0), Origin + glm::vec3(dx*(j+1), dy*(i+1), 0), Origin+glm::vec3(dx*j, dy*(i+1),0), data, k);
        }
    }

    //fTriQuad({-1, -1, 0}, {1, -1, 0}, {1,1,0}, {-1, 1, 0}, data, i);

    return {data, 32*6*subDivisions*subDivisions, 6*subDivisions*subDivisions};
}

#endif