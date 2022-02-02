#include "BoxObject.h"
#include "RayCasting.h"
BoxObject::BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff)
{
	mCornerPosition = c;
	mLegth = l;
	mWidth = w;
	mHeight = h;
	mMaterialDiffuse = mat_diff;
}
BoxObject::~BoxObject()
{

}
glm::vec3 BoxObject::GetCenter()
{
	return glm::vec3(0.0, 0.0, 0.0);
}
glm::vec3 BoxObject::GetCorner()
{
	return mCornerPosition;
}
glm::vec3 BoxObject::GetLength()
{
	return mLegth;
}
glm::vec3 BoxObject::GetWidth()
{
	return mWidth;
}
glm::vec3 BoxObject::GetHeight()
{
	return mHeight;
}
float BoxObject::GetScale()
{
	return 0;
}
glm::vec3 BoxObject::GetMaterialDiffuse()
{
	return glm::vec3(0.0, 0.0, 0.0);
}
float BoxObject::IntersectWithRay(Ray r)
{
	return -1.0f;
}