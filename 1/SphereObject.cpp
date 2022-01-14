#include "SphereObject.h"
SphereObject::SphereObject(glm::vec3 p, glm::vec3 diff, float s) : mPosition{ p }, mMaterialDiffuse{ diff }, mScale{s}
{
	mPosition = p;
	mMaterialDiffuse = diff;
	mScale = s;
}
SphereObject::~SphereObject()
{

}