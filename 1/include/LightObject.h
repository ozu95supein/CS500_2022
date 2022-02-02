#pragma once
#include<glm/vec3.hpp>
class LightObject
{
public:
	LightObject(glm::vec3 p, glm::vec3 diff, float s);
	~LightObject();
	glm::vec3 GetCenter();
	float GetScale();
	glm::vec3 GetMaterialDiffuse();
private:
	glm::vec3 mPosition;
	float mScale;
	glm::vec3 mMaterialDiffuse;
};