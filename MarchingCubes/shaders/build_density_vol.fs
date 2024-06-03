#version 460 core



in VertexData {
  vec4 wsCoord;
  vec3 chunkCoord;
  uint RTIndex;
} inData;

uniform sampler3D noiseVol0;
uniform sampler3D noiseVol1;
uniform sampler3D noiseVol2;
uniform sampler3D noiseVol3;
uniform sampler3D packedNoiseVol0;
uniform sampler3D packedNoiseVol1;
uniform sampler3D packedNoiseVol2;
uniform sampler3D packedNoiseVol3;

#include "lodCB.h"

#include "density.glsl"

void main()
{
  //gl_FragColor = vec4(DENSITY(inData.wsCoord.xyz)); //Suppose the density texture slice is actually mapped to the framebuffer
  gl_FragColor = vec4(inData.chunkCoord.x, inData.chunkCoord.x, inData.chunkCoord.x, 1.0);
}