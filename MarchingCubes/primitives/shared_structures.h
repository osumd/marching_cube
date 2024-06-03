#ifndef MARCHING_SHARED_STRUCTURES_H
#define MARCHING_SHARED_STRUCTURES_H 

struct vec2
{
    float x,y;
    vec2(float _x, float _y) : x(_x),y(_y)
    {

    }


};


struct vec4
{
    float x,y,z,w;
    vec2(float _x, float _y, float _z, float _w) : x(_x),y(_y),z(_z),w(_w)
    {

    }


};


struct LodCB {
  float  VoxelDim = 65;          // # of cell corners
  float  VoxelDimMinusOne = 64;  // # of cells
  vec2 wsVoxelSize = vec2(1.0/64.0, 0);  // could be 1/63, 1/31, 1/15 depending on LOD
  float  wsChunkSize = 4.0;                  // 1.0, 2.0, or 4.0 depending on LOD
  vec2 InvVoxelDim = vec2(1.0/65.0, 0); 
  vec2 InvVoxelDimMinusOne = vec2(1.0/64.0, 0); 
  float  Margin                      = 4;
  float  VoxelDimPlusMargins         = 73;
  float  VoxelDimPlusMarginsMinusOne = 72;
  vec2 InvVoxelDimPlusMargins         = vec2(1.0/73.0, 0);
  vec2 InvVoxelDimPlusMarginsMinusOne = vec2(1.0/72.0, 0);
};


struct MyCB {
  float4 viewportDim = float4(8,6,1.0/8.0,1.0/6.0);
};

// 'ChunkCB' is updated each time we want to build a chunk:
cbuffer ChunkCB {
  float3 wsChunkPos = float3(0,0,0); //wsCoord of lower-left corner
  float  opacity = 1;
}




#endif