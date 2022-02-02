#pragma once
#include<glm/vec3.hpp>
class BoxObject
{
public:
	BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff);
	~BoxObject();
	glm::vec3 GetCenter();
	float GetScale();
	glm::vec3 GetMaterialDiffuse();
	float IntersectWithRay();

private:
	glm::vec3 mCornerPosition;
	glm::vec3 mLegth;
	glm::vec3 mWidth;
	glm::vec3 mHeight;
	glm::vec3 mMaterialDiffuse;
};