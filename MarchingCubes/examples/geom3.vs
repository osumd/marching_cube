#version 460 core
layout(location = 0) in vec3 uv;

uniform sampler3D density;

uniform vec3 chunk_lower;
uniform vec3 chunk_upper;

out VertexData
{
    vec3 chunk_lower;
    vec3 chunk_upper;
    
    vec3 chunk_front_br;
    vec3 chunk_front_tr;
    vec3 chunk_front_tl;

    vec3 chunk_back_br;
    vec3 chunk_back_bl;
    vec3 chunk_back_tl;

    

} outData;

void main()
{

    vec3 chunk_front_br    = vec3(chunk_upper.x, chunk_lower.y, chunk_lower.z);
    vec3 chunk_front_tr    = vec3(chunk_upper.x, chunk_upper.y, chunk_lower.z);
    vec3 chunk_front_tl    = vec3(chunk_lower.x, chunk_upper.y, chunk_lower.z);
    
    vec3 chunk_back_br     = vec3(chunk_upper.x, chunk_lower.y, chunk_upper.z);
    vec3 chunk_back_bl     = vec3(chunk_lower.x, chunk_lower.y, chunk_upper.z);
    vec3 chunk_back_tl     = vec3(chunk_lower.x, chunk_upper.y, chunk_upper.z);

    /* outData.chunk_lower    = chunk_lower;
    outData.chunk_upper    = chunk_upper;
    outData.chunk_front_br = chunk_front_br;
    outData.chunk_front_tr = chunk_front_tr;
    outData.chunk_front_tl = chunk_front_tl;
    outData.chunk_back_br  = chunk_back_br;
    outData.chunk_back_bl  = chunk_back_bl;
    outData.chunk_back_tl  = chunk_back_tl; */
    
    float uvx = uv.x;
    float uvy = uv.y;
    float uvz = uv.z;

    vec3 chunk_x           = normalize( chunk_front_br - chunk_lower );
    vec3 chunk_y           = normalize( chunk_front_tl - chunk_lower );
    vec3 chunk_z           = normalize( chunk_back_bl - chunk_lower );
    
    float dimx = 1.0/2.0;
    float dimy = 1.0/2.0;
    float dimz = 1.0/2.0;

    //positional elements not needed, but the uv is needed for the texture.

    vec3 voxel_front_bl = chunk_lower + (chunk_x * uvx) + (chunk_y * uvy) + (chunk_z * uvz);
    vec3 voxel_front_br = voxel_front_bl + (chunk_x * dimx);
    vec3 voxel_front_tl = voxel_front_bl + (chunk_y * dimy);
    vec3 voxel_front_tr = voxel_front_bl + (chunk_x * dimx) + (chunk_y * dimy);

    vec3 voxel_back_bl = voxel_front_bl + (chunk_z * dimz);
    vec3 voxel_back_br = voxel_back_bl + (chunk_x * dimx);
    vec3 voxel_back_tr = voxel_back_bl + (chunk_x * dimx) + (chunk_y * dimy);
    vec3 voxel_back_tl = voxel_back_bl + (chunk_y * dimy);

    
    float f_bl_density = texture( density, vec3(uvx, uvy, uvz) ).r;
    float f_br_density = texture( density, vec3(uvx+dimx, uvy, uvz) ).r;
    float f_tr_density = texture( density, vec3(uvx+dimx, uvy+dimy, uvz) ).r;
    float f_tl_density = texture( density, vec3(uvx, uvy+dimy, uvz) ).r;

    float b_bl_density = texture( density, vec3(uvx, uvy, uvz+dimz) ).r;
    float b_br_density = texture( density, vec3(uvx+dimx, uvy, uvz+dimz) ).r;
    float b_tr_density = texture( density, vec3(uvx+dimx, uvy+dimy, uvz+dimz) ).r;
    float b_tl_density = texture( density, vec3(uvx, uvy+dimy, uvz+dimz) ).r;

    outData.chunk_lower    = voxel_front_bl;
    outData.chunk_upper    = voxel_back_tr;

    outData.chunk_front_br = voxel_front_br;
    outData.chunk_front_tr = voxel_front_tr;

    outData.chunk_front_tl = voxel_front_tl;
    outData.chunk_back_br  = voxel_back_br;
    outData.chunk_back_bl  = voxel_back_bl;
    outData.chunk_back_tl  = voxel_back_tl;

    //output
    gl_Position = vec4(uv, 1.0);

}