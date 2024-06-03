#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 32) out;

in VertexData
{
    vec3 chunk_lower;
    vec3 chunk_upper;
    
    vec3 chunk_front_br;
    vec3 chunk_front_tr;
    vec3 chunk_front_tl;

    vec3 chunk_back_br;
    vec3 chunk_back_bl;
    vec3 chunk_back_tl;

} inData[];


uniform mat4 projection;
uniform mat4 view;

void populate_edge_axes(inout vec3 edge_axes[12], inout vec3 edge_start[12], inout vec3 edge_end[12])
{
    // front left counter clockwise [front face]
    // back left counter clockwise [back face]
    // front left counter clockwise [front back face connectors]

    vec3 chunk_lower = inData[0].chunk_lower;
    vec3 chunk_upper = inData[0].chunk_upper;
    
    vec3 chunk_front_br = inData[0].chunk_front_br;
    vec3 chunk_front_tr = inData[0].chunk_front_tr;
    vec3 chunk_front_tl = inData[0].chunk_front_tl;

    vec3 chunk_back_br = inData[0].chunk_back_br;
    vec3 chunk_back_tl = inData[0].chunk_back_tl;
    vec3 chunk_back_bl = inData[0].chunk_back_bl;

    edge_axes[0] = chunk_front_br - chunk_lower; //->
    edge_axes[1] = chunk_front_tr - chunk_front_br; //->
    edge_axes[2] = chunk_front_tl - chunk_front_tr; //->
    edge_axes[3] = chunk_lower - chunk_front_tl; //->


    edge_axes[4] = chunk_back_br - chunk_back_bl; //->
    edge_axes[5] = chunk_upper - chunk_back_br; //->
    edge_axes[6] = chunk_back_tl - chunk_upper; //->
    edge_axes[7] = chunk_back_bl - chunk_back_tl; //->


    edge_axes[8] = chunk_back_bl - chunk_lower; //->
    edge_axes[9] = chunk_back_br - chunk_front_br; //->
    edge_axes[10] = chunk_upper - chunk_front_tr; //->
    edge_axes[11] = chunk_back_bl - chunk_lower; //->

    edge_start[0] = chunk_lower;
    edge_start[1] = chunk_front_br;
    edge_start[2] = chunk_front_tr;
    edge_start[3] = chunk_front_tl;

    edge_start[4] = chunk_back_bl;
    edge_start[5] = chunk_back_br;
    edge_start[6] = chunk_upper;
    edge_start[7] = chunk_back_tl;

    edge_start[8] = chunk_lower;
    edge_start[9] = chunk_front_br;
    edge_start[10] = chunk_front_tr;
    edge_start[11] = chunk_lower;


    edge_end[0] = chunk_front_br;
    edge_end[1] = chunk_front_tr;
    edge_end[2] = chunk_front_tl;
    edge_end[3] = chunk_lower;

    edge_end[4] = chunk_back_br;
    edge_end[5] = chunk_upper;
    edge_end[6] = chunk_back_tl;
    edge_end[7] = chunk_back_bl;

    edge_end[8] = chunk_back_bl;
    edge_end[9] = chunk_back_br;
    edge_end[10] = chunk_upper;
    edge_end[11] = chunk_back_bl;

}


void rectoid(vec3 edge_start, vec3 edge_end, vec3 x, vec3 y)
{
    //start cap
    gl_Position = projection*view*vec4(edge_start-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start+x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(edge_start-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start+x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start-x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    //end cap
    gl_Position = projection*view*vec4(edge_end-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(edge_end-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    //back_face
    gl_Position = projection*view*vec4(edge_start-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(edge_start-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start-x+y, 1.0);
    EmitVertex();
    EndPrimitive();

    //front face

    gl_Position = projection*view*vec4(edge_start+x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x-y, 1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(edge_start+x+y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start+x-y, 1.0);
    EmitVertex();
    EndPrimitive();


    // bottom face

    gl_Position = projection*view*vec4(edge_start+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x-y, 1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(edge_start+x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_end-x-y, 1.0);
    EmitVertex();
    gl_Position = projection*view*vec4(edge_start-x-y, 1.0);
    EmitVertex();
    EndPrimitive();
}



void main()
{
    gl_Position = projection*view*vec4(inData[0].chunk_lower,1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_front_br,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].chunk_lower,1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_front_tl,1);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = projection*view*vec4(inData[0].chunk_lower, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_back_bl,1);
    EmitVertex();
    EndPrimitive();


    gl_Position = projection*view*vec4(inData[0].chunk_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_front_tr,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].chunk_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_back_tl,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].chunk_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].chunk_back_br,1);
    EmitVertex();
    EndPrimitive();

}
