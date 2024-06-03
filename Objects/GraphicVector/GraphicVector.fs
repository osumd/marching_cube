#version 460 core

in vec2 o_uv;
uniform float time;
void main()
{

    gl_FragColor = vec4(o_uv.x*cos(time), o_uv.y, 0, 1);

}