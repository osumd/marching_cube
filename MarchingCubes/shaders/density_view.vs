#version 460 core
layout(location = 0) in vec4 pos;

out vec2 tex;
void main()
{
    tex = pos.zw;

    gl_Position = vec4(pos.xy,0.0,1.0);
}