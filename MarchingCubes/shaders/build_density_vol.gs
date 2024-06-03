#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexData
{
    vec4 wsCoord;
    vec3 chunkCoord;
    int   nInstanceID ;
} inData[];

out VertexData
{   
    vec4 wsCoord;
    vec3 chunkCoord;
    uint   RTIndex ;
} outData;


void main()
{
    for (int v = 0; v < 3; v++) //just needs to get render target using gl_Layer
    {
        outData.wsCoord = inData[v].wsCoord;
        outData.RTIndex = inData[v].nInstanceID;
        //gl_Layer        = (inData[v].nInstanceID-1); //instance id to set layer index to array value
        gl_Layer = 0;
        outData.chunkCoord = inData[v].chunkCoord;
        gl_Position = gl_in[v].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
