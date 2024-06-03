 #ifndef extendedVectos_H
 #define extendedVectos_H 
 
typedef struct
{
    glm::vec3 near_top_left;
    glm::vec3 near_top_right;
    glm::vec3 near_bottom_left;
    glm::vec3 near_bottom_right;

    glm::vec3 far_top_left;
    glm::vec3 far_top_right;
    glm::vec3 far_bottom_left;
    glm::vec3 far_bottom_right;
} cf_8vec;

typedef struct
{
    glm::vec3 top_left;
    glm::vec3 top_right;
    glm::vec3 bottom_left;
    glm::vec3 bottom_right;
} cf_4vec;

#pragma region CF_BVOL

typedef struct
{
    glm::vec3 BottomRightBack;
    glm::vec3 TopLeftFront;
} cf_bvol;


void bvol_center(const cf_bvol& parent, glm::vec3& center)
{
    

    center.x = parent.TopLeftFront.x + ((parent.BottomRightBack.x - parent.TopLeftFront.x)/2);
    center.y = parent.BottomRightBack.y + ((parent.TopLeftFront.y - parent.BottomRightBack.y)/2);
    center.z = parent.TopLeftFront.z + ((parent.BottomRightBack.z - parent.TopLeftFront.z)/2);



}

#pragma endregion



 #endif