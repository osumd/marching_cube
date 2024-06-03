#version 460 core

// (PASSTHRU)

layout(points) in;
layout(points, max_vertices = 1) out;

in VertexData {
  vec4 wsCoordAmbo;
  vec3 wsNormal   ;
} inData;

out VertexData {
  vec4 worldCoordAmbo ;     // .w = occlusion
  vec4 worldNormal    ;       
} outData;

void main()
{
  
  outData.worldCoordAmbo  = inData[0].wsCoordAmbo;
  outData.worldNormal     = vec4(inData[0].wsNormal, 0);
  EmitVertex();
  EmitPrimitive();
}