#pragma once
#include <vector>
#include "SphereObject.h"
#include "BoxObject.h"
#include "LightObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
//A scene struct to contain objects, camera, and lighting infomation
struct SceneStruct
{
	std::vector<SphereObject> mSceneSpheres;
	std::vector<BoxObject> mSceneBoxes;
	std::vector<LightObject> mSceneLights;

	glm::vec3 mSceneAmbient;
	Camera mSceneCamera;
};