#version 460 core

in VertexData {
  vec4 wsCoord;
  vec3 chunkCoord;
  int nInstanceID;
} inData;


void main()
{
  //gl_FragColor = vec4(DENSITY(inData.wsCoord.xyz)); //Suppose the density texture slice is actually mapped to the framebuffer
  gl_FragColor = vec4(inData.chunkCoord.x, inData.chunkCoord.x, inData.chunkCoord.x, 1.0);
}