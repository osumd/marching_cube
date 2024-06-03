#version 460 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

out vec2 o_uv;

void main()
{
    o_uv = uv;
    gl_Position = vec4(pos.xy, 0,1);

}