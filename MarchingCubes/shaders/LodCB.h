layout (std140) uniform LodCB {
  float  VoxelDim ;          // # of cell corners
  float  VoxelDimMinusOne ;  // # of cells
  vec2 wsVoxelSize ;  // could be 1/63, 1/31, 1/15 depending on LOD
  float  wsChunkSize ;                  // 1.0, 2.0, or 4.0 depending on LOD
  vec2 InvVoxelDim ; 
  vec2 InvVoxelDimMinusOne ; 
  float  Margin                      ;
  float  VoxelDimPlusMargins         ;
  float  VoxelDimPlusMarginsMinusOne ;
  vec2 InvVoxelDimPlusMargins         ;
  vec2 InvVoxelDimPlusMarginsMinusOne ;
};

vec3 ChunkCoord_To_ExtChunkCoord(vec3 chunkCoord)
{ 
  // if VoxelDim is 65 
  // (i.e. 65 corners / 64 cells per block)
  // then chunkCoord should be in [0..64/65]
  // and extChunkCoord (returned) will be outside that range.
  return (chunkCoord*VoxelDimPlusMargins.xxx - Margin.xxx)*InvVoxelDim.xxx;
}

vec3 ExtChunkCoord_To_ChunkCoord(vec3 extChunkCoord)
{ 
  return (extChunkCoord*VoxelDim.xxx + Margin.xxx)*InvVoxelDimPlusMargins.xxx;
}