#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexData
{
    int layer;
    vec2 uv;
} inData[];

out VertexData
{
    int layer;
    vec2 uv;
} outData;


void main()
{
    for (int v = 0; v < 3; v++) 
    {

        outData.uv = inData[v].uv;
        outData.layer = inData[v].layer;
        gl_Layer = inData[v].layer-1;
        gl_Position = gl_in[v].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
