#pragma once
#include<glm/vec3.hpp>
class BoxObject
{
public:
	BoxObject(glm::vec3 p, glm::vec3 diff, float s);
	~BoxObject();
	glm::vec3 GetCenter();
	float GetScale();
	glm::vec3 GetMaterialDiffuse();
private:
	glm::vec3 mPosition;
	float mScale;
	glm::vec3 mMaterialDiffuse;
};