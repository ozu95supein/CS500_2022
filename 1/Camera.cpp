#include "Camera.h"
Camera::Camera()
{
	mCPosition = glm::vec3(0,0,0);
	mTPosition = glm::vec3(0, 0, 0);
	mUpVector = glm::vec3(0, 0, 0);
	mFocalLength = 0.0f;
}
Camera::Camera(glm::vec3 c, glm::vec3 T, glm::vec3 U, float f) : mCPosition{ c }, mTPosition{ T }, mUpVector{ U }, mFocalLength{ f }
{
	mCPosition = c;
	mTPosition = T;
	mUpVector = U;
	mFocalLength = f;
}
Camera::~Camera()
{

}
glm::vec3 Camera::GetCPosition()
{
	return mCPosition;
}
glm::vec3 Camera::GetTPosition()
{
	return mTPosition;
}
glm::vec3 Camera::GetUpVector()
{
	return mUpVector;
}
float Camera::GetFocalLength()
{
	return mFocalLength;
}
void Camera::SetCPosition(glm::vec3 c) 
{
	mCPosition = c;
}
void Camera::SetTPosition(glm::vec3 t)
{
	mTPosition = t;
}
void Camera::SetUpVector(glm::vec3 u)
{
	mUpVector = u;
}
void Camera::SetFocalLength(float f)
{
	mFocalLength = f;
}