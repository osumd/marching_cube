
// This file contains helper functions for sampling noise volumes.
// 
//  NLQu    sample noise, low quality, unsigned
//  NLQs    sample noise, low quality, signed
//  NMQu    sample noise, medium quality, unsigned
//  NMQs    sample noise, medium quality, signed
//  NHQu    sample noise, high quality, unsigned
//  NHQs    sample noise, high quality, signed
//
// WARNING: DON'T USE THESE HELPER FUNCTIONS FROM PIXEL SHADERS
//          DOING TEXTURING - THEY ALL FORCE THE ZERO MIP LEVEL
//          AND YOU'LL GET ALIASING.
//
// The low and medium qualities are pretty close to each other,
// both in computational complexity and speed.  The high quality
// functions do manual trilinear interpolation and are much
// slower, but also much more accurate, especially when 'ws'
// is changing very slowly over space, and the result of the
// noise value fetched is being highly amplified.  
//
// The low quality is a simple HW trilinear fetch.
// The medium quality is similar but first-order continuous,
//   because it warps the input coordinate to be first-order
//   continuous.  (Looks much better for lighting because
//   lighting is based on the derivative.)
// The high quality is much slower.  It can optionally do the 
//   warping/smoothing that the medium quality gives you
//   (set 'smooth' to zero or one).  See comments inside
//   the function itself, as well as the file 
//   textures\about_these_noise_volumes.txt.

#include "master.h"

vec4 NLQu( vec3 uvw, sampler3D noiseTex ) {
  return texture(noiseTex, uvw, 0);
}
vec4 NLQs( vec3 uvw, sampler3D noiseTex ) {
  return NLQu(uvw, noiseTex)*2-1;
}

vec4 NMQu( vec3 uvw, sampler3D noiseTex ) {
  // smooth the input coord
  vec3 t = fract(uvw * NOISE_LATTICE_SIZE + 0.5);
  vec3 t2 = (3 - 2*t)*t*t;

  int lattice_size = NOISE_LATTICE_SIZE;
  vec3 uvw2 = uvw + (t2-t)/float(lattice_size);
  // fetch
  return NLQu(uvw2, noiseTex);
}

vec4 NMQs( vec3 uvw, sampler3D noiseTex ) {
  // smooth the input coord
  vec3 t = fract(uvw * NOISE_LATTICE_SIZE + 0.5);
  vec3 t2 = (3 - 2*t)*t*t;
  vec3 uvw2 = uvw + (t2-t)/float(NOISE_LATTICE_SIZE);
  // fetch  
  return NLQs(uvw2, noiseTex);
}


// SUPER MEGA HIGH QUALITY noise sampling (signed)
float NHQu(vec3 uvw, sampler3D tex, float smoove) 
{


  vec3 uvw2 = floor(uvw * NOISE_LATTICE_SIZE) * INV_LATTICE_SIZE;
  vec3 t    = (uvw - uvw2) * NOISE_LATTICE_SIZE;
  t = mix(t, t*t*(3 - 2*t), smoove); //previusly mix
 
  vec2 d = vec2( INV_LATTICE_SIZE, 0 );

  #if 0
    // the 8-lookup version... (SLOW)
    vec4 f1 = vec4( texture(tex, uvw2 + d.xxx, 0).x, 
                        texture(tex, uvw2 + d.yxx, 0).x, 
                        texture(tex, uvw2 + d.xyx, 0).x, 
                        texture(tex, uvw2 + d.yyx, 0).x );
    vec4 f2 = vec4( texture(tex, uvw2 + d.xxy, 0).x, 
                        texture(tex, uvw2 + d.yxy, 0).x, 
                        texture(tex, uvw2 + d.xyy, 0).x, 
                        texture(tex, uvw2 + d.yyy, 0).x );
    vec4 f3 = mix(f2, f1, t.zzzz);
    float2 f4 = mix(f3.zw, f3.xy, t.yy);
    float  f5 = mix(f4.y, f4.x, t.x);
  #else
    // THE TWO-SAMPLE VERSION: much faster!
    // note: requires that three YZ-neighbor texels' original .x values
    //       are packed into .yzw values of each texel.
    vec4 f1 = texture(tex, uvw2        , 0).zxyw; // <+0, +y, +z, +yz>
    vec4 f2 = texture(tex, uvw2 + d.xyy, 0).zxyw; // <+x, +xy, +xz, +xyz>
    vec4 f3 = mix(f1, f2, t.xxxx);  // f3 = <+0, +y, +z, +yz>
    vec2 f4 = mix(f3.xy, f3.zw, t.yy); // f4 = <+0, +z>
    float  f5 = mix(f4.x, f4.y, t.z);
  #endif
  
  return f5;
}

float NHQs(vec3 uvw, sampler3D tex, float smoove) {
  return NHQu(uvw, tex, smoove)*2-1;
}

