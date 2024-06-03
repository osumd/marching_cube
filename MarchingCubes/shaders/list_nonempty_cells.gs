#version 460 core

layout (points) in;
layout (points, max_vertices = 1) out;

in VertexData
{
    uint z8_y8_x8_case8;
} inData[];

out VertexData
{   
    uint z8_y8_x8_case8;
} outData;

void main()
{
  uint cube_case = (inData[0].z8_y8_x8_case8 & 0xFF);
  if (cube_case * (255-cube_case) > 0)
  {
    outData.z8_y8_x8_case8 = inData[0].z8_y8_x8_case8; //then need to store the value and write it to texture
    EmitVertex();
    EndPrimitive();
  }
}