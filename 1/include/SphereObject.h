#pragma once
#include<glm/vec3.hpp>
#include "ObjectCommonHeader.h"
#include "Material.h"
struct Ray;
class SphereObject
{
public:
	SphereObject(glm::vec3 p, glm::vec3 diff, float s);
	~SphereObject();
	glm::vec3 GetCenter();
	float GetScale();
	glm::vec3 GetMaterialDiffuse();
	float IntersectWithRay(Ray r);
	void SetMaterial(Material m);
	Material GetMaterial();
private:
	glm::vec3 mPosition;
	float mScale;
	glm::vec3 mMaterialDiffuse;
	Material mMaterial;
};