#pragma once
#include "glm/glm.hpp"
#include "Camera.h"
#include <vector>
#include "SphereObject.h"
#include "Scene.h"
// A simple ray struct to use for creating rays
struct Ray
{
    Ray(glm::vec3 p, glm::vec3 v)
    {
        RayOrigin_p = p;
        direction_v = v;
    }
    glm::vec3 RayOrigin_p;
    glm::vec3 direction_v;
};

//returns a color in vec3 form with ranges from 0.0 to 1.0
glm::vec3 CastRayToScene(Camera camera, Ray r, std::vector<SphereObject> scene, glm::vec3 sceneAmbient);

glm::vec3 CastRayRecursiveBounce(SceneStruct scene, Ray r, int RemainingBounces);
