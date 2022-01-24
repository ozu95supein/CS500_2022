#pragma once
#include <vector>
#include "SphereObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
//A scene struct to contain objects, camera, and lighting infomation
struct SceneStruct
{
	std::vector<SphereObject> mSceneSpheres;
	glm::vec3 mSceneAmbient;
	Camera mSceneCamera;
};