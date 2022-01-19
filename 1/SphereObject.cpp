#include "SphereObject.h"
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