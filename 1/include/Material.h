#pragma once
#include <glm/glm.hpp>
enum class MaterialType
{
	E_DIFFUSE_MAT,
	E_METALLIC_MAT
};
struct Material
{
	glm::vec3 mColor;
	float mReflecttionRoughness;
	MaterialType mMatType;
	Material()
	{
		mColor = glm::vec3(0.0f);
		mReflecttionRoughness = 0.0f;
		mMatType = MaterialType::E_DIFFUSE_MAT;
	}
	Material(glm::vec3 color, float rough, MaterialType type)
	{
		mColor = color;
		mReflecttionRoughness = rough;
		mMatType = type;
	}
	glm::vec3 GetColor()
	{
		return mColor;
	}
	glm::vec3 GetDiffuse()
	{
		return mColor;
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