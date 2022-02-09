#pragma once
#include<glm/glm.hpp>
#include "ObjectCommonHeader.h"
#include <vector>
#include <algorithm>    // std::max
struct Ray;
struct HalfSpace
{
	HalfSpace(glm::vec3 n, glm::vec3 p)
	{
		normal = n;
		point = p;
		float max_f = (float)INFINITY;
		HalfSpaceInterval = glm::vec2(0.0f, (float)max_f);	//by default it is this value
	}
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec2 HalfSpaceInterval;
};

class BoxObject
{
public:
	BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff);
	~BoxObject();
	glm::vec3 GetCenter();
	glm::vec3 GetCorner();
	glm::vec3 GetLength();
	glm::vec3 GetWidth();
	glm::vec3 GetHeight();
	float GetScale();
	glm::vec3 GetMaterialDiffuse();
	float IntersectWithRay(Ray r);
	glm::vec3 GetNormalOfIntersection(glm::vec3 pi, float epsilon_range);
private:
	glm::vec3 mCornerPosition;
	glm::vec3 mLegth;
	glm::vec3 mWidth;
	glm::vec3 mHeight;
	glm::vec3 mMaterialDiffuse;
	std::vector<HalfSpace> BoxPlanes;
};