#pragma once
#include "glm/glm.hpp"
#include "Camera.h"
#include <vector>
#include "SphereObject.h"
#include "Scene.h"
#include <stdlib.h>
#include <random>
#include <iomanip>
#include <math.h>
const float PI = 3.141592f;
const float MY_EPSILON = 0.001f;
enum RAY_HIT_TYPE
{
    E_NO_HIT = 0,
    E_SPHERE_HIT = 1,
    E_BOX_HIT = 2,
    E_LIGHT_HIT = 3
};
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
