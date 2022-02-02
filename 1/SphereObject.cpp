#include "SphereObject.h"
#include "RayCasting.h"
SphereObject::SphereObject(glm::vec3 p, glm::vec3 diff, float s) : mPosition{ p }, mScale{ s }, mMaterialDiffuse{ diff }
{
	mPosition = p;
	mScale = s;
	mMaterialDiffuse = diff;
}
SphereObject::~SphereObject()
{

}
glm::vec3 SphereObject::GetCenter()
{
	return mPosition;
}
float SphereObject::GetScale()
{
	return mScale;
}
glm::vec3 SphereObject::GetMaterialDiffuse()
{
	return mMaterialDiffuse;
}
//this will not check for negatives, this will return t or -1, 
float SphereObject::IntersectWithRay(Ray r)
{
	//t values to compare which one is the closest
	float current_t = INFINITY;
	//return black by default
	glm::vec3 nearest_sphere_color = glm::vec3(0.0f, 0.0f, 0.0f);
	
    glm::vec3 CP = (r.RayOrigin_p - this->GetCenter()); //Sphere to Cam vector
    float sphere_radius = this->GetScale();                     //radius of sphere
    //gain discriminant
    auto a = glm::dot(r.direction_v, r.direction_v);
    auto b = 2.0f * (glm::dot(r.direction_v, CP));
    auto c = glm::dot(CP, CP) - (sphere_radius * sphere_radius);
    float discriminant = b * b - (4.0f * a * c);
    if (discriminant < 0)
    {
        //return a negative number for no intersection
        return -1.0f;
    }
    else if (discriminant > 0)
    {
        //2 intercetions, take the smaller one (the one witht he subtraction)
        current_t = ((-2.0f * (glm::dot(r.direction_v, CP))) - glm::sqrt(discriminant)) / (2 * (glm::dot(r.direction_v, r.direction_v)));  
        return current_t;
    }
    else if (discriminant == 0)
    {
        current_t = -(glm::dot(r.direction_v, CP)) / (glm::dot(r.direction_v, r.direction_v));
        return current_t;
    }
	return -1.0;
}