#include "BoxObject.h"

BoxObject::BoxObject(glm::vec3 p, glm::vec3 diff, float s)
{

}
BoxObject::~BoxObject()
{

}
glm::vec3 BoxObject::GetCenter()
{
	return glm::vec3(0.0, 0.0, 0.0);

}
float BoxObject::GetScale()
{
	return 0;
}
glm::vec3 BoxObject::GetMaterialDiffuse()
{
	return glm::vec3(0.0, 0.0, 0.0);
}