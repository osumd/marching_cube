#ifndef CAMERA_FRUSTRUM_H
#define CAMERA_FRUSTRUM_H 

#include <glmmath.h>
#include "cubeBuffer.h"
#include "extendedVectors.h"


cf_8vec CreateFrustrum(glm::vec3 camera_pos, glm::vec3 Forward, glm::vec3 up_vector, float hfov, float vfov, float near_dist, float far_dist)
{
    glm::vec3 right = glm::cross(Forward, up_vector);
    right = glm::normalize(right);
    glm::vec3 up = glm::cross(right,Forward);
    up = glm::normalize(up);

    float hhnear = near_dist * tan((hfov*(3.14159265359/180))/2);
    float hhfar = far_dist * tan(((hfov)*(3.14159265359/180))/2);

    glm::vec3 near_center = camera_pos+ glm::normalize(Forward)*near_dist;
    glm::vec3 far_center = camera_pos+ glm::normalize(Forward)*far_dist;
    
    cf_8vec newFrustrum;

    newFrustrum.near_top_left = near_center + up * hhnear - right * hhnear;
    newFrustrum.near_top_right = near_center + up * hhnear + right * hhnear;
    newFrustrum.near_bottom_left = near_center - up * hhnear - right * hhnear;
    newFrustrum.near_bottom_right = near_center - up * hhnear + right * hhnear;

    newFrustrum.far_top_left = far_center + up * hhfar - right * hhfar;
    newFrustrum.far_top_right = far_center + up * hhfar + right * hhfar;
    newFrustrum.far_bottom_left = far_center - up * hhfar - right * hhfar;
    newFrustrum.far_bottom_right = far_center - up * hhfar + right * hhfar;


    return newFrustrum;

}


bool PointInBetweenPlanes(glm::vec3 point, cf_4vec plane1, cf_4vec plane2)
{
    
    glm::vec3 planeDirection = plane2.bottom_left - plane1.bottom_left;
    glm::vec3 planeNormal = glm::cross(plane1.top_left-plane1.bottom_left, plane1.bottom_right-plane1.bottom_left);
    float     dplaneDir   = glm::dot(planeDirection, planeNormal);

    if(dplaneDir < 0)
    {
        planeNormal*=-1;
    }
    
    planeNormal = glm::normalize(planeNormal);

    float dPoint = glm::dot(point,planeNormal);

    if(dPoint > 0)
    {
        return true;
    }

    return false;

}

bool ManualPointInBetweenPlanes(glm::vec3& point, glm::vec3& plane1bottom_left, glm::vec3& plane1bottom_right,  glm::vec3& plane1top_right,  glm::vec3& plane1top_left, glm::vec3& plane2bottom_left, glm::vec3& plane2bottom_right,  glm::vec3& plane2top_right,  glm::vec3& plane2top_left)
{
    
    glm::vec3 planeDirection = plane2bottom_left - plane1bottom_left;
    glm::vec3 planeNormal = glm::cross(plane1top_left-plane1bottom_left, plane1bottom_right-plane1bottom_left);
    float     dplaneDir   = glm::dot(planeDirection, planeNormal);

    if(dplaneDir < 0)
    {
        planeNormal*=-1;
    }
    
    planeNormal = glm::normalize(planeNormal);

    float d1Point = glm::dot(point,planeNormal);


    planeDirection = plane1bottom_left - plane2bottom_left;
    planeNormal = glm::cross(plane2top_left-plane2bottom_left, plane2bottom_right-plane2bottom_left);
    dplaneDir   = glm::dot(planeDirection, planeNormal);

    if(dplaneDir < 0)
    {
        planeNormal*=-1;
    }
    
    planeNormal = glm::normalize(planeNormal);

    float d2Point = glm::dot(point,planeNormal);

    if(d1Point > 0 && d2Point > 0)
    {
        return true;
    }

    return false;

}

//Optionally could compute bounding box of view frustrum and compute simple bounding bounding intersection

//Optionally could compute the frustrum rays, intersect the ray with the box, and then clip the distance.

