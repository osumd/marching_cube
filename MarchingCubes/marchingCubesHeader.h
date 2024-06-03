#ifndef MARCHING_CUBES_HEAD
#define MARCHING_CUBES_HEAD

#include "cameraFrustrum.h"
#include "PerspectiveFPSCamera.h"
#include "globalObjectsBuffer.h"
#include <algorithm>

#pragma region VISIBLE_CUBES_ALGORITHMS

void BoundingFrustrumVisibleCubes(glm::vec3& cameraPos, glm::vec3& cameraDir, cf_bvol& frustrumVolume,  cubeBuffer& visible_cubes)
{
    
    //glm::ivec3 origin = glm::ivec3(floor(frustrumVolume.TopLeftFront.x), floor(frustrumVolume.BottomRightBack.y), floor(frustrumVolume.TopLeftFront.z));
    glm::vec3 zaxis = glm::vec3(frustrumVolume.TopLeftFront.x, frustrumVolume.BottomRightBack.y, frustrumVolume.BottomRightBack.z)-glm::vec3(frustrumVolume.TopLeftFront.x, frustrumVolume.BottomRightBack.y, frustrumVolume.TopLeftFront.z);
    glm::vec3 xaxis = glm::vec3(frustrumVolume.BottomRightBack.x,frustrumVolume.BottomRightBack.y, frustrumVolume.TopLeftFront.z)-glm::vec3(frustrumVolume.TopLeftFront.x, frustrumVolume.BottomRightBack.y, frustrumVolume.TopLeftFront.z);
    glm::vec3 yaxis = glm::vec3(frustrumVolume.TopLeftFront.x, frustrumVolume.TopLeftFront.y, frustrumVolume.TopLeftFront.z)-glm::vec3(frustrumVolume.TopLeftFront.x, frustrumVolume.BottomRightBack.y, frustrumVolume.TopLeftFront.z);
    
    glm::vec3 center;
    bvol_center(frustrumVolume, center);

    glm::ivec3 origin = {floor(frustrumVolume.TopLeftFront.x), floor(frustrumVolume.BottomRightBack.y), floor(frustrumVolume.TopLeftFront.z)};

    graphicVectors[0].m_Start = center;
    graphicVectors[0].m_End = cameraDir*static_cast<float>(0.8);
    graphicVectors[0].ReconstructVector();
    

    int z_depth = ceil(glm::length(zaxis));
    int x_depth = ceil(glm::length(xaxis));
    int y_depth = ceil(glm::length(yaxis));

    xaxis = glm::normalize(xaxis);
    yaxis = glm::normalize(yaxis);
    zaxis = glm::normalize(zaxis);

    for(int i = 0; i < x_depth; i++)
    {
        for(int j = 0; j < y_depth; j++)
        {
            for(int k = 0; k < z_depth; k++)
            {

                if(i*j*k < MAX_VISIBLE_CUBES)
                {
                    visible_cubes.cubeIndex[i*j*k] = center + glm::vec3(i,j,k);
                    
                    visible_cubes.n_visible_cubes++;
                }else
                {
                    return;
                }
            
            }

        }

    }


    //1x1x1
    //{cubeX, cubeY, cubeZ}+{1,1,1} //top and cubeX, cubeY, cubeZ, is bottom left
    //we need to store all of the currently visible blocks in combination with the vertex buffers.

    //we need only consider blocks within the tangent cube distance in the cameraDir direction
    //the tangent cube is composed of three distances (far-near) in the cameraDir
    //(+- tan(fov/2)) from center of frustrum cube

    //we add our current cube to the and incrementally all that floor into the far and near zones
}

void FrustrumVisibleWalk(glm::vec3& cameraPos, glm::vec3& cameraDir, cf_8vec& frustrum, cubeBuffer& visible_cubes)
{
    
    int zDistanceSpan = 3;

    glm::vec3 scanCursor0 = frustrum.near_bottom_left;
    glm::vec3 scanCursor1 = frustrum.near_top_right;

    //rectangular scan axis
    glm::vec3 cursorAxis0 = glm::normalize((frustrum.far_bottom_left-frustrum.near_bottom_left));
    glm::vec3 cursorAxis1 = glm::normalize((frustrum.far_top_right-frustrum.near_top_right));



    int fieldDepth = frustrum.near_bottom_left.z-frustrum.far_bottom_left.z; 
   
    
    for(int k = 0; k < fieldDepth; k++)
    {
        scanCursor0 += cursorAxis0; //cursor scans along the bottom left framing sight line
        int layerWidth = scanCursor1.x - scanCursor0.x;
        int layerHeight = scanCursor1.y - scanCursor0.y;

        for(int j = 0; j < layerHeight; j++)
        {
            for(int k = 0; k < layerWidth; k++)
            {
                glm::vec3 kAxis = glm::normalize(glm::vec3(scanCursor1.x, scanCursor0.y, scanCursor0.z) - scanCursor0);
                glm::vec3 jAxis = glm::normalize(scanCursor1 - glm::vec3(scanCursor1.x, scanCursor0.y, scanCursor0.z));
                glm::vec3 visibleCursor = scanCursor0 + (kAxis * (float)k ) + (jAxis * (float)j);
                glm::ivec3 visibleCubeLowerLeft = glm::floor(visibleCursor);
            }
        }
        
    }   



}

#pragma endregion


#pragma region WORLD_BLOCKS

struct shad_world_block
{
    glm::vec4 wcoord;
};

cubeBuffer  global_cube_buffer;

void FindWorldBlocks(FirstPersonCamera& perspective)
{
    cf_bvol boundingFrustrum = {glm::vec3(0,0,0),glm::vec3(0,0,0)};
    cf_8vec frustrum = CreateFrustrum(perspective.cameraPos, perspective.cameraFront, perspective.cameraUp, perspective.fov, perspective.fov, 0.1, 0.4);
    global_cube_buffer.n_visible_cubes = 0;
    FrustrumCube(frustrum,  boundingFrustrum); //makes block

    BoundingFrustrumVisibleCubes(perspective.cameraPos, perspective.cameraFront, boundingFrustrum, global_cube_buffer);

    printf("Found around: %d world blocks\n", global_cube_buffer.n_visible_cubes);
}
bool VolumeDistanceCompare(glm::ivec3 a, glm::ivec3 b, FirstPersonCamera& perspective) {
    
    float dA = glm::length(glm::vec3(a.x,a.y,a.z)-perspective.cameraPos);
    float dB = glm::length(glm::vec3(b.x,b.y,b.z)-perspective.cameraPos);
    return dA < dB;

}
void SortWorldBlocks(FirstPersonCamera& perspective)
{

    std::sort(global_cube_buffer.cubeIndex, global_cube_buffer.cubeIndex+global_cube_buffer.n_visible_cubes,[&perspective](glm::ivec3 a, glm::ivec3 b)
    {
        return VolumeDistanceCompare(a,b,perspective);
    });
    

}


#pragma endregion


#endif