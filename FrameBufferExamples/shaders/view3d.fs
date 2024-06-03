#version 460 core

in vec2 o_uv;

uniform sampler3D myTexture;

uniform float layerID;

void main()
{
    
    gl_FragColor = texture(myTexture, vec3(o_uv.x, o_uv.y, 2.0/4.0), 0.0);

    //gl_FragColor = vec4(0.1,0,0,1);

}