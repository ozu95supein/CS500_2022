#pragma once
#include <glm/glm.hpp>
enum class MaterialType
{
	E_DIFFUSE_MAT,
	E_METALLIC_MAT
};
struct Material
{
	glm::vec3 mDiffColor;
	float mReflecttionRoughness;
	MaterialType mMatType;
	Material()
	{
		mDiffColor = glm::vec3(0.0f);
		mReflecttionRoughness = 0.0f;
		mMatType = MaterialType::E_DIFFUSE_MAT;
	}
	Material(glm::vec3 diff, float rough, MaterialType type)
	{
		mDiffColor = diff;
		mReflecttionRoughness = rough;
		mMatType = type;
	}
	glm::vec3 GetDiffuse()
	{
		return mDiffColor;
	}
	float GetRoughness()
	{
		return mReflecttionRoughness;
	}
	MaterialType GetType()
	{
		return mMatType;
	}
};