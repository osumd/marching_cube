
#include "sampleNoise.h"  // contains NLQs, NMQs, NHQs, etc.

//Precision
#define MEDIUM_PRECISION 0xabde123
// #define HIGH_PRECISION 0xabde123

//Shape
#define FLOOR_S  0xFFFF03
// #define PLANET 0xFFFF03
// #define CAVES 0xFFFF03

//Surface Structure
//#define FLAT_TERRACE
// #define CRUSTY_TERRACE
// #define SPHERICAL_TERRACE

//Random Effects
// #define  GIANT_MOUNTAINS
// #define CURVY_RIDGES
// #define FLAT_SPOTS


vec3 rot(vec3 coord, mat4 mat)
{
  return vec3( dot(mat[0].xyz, coord),   // 3x3 transform,
                 dot(mat[1].xyz, coord),   // no translation
                 dot(mat[2].xyz, coord) );
}

float smooth_snap(float t, float m)
{
  // input: t in [0..1]
  // maps input to an output that goes from 0..1,
  // but spends most of its time at 0 or 1, except for
  // a quick, smooth jump from 0 to 1 around input values of 0.5.
  // the slope of the jump is roughly determined by 'm'.
  // note: 'm' shouldn't go over ~16 or so (precision breaks down).

  //float t1 =     pow((  t)*2, m)*0.5;
  //float t2 = 1 - pow((1-t)*2, m)*0.5;
  //return (t > 0.5) ? t2 : t1;
  
  // optimized:
  float c = (t > 0.5) ? 1 : 0;
  float s = 1-c*2;
  return c + s*pow((c+s*t)*2, m)*0.5;  
}



