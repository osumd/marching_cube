#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

out VertexData{
    int layer;
    vec2 uv;

} outData;

void main()
{
    outData.uv = uv;
    outData.layer = gl_InstanceID;

    gl_Position = vec4(pos.xy, 0.0,1.0);
}