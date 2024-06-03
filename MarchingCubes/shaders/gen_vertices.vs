#version 460 core

layout(location = 0) in uint z8_y8_x8_null4_edgeNum4;

in VertexData {
  vec4 wsCoordAmbo;
  vec3 wsNormal   ;
} inData;

// 'ChunkCB' is updated each time we want to build a chunk:
layout (std140) uniform ChunkCB {
  vec3 wsChunkPos; //wsCoord of lower-left corner
  float  opacity;
}

uniform Sampler3D density_vol;
uniform Sampler3D noiseVol0;
uniform Sampler3D noiseVol1;
uniform Sampler3D noiseVol2;
uniform Sampler3D noiseVol3;
uniform Sampler3D packedNoiseVol0;
uniform Sampler3D packedNoiseVol1;
uniform Sampler3D packedNoiseVol2;
uniform Sampler3D packedNoiseVol3;

#include "master.h"
#include "LodCB.h"

#define EVAL_NO_MIPS
#define EVAL_CHEAP

#include "density.h"

layout (std140) uniform g_mc_lut {
  uint  case_to_numpolys[256];
  vec3  edge_start[12];
  vec3  edge_dir[12];
  vec3  edge_end[12];
  uint   edge_axis[12];  // 0 for x edges, 1 for y edges, 2 for z edges.
};

#define AMBO_STEPS 16
layout (std140) uniform g_ambo_lut {
  float  ambo_dist[16];
  vec4 occlusion_amt[16];
  vec3 g_ray_dirs_32[32];  // 32 rays w/a good poisson distrib.
  vec3 g_ray_dirs_64[64];  // 64 rays w/a good poisson distrib.
  vec3 g_ray_dirs_256[256];  // 256 rays w/a good poisson distrib.
};

#if (AMBO_RAYS==32)
  #define g_ray_dirs g_ray_dirs_32
#elif (AMBO_RAYS==64)
  #define g_ray_dirs g_ray_dirs_64
#elif (AMBO_RAYS==128)
  #define g_ray_dirs g_ray_dirs_256
#else
  ERROR
#endif

struct vertex {
  vec4 worldCoordAmbo ;     // .w = occlusion
  vec3 worldNormalMisc;       
};


