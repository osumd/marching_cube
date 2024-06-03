#version 460 core

layout(location = 0) in uint z8_y8_x8_case8;

out VertexData {
    uint z8_y8_x8_null5_edgeFlags3;
};

void main()
{

    int cube_case = int(z8_y8_x8_case8 & 0xFF); 
    int bit0 = (cube_case ) & 1;
    int bit1 = (cube_case >> 1) & 1;
    int bit3 = (cube_case >> 3) & 1;
    int bit4 = (cube_case >> 4) & 1;

    // | b3 b1 b4 - b0 b0 b0 |

    ivec3 build_vert_on_edge = abs( ivec3(bit3,bit1,bit4) - ivec3(bit0,bit0,bit0) );
    uint bits = z8_y8_x8_case8 & 0xFFFFFF00;
    if(build_vert_on_edge.x != 0)
    {
        bits |= 1;
    }
    if(build_vert_on_edge.y != 0)
    {
        bits |= 2;
    }if(build_vert_on_edge.y != 0)
    {
        bits |= 4;
    }

    VertexData.z8_y8_x8_null5_edgeFlags3 = bits;

    gl_Position = vec4(0.0,0.0,0.0,0.0);

}