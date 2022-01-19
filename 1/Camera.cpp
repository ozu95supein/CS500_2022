#include "Camera.h"
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