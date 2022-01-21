#pragma once
#include<glm/vec3.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 c, glm::vec3 T, glm::vec3 U, float f);
	~Camera(); 
	glm::vec3 GetCPosition();
	glm::vec3 GetTPosition();
	glm::vec3 GetUpVector();
	float GetFocalLength();
private:
	glm::vec3 mCPosition;
	glm::vec3 mTPosition;
	glm::vec3 mUpVector;
	float mFocalLength;
};