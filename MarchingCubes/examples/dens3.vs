#version 460 core

// input here is some # of instances of quadProxy_geo (a big quad to be rasterized)

layout(location = 0) in vec4 pos; //input each point of a subdivided volume chunk


out VertexData
{
    vec4 wsCoord;
    vec3 chunkCoord;
    int   nInstanceID ;
    int depth;
} outData;

uniform int depth;

void main()
{
    vec2 tex = pos.zw;
    // chunkCoord is in [0..1] range
    vec3 chunkCoord = vec3(tex.xy,gl_InstanceID); 
    
      // HACK #1 - because in DX, when you render a quad with uv's in [0..1],
      //           the upper left corner will be a 0 and the lower right
      //           corner will be at 1 (it should be at 63/64, or whatever).
    //chunkCoord.xyz *= VoxelDim.x*InvVoxelDimMinusOne.x;  
      //chunkCoord.xy *= VoxelDimPlusMargins.x*InvVoxelDimPlusMarginsMinusOne.x;  // HACK #1 

    // extChunkCoord goes outside that range, so we also compute
    // some voxels outside of the chunk
    outData.wsCoord    = vec4(1);
    outData.nInstanceID = gl_InstanceID;
    outData.chunkCoord = chunkCoord;
    outData.depth = depth;
    
    //gl_Position = vec4((pos.xy*1/5) - (1-(1/5)),0,1);
    gl_Position = vec4(pos.xy, 0, 1);
}
