#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

uniform mat4 projection;
uniform mat4 view;



out vec2 o_uv;


void main(){

    o_uv = uv;
    vec4 position = vec4(pos,1);
    
    gl_Position = projection*view*position;
}