#pragma once
#include<glm/vec3.hpp>
class SphereObject
{
public:
	SphereObject(glm::vec3 p, glm::vec3 diff, float s);
	~SphereObject();
private:
	glm::vec3 mPosition;
	float mScale;
	glm::vec3 mMaterialDiffuse;
};