vertex PlaceVertOnEdge( vec3 wsCoord_LL, vec3 uvw_LL, int edgeNum )
{
  vertex outPut;
  
  // get the density values at the two ends of this edge of the cell,
  // then interpolate to find the point (t in 0..1) along the edge 
  // where the density value hits zero.  
  float str0 = texture(density_vol, uvw_LL + InvVoxelDimPlusMarginsMinusOne.xxx*edge_start[edgeNum], 0).x;
  float str1 = texture(density_vol, uvw_LL + InvVoxelDimPlusMarginsMinusOne.xxx*edge_end  [edgeNum], 0).x;
  float t = clamp( str0/(str0 - str1), 0.0, 1.0 );  // 'saturate' keeps occasional crazy stray triangle from appearing @ edges

  // reconstruct the interpolated point & place a vertex there.
  vec3 pos_within_cell = edge_start[edgeNum] + vec3(t,t,t)*edge_dir[edgeNum];  //0..1
  vec3 wsCoord = wsCoord_LL + pos_within_cell*wsVoxelSize.xxx;
  vec3 uvw     = uvw_LL     + pos_within_cell*InvVoxelDimPlusMarginsMinusOne.xxx;

  outPut.worldCoordAmbo.xyz  = wsCoord.xyz;
  
  
  // generate ambient occlusion for this vertex
  float ambo;

  const float cells_to_skip_at_ray_start = 1.25;

  //float AMBO_RAY_DIST_CELLS = min(MAX_AMBO_RAY_DIST_CELLS, VoxelDimPlusMargins*0.18);
  float AMBO_RAY_DIST_CELLS = VoxelDimPlusMargins*0.25;

// so that ambo looks the same if we change the voxel dim:
  vec3 inv_voxelDim_tweaked = InvVoxelDimPlusMargins.xxx * VoxelDimPlusMargins/160.0;
    
    for (int i=0; i<AMBO_RAYS; i++) 
    {
        // cast a ray through uvw space
        vec3 ray_dir = g_ray_dirs[ i ];
        vec3 ray_start = uvw;
        vec3 ray_now = ray_start + ray_dir*InvVoxelDimPlusMargins.xxx*cells_to_skip_at_ray_start;  // start a little out along the ray
        vec3 ray_delta =           ray_dir*inv_voxelDim_tweaked*AMBO_RAY_DIST_CELLS.xxx;
        
        
        float ambo_this = 1;
        
        // SHORT RANGE:
        //  -step along the ray at AMBO_STEPS points,
        //     sampling the density volume texture
        //  -occlusion_amt[] LUT makes closer occlusions have more weight than far ones
        //  -start sampling a few cells away from the vertex, to reduce noise.
        ray_delta *= (1.0/(AMBO_STEPS));
        for (int j=0; j<AMBO_STEPS; j++) 
        {   
            ray_now += ray_delta;
            float t = texture(density_vol, ray_now, 0);
            ambo_this = lerp(ambo_this, 0, saturate(t*6) 
                                            * occlusion_amt[j].z//* pow(1-j/(float)AMBO_STEPS,0.4)
            );
            
        
            // LONG RANGE: 
            //   also take a few samples far away,
            //   using the density *function*.
            for (int j=0; j<5; j++) {   
                // be sure to start some distance away, otherwise same vertex
                // in different LODs might have different brightness!  
                // (due to density function LOD bias)
                float dist = (j+2)/5.0;
                dist = pow(dist, 1.8);
                dist *= 40;
                float t = DENSITY(wsCoord + ray_dir*dist);
                const float shadow_hardness = 0.5;
                ambo_this *= 0.1 + 0.9*clamp(-t*shadow_hardness + 0.3, 0.0, 1.0);
            }      
        
            ambo_this *= 1.4;
        
            ambo += ambo_this;
        }

        ambo *= (1.0/(AMBO_RAYS));
    } 

  outPut.worldCoordAmbo.w = ambo;
  

  // figure out the normal vector for this vertex
  float3 grad;
  grad.x =   texture(density_vol, uvw + InvVoxelDimPlusMargins.xyy, 0)
           - texture(density_vol, uvw - InvVoxelDimPlusMargins.xyy, 0);
  grad.y =   texture(density_vol, uvw + InvVoxelDimPlusMargins.yxy, 0)
           - texture(density_vol, uvw - InvVoxelDimPlusMargins.yxy, 0);
  grad.z =   texture(density_vol, uvw + InvVoxelDimPlusMargins.yyx, 0)
           - texture(density_vol, uvw - InvVoxelDimPlusMargins.yyx, 0);
  outPut.worldNormalMisc.xyz = -normalize(grad);
  
 
  return outPut;
}

void main()
{
  ivec3 unpacked_coord;
  unpacked_coord.x = (inData.z8_y8_x8_null4_edgeNum4 >>  8) & 0xFF;
  unpacked_coord.y = (inData.z8_y8_x8_null4_edgeNum4 >> 16) & 0xFF;
  unpacked_coord.z = (inData.z8_y8_x8_null4_edgeNum4 >> 24) & 0xFF;
  
  vec3 chunkCoordWrite = (vec3)unpacked_coord * InvVoxelDimMinusOne.xxx;
  vec3 chunkCoordRead  = (Margin + VoxelDimMinusOne*chunkCoordWrite)*InvVoxelDimPlusMarginsMinusOne.xxx;

  vec3 wsCoord = wsChunkPos + chunkCoordWrite*wsChunkSize;
    
  // very important: ws_to_uvw() should subtract 1/2 a texel in XYZ, 
  // to prevent 1-bit float error from snapping to wrong cell every so often!  
  vec3 uvw = chunkCoordRead + InvVoxelDimPlusMarginsMinusOne.xxx*0.25;
    // HACK #2
  uvw.xyz *= (VoxelDimPlusMargins.x-1)*InvVoxelDimPlusMargins.x;

  // generate a vertex along this edge.
  int edgeNum = (inData.z8_y8_x8_null4_edgeNum4 & 0x0F);
  vertex v = PlaceVertOnEdge( wsCoord, uvw, edgeNum );     

  // send it to the GS for stream out.
  
  outData.wsCoordAmbo = v.worldCoordAmbo;
  outData.wsNormal    = v.worldNormalMisc;
  gl_FragColor = vec4(0.0,0.0,0.0,0.0);
}