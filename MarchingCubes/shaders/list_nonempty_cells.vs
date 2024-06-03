#version 460 core

layout(location = 0) in vec2 uv_write;
layout(location = 1) in vec2 uv_read; //uv_read not really used here, samples the density texture we made previously.

//store uv's subdivided by 63, then run instanced, in terms of writing.

// input: dummy_cells
// we have to put all block points here.
// input here is ~63x63 points, one for each *cell* we'll run marching cubes on.
// note that if VoxelDim is 65   (65 corners/64 cells)
// and Margin is 4
// then the points we actually get in here are [4..67]
// Input is a quadrature screen dimension slice.

out VertexData{
  uint z8_y8_x8_case8;
} outData;

// 'ChunkCB' is updated each time we want to build a chunk:
//Current block or chunck
layout (std140) uniform ChunkCB {
  vec4 wsChunkPos;
};

#include "master.h"
#include "LodCB.h"

uniform sampler3D density_vol;

void main() //this is supposed to sample a single cell or voxel in a chunk for each value of the chunk, and then output cell values that have polygons in them.
{
  int inst = gl_InstanceID;
  
  vec3 chunkCoordRead  = vec3( uv_read.x,//(uv.x*VoxelDimMinusOne + Margin)*InvVoxelDimPlusMarginsMinusOne.x , 
                                   uv_read.y,//(uv.y*VoxelDimMinusOne + Margin)*InvVoxelDimPlusMarginsMinusOne.x ,
                                   (gl_InstanceID + Margin)*InvVoxelDimPlusMargins.x);

  vec3 chunkCoordWrite = vec3(  uv_write.x, 
                                   uv_write.y,
                                   gl_InstanceID * InvVoxelDim.x); //[0,0],[1/32,0],[1/32,1/32],[0,1/32], for each 32 voxel slice
                                  
  vec3 wsCoord = wsChunkPos.xyz + chunkCoordWrite*wsChunkSize;   //FIXME! // lower left -> Cunk(write)
    
  // very important: ws_to_uvw() should subtract 1/2 a texel in XYZ, 
  // to prevent 1-bit float error from snapping to wrong cell every so often!  
  vec3 uvw = chunkCoordRead + vec3(InvVoxelDimPlusMarginsMinusOne.x,InvVoxelDimPlusMarginsMinusOne.x,InvVoxelDimPlusMarginsMinusOne.x)*0.125;
    // HACK #2
  uvw.xy *= ((VoxelDimPlusMargins-1)*InvVoxelDimPlusMargins.x)*((VoxelDimPlusMargins-1)*InvVoxelDimPlusMargins.x);
  
  vec4 field4567;
  vec4 field0123;

  field0123.x = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.yyy).x;
  field0123.y = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.yxy).x;
  field0123.z = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.xxy).x;
  field0123.w = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.xyy).x;

  field4567.x = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.yyx).x;
  field4567.y = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.yxx).x;
  field4567.z = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.xxx).x;
  field4567.w = texture(density_vol, uvw + InvVoxelDimPlusMarginsMinusOne.xyx).x;

  uvec4 i0123 = uvec4(clamp(field0123*99999,0.0,1.0));
  uvec4 i4567 = uvec4(clamp(field4567*99999,0.0,1.0));

  uint cube_case = (i0123.x     ) | (i0123.y << 1) | (i0123.z << 2) | (i0123.w << 3) |
                  (i4567.x << 4) | (i4567.y << 5) | (i4567.z << 6) | (i4567.w << 7);

  uvec3 uint3coord = uvec3(uv_write.xy * vec2(VoxelDimMinusOne,VoxelDimMinusOne), gl_InstanceID);
  
  outData.z8_y8_x8_case8 = (uint3coord.z << 24) |
                        (uint3coord.y << 16) |
                        (uint3coord.x <<  8) |
                        (cube_case         );

  gl_Position = vec4(0,0,0,0);

}
