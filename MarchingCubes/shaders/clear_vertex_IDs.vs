#version 460 core

layout (location = 0) in vec3 objCoord;
layout (location = 1) in vec2 tex;

out VertexData {
  vec4 POSITION ;
  uint  nInstanceID;
} outData;

void main()
{
    
    vec4 projCoord = vec4(objCoord.xyz, 1);
    projCoord.y *= -1;               // flip Y coords for DX
    
    outData.POSITION = projCoord;
    outData.nInstanceID = gl_InstanceID;

    gl_Position = vec4(objCoord, 1.0);
}