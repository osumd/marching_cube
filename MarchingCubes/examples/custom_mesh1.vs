#version 460 core
layout(location = 0) in vec3 uv;

uniform sampler3D density;

uniform vec3 chunk_lower;
uniform vec3 chunk_upper;

out VertexData
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

} outData;

void main()
{

    vec3 chunk_front_br    = vec3(chunk_upper.x, chunk_lower.y, chunk_lower.z);
    vec3 chunk_front_tr    = vec3(chunk_upper.x, chunk_upper.y, chunk_lower.z);
    vec3 chunk_front_tl    = vec3(chunk_lower.x, chunk_upper.y, chunk_lower.z);
    
    vec3 chunk_back_br     = vec3(chunk_upper.x, chunk_lower.y, chunk_upper.z);
    vec3 chunk_back_bl     = vec3(chunk_lower.x, chunk_lower.y, chunk_upper.z);
    vec3 chunk_back_tl     = vec3(chunk_lower.x, chunk_upper.y, chunk_upper.z);

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

    uint b7 = uint( b_br_density >= 0 );
    uint b6 = uint( b_tr_density >= 0 );
    uint b5 = uint( b_tl_density >= 0 );
    uint b4 = uint( b_bl_density >= 0 );
    uint b3 = uint( f_br_density >= 0 );
    uint b2 = uint( f_tr_density >= 0 );
    uint b1 = uint( f_tl_density >= 0 );
    uint b0 = uint( f_bl_density >= 0 );

    //uint cube_case = b7 | b6 << 1 | b5 << 2 | b4 << 3 | b3 << 4 | b2 << 5 | b1 << 6 | b0 << 7;
    //uint cube_case = uint(texture(density, vec3(1,1,1) ).r);
    
    outData.voxel_lower    = voxel_front_bl;
    outData.voxel_upper    = voxel_back_tr;

    outData.voxel_front_br = voxel_front_br;
    outData.voxel_front_tr = voxel_front_tr;

    outData.voxel_front_tl = voxel_front_tl;
    outData.voxel_back_br  = voxel_back_br;
    outData.voxel_back_bl  = voxel_back_bl;
    outData.voxel_back_tl  = voxel_back_tl;

    outData.cube_case = cube_case;

    //output
    gl_Position = vec4(uv, 1.0);

}