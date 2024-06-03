#version 460 core

layout(points) in;
layout(points, max_vertices = 3) out;

in VertexData{
    uint z8_y8_x8_null5_edgeFlags3;

} inData[];

out VertexData
{
    uint z8_y8_x8_null4_edgeNum4;
} outData;

void main()
{
  uint z8_y8_x8_null8 = inData[0].z8_y8_x8_null5_edgeFlags3 & 0xFFFFFF00;
  
  if (inData[0].z8_y8_x8_null5_edgeFlags3 & 1) {
    outData.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 3;
    EmitVertex();
    EndPrimitive();
  }
  if (inData[0].z8_y8_x8_null5_edgeFlags3 & 2) {
    outData.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 0;
    EmitVertex();
    EndPrimitive();
  }
  if (inData[0].z8_y8_x8_null5_edgeFlags3 & 4) {
    outData.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 8;
    EmitVertex();
    EndPrimitive();
  }
}
