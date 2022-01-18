#pragma once
#include<glm/vec3.hpp>

class Camera
{
public:
	Camera(glm::vec3 c, glm::vec3 T, glm::vec3 U, float f) : mCPosition{ c }, mTPosition{ T }, mUpVector{ U }, mFocalLength{f}
	{
		mCPosition = c;
		mTPosition = T;
		mUpVector = U;
		mFocalLength = f;
	}
	~Camera(); 
	glm::vec3 GetCPosition()
	{
		return mCPosition;
	}
	glm::vec3 GetTPosition()
	{
		return mTPosition;
	}
	glm::vec3 GetUpVector()
	{
		return mUpVector;
	}
	float GetFocalLength()
	{
		return mFocalLength;
	}
	float GFocalLength;
private:
	glm::vec3 mCPosition;
	glm::vec3 mTPosition;
	glm::vec3 mUpVector;
	float mFocalLength;
};