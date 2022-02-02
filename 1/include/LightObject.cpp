#include "LightObject.h"
#include "RayCasting.h"
LightObject::LightObject(glm::vec3 p, glm::vec3 diff, float s)
{
	mPosition = p;
	mScale = s;
	mMaterialDiffuse = diff;
}
LightObject::~LightObject()
{

}
glm::vec3 LightObject::GetCenter()
{
	return mPosition;
}
float LightObject::GetScale()
{
	return mScale;
}
glm::vec3 LightObject::GetMaterialDiffuse()
{
	return mMaterialDiffuse;
}
float LightObject::IntersectWithRay(Ray r)
{
	return -1.0f;
}
