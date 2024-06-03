#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexData
{
    vec4 wsCoord;
    vec3 chunkCoord;
    int   nInstanceID ;
    int depth;
} inData[];

out VertexData
{   
    vec4 wsCoord;
    vec3 chunkCoord;
    flat int nInstanceID ;
    
} outData;


void main()
{
    for (int v = 0; v < 3; v++) //just needs to get render target using gl_Layer
    {
        outData.wsCoord = inData[v].wsCoord;
        outData.nInstanceID = inData[v].nInstanceID;
        gl_Layer        = (inData[v].nInstanceID); //instance id to set layer index to array value
        //gl_Layer = inData[v].depth;
        outData.chunkCoord = inData[v].chunkCoord;

        int layer_count = 4;

        //vec4 adjustment_debug = vec4(inData[v].nInstanceID*0.5, 0.0, 0.0, 0.0);
        vec4 adjustment_debug = vec4(inData[v].nInstanceID*0.5, 0.0, 0.0, 0.0);
        //gl_Position = (gl_in[v].gl_Position * (1.0/4.0)) /* + adjustment_debug */;
        //gl_Position = (gl_in[v].gl_Position * vec4(1.0/4.0, 1.0/4.0, 1, 1.0)) + vec4(-(1-(1.0/4.0)),-(1-(1.0/4.0)), 0.0, 0.0)
                    + (inData[v].nInstanceID * (1.0/4.0));
        gl_Position = gl_in[v].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
