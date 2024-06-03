#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 32) out;

in VertexData
{
    vec3 voxel_lower;
    vec3 voxel_upper;
    
    vec3 voxel_front_br;
    vec3 voxel_front_tr;
    vec3 voxel_front_tl;

    vec3 voxel_back_br;
    vec3 voxel_back_bl;
    vec3 voxel_back_tl;

    uint cube_case;

} inData[];


uniform mat4 projection;
uniform mat4 view;

void populate_edge_axes(inout vec3 edge_axes[12], inout vec3 edge_start[12], inout vec3 edge_end[12])
{
    // front left counter clockwise [front face]
    // back left counter clockwise [back face]
    // front left counter clockwise [front back face connectors]

    vec3 voxel_lower = inData[0].voxel_lower;
    vec3 voxel_upper = inData[0].voxel_upper;
    
    vec3 voxel_front_br = inData[0].voxel_front_br;
    vec3 voxel_front_tr = inData[0].voxel_front_tr;
    vec3 voxel_front_tl = inData[0].voxel_front_tl;

    vec3 voxel_back_br = inData[0].voxel_back_br;
    vec3 voxel_back_tl = inData[0].voxel_back_tl;
    vec3 voxel_back_bl = inData[0].voxel_back_bl;

    edge_axes[0] = voxel_front_br - voxel_lower; //->
    edge_axes[1] = voxel_front_tr - voxel_front_br; //->
    edge_axes[2] = voxel_front_tl - voxel_front_tr; //->
    edge_axes[3] = voxel_lower - voxel_front_tl; //->


    edge_axes[4] = voxel_back_br - voxel_back_bl; //->
    edge_axes[5] = voxel_upper - voxel_back_br; //->
    edge_axes[6] = voxel_back_tl - voxel_upper; //->
    edge_axes[7] = voxel_back_bl - voxel_back_tl; //->


    edge_axes[8] = voxel_back_bl - voxel_lower; //->
    edge_axes[9] = voxel_back_br - voxel_front_br; //->
    edge_axes[10] = voxel_upper - voxel_front_tr; //->
    edge_axes[11] = voxel_back_bl - voxel_lower; //->

    edge_start[0] = voxel_lower;
    edge_start[1] = voxel_front_br;
    edge_start[2] = voxel_front_tr;
    edge_start[3] = voxel_front_tl;

    edge_start[4] = voxel_back_bl;
    edge_start[5] = voxel_back_br;
    edge_start[6] = voxel_upper;
    edge_start[7] = voxel_back_tl;

    edge_start[8] = voxel_lower;
    edge_start[9] = voxel_front_br;
    edge_start[10] = voxel_front_tr;
    edge_start[11] = voxel_lower;


    edge_end[0] = voxel_front_br;
    edge_end[1] = voxel_front_tr;
    edge_end[2] = voxel_front_tl;
    edge_end[3] = voxel_lower;

    edge_end[4] = voxel_back_br;
    edge_end[5] = voxel_upper;
    edge_end[6] = voxel_back_tl;
    edge_end[7] = voxel_back_bl;

    edge_end[8] = voxel_back_bl;
    edge_end[9] = voxel_back_br;
    edge_end[10] = voxel_upper;
    edge_end[11] = voxel_back_bl;

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

    if ( inData[0].cube_case == 0 || inData[0].cube_case == 255 )
    {
        
    }

    gl_Position = projection*view*vec4(inData[0].voxel_lower.x+inData[0].cube_case,inData[0].voxel_lower.y,inData[0].voxel_lower.z,1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_front_br,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].voxel_lower,1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_front_tl,1);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = projection*view*vec4(inData[0].voxel_lower, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_back_bl,1);
    EmitVertex();
    EndPrimitive();


    gl_Position = projection*view*vec4(inData[0].voxel_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_front_tr,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].voxel_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_back_tl,1);
    EmitVertex();
    EndPrimitive();

    gl_Position = projection*view*vec4(inData[0].voxel_upper, 1);
    EmitVertex();
    gl_Position = projection*view*vec4(inData[0].voxel_back_br,1);
    EmitVertex();
    EndPrimitive();

}
