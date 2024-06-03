#version 460 core

in VertexData
{
    int layer;
    vec2 uv;
} inData;


void main()
{   
    //gl_FragColor = vec4(0.0, 1,1.0,1.0);
    if(gl_Layer == 0)
    {
        gl_FragColor = vec4(1.0, 0,0.0,1.0);
    }
    if(gl_Layer == 1)
    {
        gl_FragColor = vec4(0.0, 1.0,0.0,1.0);
    }
    if(gl_Layer == 2)
    {
        gl_FragColor = vec4(0.0, 0.0, 1.0,1.0);
    }
    if(gl_Layer == 3)
    {
        gl_FragColor = vec4(1.0, 0.0,1.0,1.0);
    }
    if(gl_Layer == 4)
    {
        gl_FragColor = vec4(0.0, 1,1.0,1.0);
    }
    
}