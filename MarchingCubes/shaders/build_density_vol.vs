#version 460 core

// input here is some # of instances of quadProxy_geo (a big quad to be rasterized)

layout(location = 0) in vec4 pos; //input each point of a subdivided volume chunk


out VertexData
{
    vec4 wsCoord;
    vec3 chunkCoord;
    int nInstanceID;
} outData;

layout (std140) uniform MyCB {
  vec4 viewportDim;
};

// 'ChunkCB' is updated each time we want to build a chunk:
layout (std140) uniform ChunkCB {
  vec4 wsChunkPos; //wsCoord of lower-left corner of chunck
};

#include "master.h"
#include "LodCB.h"

vec3 rot(vec3 coord, mat4 mat)
{
  return vec3( dot(mat[0].xyz, coord),   // 3x3 transform,
                 dot(mat[0].xyz, coord),   // no translation
                 dot(mat[1].xyz, coord) );
}



void main()
{
    vec2 tex = pos.zw;
    // chunkCoord is in [0..1] range
    vec3 chunkCoord = vec3(tex.xy, gl_InstanceID * InvVoxelDimPlusMargins.x ); 
    
      // HACK #1 - because in DX, when you render a quad with uv's in [0..1],
      //           the upper left corner will be a 0 and the lower right
      //           corner will be at 1 (it should be at 63/64, or whatever).
    //chunkCoord.xyz *= VoxelDim.x*InvVoxelDimMinusOne.x;  
      //chunkCoord.xy *= VoxelDimPlusMargins.x*InvVoxelDimPlusMarginsMinusOne.x;  // HACK #1 

    // extChunkCoord goes outside that range, so we also compute
    // some voxels outside of the chunk
    vec3 extChunkCoord = (chunkCoord*VoxelDimPlusMargins.x - Margin)*InvVoxelDim.x;
                                
    vec3 ws = wsChunkPos.xyz + extChunkCoord*wsChunkSize;

    outData.wsCoord    = vec4(ws,1);
    outData.nInstanceID = gl_InstanceID;
    outData.chunkCoord = chunkCoord;
    
    gl_Position = vec4(pos.xy,0,1);
}
