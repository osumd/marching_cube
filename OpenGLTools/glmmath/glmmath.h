#ifndef glm_math_ext
#define glm_math_ext 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void rotateFrame(glm::vec3& self, const glm::vec3 Normal, glm::vec3 Binormal, float theta)
{
    self.x = (cos(theta)*Normal.x) + (sin(theta)*Binormal.x);
    self.y = (cos(theta)*Normal.y) + (sin(theta)*Binormal.y);
    self.z = (cos(theta)*Normal.z) + (sin(theta)*Binormal.z);
}

#endif