float DENSITY(vec3 ws)
{
  
  //-----------------------------------------------
  // This function determines the shape of the entire terrain.
  //-----------------------------------------------
 
  // Remember the original world-space coordinate, 
  // in case we want to use the un-prewarped coord.
  // (extreme pre-warp can introduce small error or jitter to
  //  ws, which, when magnified, looks bad - so in those
  //  cases it's better to use ws_orig.)
  vec3 ws_orig = ws;
  
    // start our density value at zero.
  // think of the density value as the depth beneath the surface 
  // of the terrain; positive values are inside the terrain, and 
  // negative values are in open air.
  float density = 0;
  
  // sample an ultra-ultra-low-frequency (slowly-varying) vec4 
  // noise value we can use to vary high-level terrain features 
  // over space.
  vec4 uulf_rand  =           clamp( NMQu(ws*0.000718, noiseVol0) * 2 - 0.5 , 0.0, 1.0);
  vec4 uulf_rand2 =           NMQu(ws*0.000632, noiseVol1);
  vec4 uulf_rand3 =           NMQu(ws*0.000695, noiseVol2);


  //-----------------------------------------------
  // PRE-WARP the world-space coordinate.
  const float prewarp_str = 25;   // recommended range: 5..25
  vec3 ulf_rand = vec3(0,0,0);

  #if MEDIUM_PRECISION  // medium-quality version; precision breaks down when pre-warp is strong.
    ulf_rand =     NMQs(ws*0.0041      , noiseVol2).xyz*0.64
                 + NMQs(ws*0.0041*0.427, noiseVol3).xyz*0.32;
  #endif
  
  #if HIGH_PRECISION  // high-quality version
    // CAREFUL: NHQu/s (high quality) RETURN A SINGLE FLOAT, not a vec4!
    ulf_rand.x =     NHQs(ws*0.0041*0.971, packedNoiseVol2, 1)*0.64
                   + NHQs(ws*0.0041*0.461, packedNoiseVol3, 1)*0.32;
    ulf_rand.y =     NHQs(ws*0.0041*0.997, packedNoiseVol1, 1)*0.64
                   + NHQs(ws*0.0041*0.453, packedNoiseVol0, 1)*0.32;
    ulf_rand.z =     NHQs(ws*0.0041*1.032, packedNoiseVol3, 1)*0.64
                   + NHQs(ws*0.0041*0.511, packedNoiseVol2, 1)*0.32;
  #endif
  
  ws += ulf_rand.xyz * prewarp_str * clamp(uulf_rand.x*1.4 - 0.3,0.0,1.0);

  //-----------------------------------------------
  // compute 8 randomly-rotated versions of 'ws'.  
  // we probably won't use them all, but they're here for experimentation.
  // (and if they're not used, the shader compiler will optimize them out.)
  vec3 c0 = rot(ws,octaveMat0);
  vec3 c1 = rot(ws,octaveMat1);
  vec3 c2 = rot(ws,octaveMat2);
  vec3 c3 = rot(ws,octaveMat3);
  vec3 c4 = rot(ws,octaveMat4);
  vec3 c5 = rot(ws,octaveMat5);
  vec3 c6 = rot(ws,octaveMat6);
  vec3 c7 = rot(ws,octaveMat7);


  //-----------------------------------------------
  // MAIN SHAPE: CHOOSE ONE
  
    #if FLOOR_S
    // very general ground plane:
    density = -ws.y * 1;
    // to add a stricter ground plane further below:
    density += clamp((-4 - ws_orig.y*0.3)*3.0, 0.0,1.0)*40 * uulf_rand2.z;
    #endif

    #if S_PLANET
    // small planet:
    const float planet_str = 2;
    const float planet_rad = 160;
    float dist_from_surface = planet_rad - length(ws);
    density = dist_from_surface * planet_str;
    #endif

    #if S_CAVES
    // infinite network of caves:  (small bias)
    density = 12;  // positive value -> more rock; negative value -> more open space
    #endif
  
  
  //----------------------------------------
     // CRUSTY SHELF
    // often creates smooth tops (~grass) and crumbly, eroded underneath parts.
    #if CRUSTY_TERRACE
      float shelf_thickness_y = 2.5;//2.5;
      float shelf_pos_y = -1;//-2;
      float shelf_strength = 9.5;   // 1-4 is good
      density = mix(density, shelf_strength, 0.83*clamp(  shelf_thickness_y - abs(ws.y - shelf_pos_y), 0, 1.0 ) * clamp(uulf_rand.y*1.5-0.5,0,1.0) );
    #endif   

    // CRUSTY SHELF
    // often creates smooth tops (~grass) and crumbly, eroded underneath parts.
    #if CRUSTY_TERRACE
      float shelf_thickness_y = 2.5;//2.5;
      float shelf_pos_y = -1;//-2;
      float shelf_strength = 9.5;   // 1-4 is good
      density = mix(density, shelf_strength, 0.83*clamp(  shelf_thickness_y - abs(ws.y - shelf_pos_y, 0, 1.0) ) * clamp(uulf_rand.y*1.5-0.5, 0, 1.0) );
    #endif    
    
    // FLAT TERRACES
    #if FLAT_TERRACE
    {
      const float terraces_can_warp = 0.5 * uulf_rand2.y;
      const float terrace_freq_y = 0.13;
      const float terrace_str  = 3*clamp(uulf_rand.z*2-1,0,1.0);  // careful - high str here diminishes strength of noise, etc.
      const float overhang_str = 1*clamp(uulf_rand.z*2-1,0,1.0);  // careful - too much here and LODs interfere (most visible @ silhouettes because zbias can't fix those).
      float fy = -mix(ws_orig.y, ws.y, terraces_can_warp)*terrace_freq_y;
      float orig_t = frac(fy);
      float t = orig_t;
      t = smooth_snap(t, 16);  // faster than using 't = t*t*(3-2*t)' four times
      fy = floor(fy) + t;
      density += fy*terrace_str;
      density += (t - orig_t) * overhang_str;
    }
    #endif

    // SPHERICAL TERRACES (for planet mode)
    #if SPHERICAL_TERRACE
    {
      const float terraces_can_warp = 0.1;   //TWEAK
      const float terrace_freq_r = 0.2;
      //const float terrace_str = 0;   // careful - high str here diminishes strength of noise, etc.
      const float overhang_str = 2;  // careful - too much here and LODs interfere (most visible @ silhouettes because zbias can't fix those).
      float r = length(ws);
      float r_orig = length(ws_orig);
      float fy = -mix(r_orig, r, terraces_can_warp)*terrace_freq_r;
      float orig_t = frac(fy);
      float t = orig_t;
      t = smooth_snap(t, 16);  // faster than using 't = t*t*(3-2*t)' four times
      fy = floor(fy) + t;
      //density += fy*terrace_str;
      density += (t - orig_t) * overhang_str;
    }
    #endif

    // other random effects...
    #if CURVY_RIDGES
      // repeating ridges on [warped] Y coord:
      density += NLQs(ws.xyz*vec3(2,27,2)*0.0037, noiseVol0).x*2 * saturate(uulf_rand2.w*2-1);
    #endif
    #if GIANT_MOUNTAINS
      // to make it extremely mountainous & climby:
      density += ulf_rand.x*80;
    #endif

    #ifdef EVAL_CHEAP   //...used for fast long-range ambo queries
      float HFM = 0;
    #else 
      float HFM = 1;
    #endif

    // sample 9 octaves of noise, w/rotated ws coord for the last few.
    // note: sometimes you'll want to use NHQs (high-quality noise)
    //   instead of NMQs for the lowest 3 frequencies or so; otherwise
    //   they can introduce UNWANTED high-frequency noise (jitter).
    //   BE SURE TO PASS IN 'PackedNoiseVolX' instead of 'NoiseVolX'
    //   WHEN USING NHQs()!!!
    // note: higher frequencies (that don't matter for long-range
    //   ambo) should be modulated by HFM so the compiler optimizes
    //   them out when EVAL_CHEAP is #defined.
    // note: if you want to randomly rotate various octaves,
    //   feed c0..c7 (instead of ws) into the noise functions.
    //   This is especially good to do with the lowest frequency,
    //   so that it doesn't repeat (across the ground plane) as often...
    //   and so that you can actually randomize the terrain!
    //   Note that the shader compiler will skip generating any rotated
    //   coords (c0..c7) that are never used.
    density += 
           ( 0
             //+ NLQs(ws*0.3200*0.934, noiseVol3).x*0.16*1.20 * HFM // skipped for long-range ambo
             + NLQs(ws*0.1600*1.021, noiseVol1).x*0.32*1.16 * HFM // skipped for long-range ambo
             + NLQs(ws*0.0800*0.985, noiseVol2).x*0.64*1.12 * HFM // skipped for long-range ambo
             + NLQs(ws*0.0400*1.051, noiseVol0).x*1.28*1.08 * HFM // skipped for long-range ambo
             + NLQs(ws*0.0200*1.020, noiseVol1).x*2.56*1.04
             + NLQs(ws*0.0100*0.968, noiseVol3).x*5 
             + NMQs(ws*0.0050*0.994,       noiseVol0).x*10*1.0 // MQ
             + NMQs(c6*0.0025*1.045,       noiseVol2).x*20*0.9 // MQ
             + NHQs(c7*0.0012*0.972, packedNoiseVol3,1.0).x*40*0.8 // HQ and *rotated*!
           );
             
  #if FLAT_SPOTS
    {
      const float flat_spot_str = 0;  // 0=off, 1=on
      const float dist_between_spots = 330;
      const float spot_inner_rad = 44;
      const float spot_outer_rad = 66;
      vec2 spot_xz = floor(ws.xz/dist_between_spots) + 0.5;
      float dist = length(ws.xz - spot_xz*dist_between_spots);
      float t = saturate( (spot_outer_rad - dist)/(spot_outer_rad - spot_inner_rad) );
      t = (3 - 2*t)*t*t;
      density = lerp(density, -ws.y*1, t*0.9*flat_spot_str);
    }   
  #endif 
    
    
             
  // LOD DENSITY BIAS:
  // this shrinks the lo- and medium-res chunks just a bit,
  // so that the hi-res chunks always "enclose" them:
  // (helps avoid LOD overdraw artifacts)
  density -= wsChunkSize.x*0.009;
        
  return density;
  
  return 0;
}


