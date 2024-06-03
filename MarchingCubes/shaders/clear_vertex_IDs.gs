#version 460 core

layout(triangle_strip) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
  vec4 POSITION;
  uint  nInstanceID;
} inData[];

out VertexData {
  vec4 POSITION;
  uint RTIndex;
} outData;

void main()
{
  for (int i=0; i<3; i++) 
  {
    outData.POSITION = inData[i].POSITION;
    outData.RTIndex  = inData[i].nInstanceID;
    EmitVertex();
  }
  EmitPrimitve();
}
