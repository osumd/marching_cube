#define MAX_INSTANCES      256
#define NOISE_LATTICE_SIZE 16

#define FOG_COLOR vec3(0.161,0.322,0.588) // must match mainDrawPass2 in models\main.nma!

#define INV_LATTICE_SIZE (1.0/float(NOISE_LATTICE_SIZE))

#define HEED_THRESH 160 //0.5 wasn't quite enough - some chunks got missed

#define AMBO_RAYS            32 //64 //DO NOT EXCEED 64 (size of table). (it's a nice 64-point poisson distribution.)

// v- should match BUFFER_SLICES in src\blobs\BlobbyConstants.h:
// v- should match BUFFER_SLICES in src\blobs\BlobbyConstants.h:
#define MAX_AMBO_RAY_DIST_CELLS  24 
// ^- should match BUFFER_SLICES in src\blobs\BlobbyConstants.h!
// ^- should match BUFFER_SLICES in src\blobs\BlobbyConstants.h!
  
// also tweak values in these LUT's:
//   occlusion_amt[]
//   ambo_dist[]

// layout (std140) uniform g_GlobalRockCB {
//   //uint3    voxelDim;         // 128, 448, 128
//   //vec4   inv_voxelDim;     // 1/128, 1/448, 1/128, 0
//   //vec2   wsVoxelSize;      // 2/(128-1), (128-1)/2 [inverse], 0, 0
//   //vec2   uvw_to_ws_y;      // ~7, ~0.14
//     // 'uvw_to_ws_y' helps you map between UVW space [0..1] for sampling the 3d texture,
//     //   and world space Y [-1..1, 0..6..7ish, -1..1]
//     // Note that in world space, Y is up; in the 3d texture, Z is up! (slices~z)
//     // See "uvw_to_ws()" and "uvw_to_ws()" helper functions below.
mat4 octaveMat0;
mat4 octaveMat1;
mat4 octaveMat2;
mat4 octaveMat3;
mat4 octaveMat4;
mat4 octaveMat5;
mat4 octaveMat6;
mat4 octaveMat7;
  
//   // use this for uvw stepping within a cell.  
//   // notice it's only minus one on .xz!!:
//   //vec4   inv_voxelDimMinusOne; // 1/127, 1/448, 1/127, 0
  
//   vec4   timeValues;   // .x=anim time, .y=real time, .zw=0
//   vec3   wsEyePos;
//   vec3   wsLookAt;
// };

//vec3 uvw_to_ws(vec3 uvw) {
//  // note: this is a one to many mapping, so your absolute WS y coord
//  //       might not be accurate.
//  // uvw -> ws:   ws.xzy = uvw.xyz*vec3(2.0,2.0,uvw_to_ws_y.x) + vec3(-1,-1,0);
//  return (uvw.xyz*vec3(2.0,2.0,uvw_to_ws_y.x) + vec3(-1,-1,0)).xzy;
//}
//
//vec3 ws_to_uvw(vec3 ws) {
//  // note: use results with 'repeat' sampling on Y in order to work!
//  // ws -> uvw:   uvw.xyz = ws.xzy*vec3(0.5,0.5,uvw_to_ws_y.y) + vec3(0.5,0.5,0);
//  vec3 uvw = (ws.xzy*vec3(0.5,0.5,uvw_to_ws_y.y) + vec3(0.5,0.5,0)).xyz;
//  
//  // note that any half-voxel translations you do here 
//  // (in Y, for example) will affect both the rock AND the
//  // vines/water; they are both built from the field, and 
//  // ws_to_uvw is used for sampling field values from the field.
//  // so it's quite useless to shift ANYTHING here.
//  
//  // HOWEVER... we used to subtract 1/2 a voxel in Y here, to fix the
//  // nearest-neighbor sampling in build_rock.vsh, which would 
//  // occasionally (due to 1-bit float error) snap to the 
//  // wrong Y slice!  We now use bilinear sampling (and a 1/2 texel
//  // offset all around), though, so it's unnecessary.
//  
//  //uvw.z -= 0.5*inv_voxelDimMinusOne.y;
//  
//  // So... the only reason to move things here is so that the 
//  // rock geom generation hits the right spots to get good 
//  // trilinear interpolation.  ALL geom (rock, vines, water)
//  // will shift together, so no big deal there.
//  // It turns out that the field sampling for building the 
//  // rock polys needs a shift of +0.5 cells in XY, 
//  // but no shift in Z, to get good interpolation.
//  uvw.xy += 0.5*inv_voxelDimMinusOne.xz;
//  
//  return uvw;
//}

vec3 vecMul(mat4 m, vec3 v) {
  return vec3(dot(m[0].xyz, v), dot(m[1].xyz, v), dot(m[2].xyz, v));
}
float  smoothy(float  t) { return t*t*(3-2*t); }
vec2 smoothy(vec2 t) { return t*t*(3-2*t); }
vec3 smoothy(vec3 t) { return t*t*(3-2*t); }
vec4 smoothy(vec4 t) { return t*t*(3-2*t); }