void incrementVolume(cf_bvol& self, glm::vec3& point)
{

    if(point.x < self.TopLeftFront.x)
    {
        self.TopLeftFront.x = point.x;
    }
    if(point.x > self.BottomRightBack.x)
    {
        self.BottomRightBack.x = point.x;
    }


    if(point.y > self.TopLeftFront.y)
    {
        self.TopLeftFront.y = point.y;
    }
    if(point.y < self.BottomRightBack.y)
    {
        self.BottomRightBack.y = point.y;
    }


    if(point.z > self.TopLeftFront.z)
    {
        self.TopLeftFront.z = point.z;
    }
    if(point.z < self.BottomRightBack.z)
    {
        self.BottomRightBack.z = point.z;
    }


}


bool CubeFrustrumIntersection(cf_8vec& viewFrustrum, cf_8vec& cube)
{
    //what if the boundingFrustrum is flipped upside down???
    cf_bvol boundingFrustrum;
    incrementVolume(boundingFrustrum, cube.far_bottom_left);
    incrementVolume(boundingFrustrum, cube.far_bottom_right);
    incrementVolume(boundingFrustrum, cube.far_top_left);
    incrementVolume(boundingFrustrum, cube.far_top_right);
    incrementVolume(boundingFrustrum, cube.near_bottom_left);
    incrementVolume(boundingFrustrum, cube.near_bottom_right);
    incrementVolume(boundingFrustrum, cube.near_top_left);
    incrementVolume(boundingFrustrum, cube.near_top_right);

    //testing two bounding volumes intersection
    //radius of bounding volume, in range???

    return true;


}

void FrustrumCube(cf_8vec& viewFrustrum, cf_bvol& boundingFrustrum)
{

    boundingFrustrum = {{viewFrustrum.near_bottom_left}, {viewFrustrum.near_bottom_left}};
    incrementVolume(boundingFrustrum, viewFrustrum.far_bottom_left);
    incrementVolume(boundingFrustrum, viewFrustrum.far_bottom_right);
    incrementVolume(boundingFrustrum, viewFrustrum.far_top_left);
    incrementVolume(boundingFrustrum, viewFrustrum.far_top_right);
    incrementVolume(boundingFrustrum, viewFrustrum.near_bottom_left);
    incrementVolume(boundingFrustrum, viewFrustrum.near_bottom_right);
    incrementVolume(boundingFrustrum, viewFrustrum.near_top_left);
    incrementVolume(boundingFrustrum, viewFrustrum.near_top_right);
}

bool FrustrumCubeIntersection(cf_8vec& viewFrustrum, cf_8vec& cube)
{
    //3 * 8 times
    bool a0,a1,a2;
    //Front and Back
    a0 = ManualPointInBetweenPlanes(cube.near_bottom_left, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.near_bottom_left, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.near_bottom_left, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

    a0 = ManualPointInBetweenPlanes(cube.near_bottom_right, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.near_bottom_right, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.near_bottom_right, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

    a0 = ManualPointInBetweenPlanes(cube.near_top_right, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.near_top_right, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.near_top_right, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

     a0 = ManualPointInBetweenPlanes(cube.near_top_left, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.near_top_left, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.near_top_left, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }



    //3 * 8 times
    //Front and Back
    a0 = ManualPointInBetweenPlanes(cube.far_bottom_left, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.far_bottom_left, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.far_bottom_left, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

    a0 = ManualPointInBetweenPlanes(cube.far_bottom_right, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.far_bottom_right, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.far_bottom_right, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

    a0 = ManualPointInBetweenPlanes(cube.far_top_right, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.far_top_right, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.far_top_right, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

     a0 = ManualPointInBetweenPlanes(cube.far_top_left, cube.near_bottom_left, cube.near_bottom_right, cube.near_top_right, cube.near_top_left, cube.far_bottom_left, cube.far_bottom_right, cube.far_top_left, cube.far_top_right);
    //Left  and Right
    a1 = ManualPointInBetweenPlanes(cube.far_top_left, cube.far_bottom_left, cube.near_bottom_left, cube.near_top_left, cube.far_top_left, cube.far_bottom_right, cube.near_bottom_right, cube.near_top_right, cube.far_top_right);
    //Bottom and Top
    a2 = ManualPointInBetweenPlanes(cube.far_top_left, cube.near_bottom_left, cube.near_bottom_right, cube.far_bottom_right, cube.far_bottom_left, cube.near_top_left, cube.near_top_right, cube.far_top_right, cube.far_top_left); 
     //front and back

    if(a0 && a1 && a2)
    {
        return true;
    }

    return false;

}





#endif