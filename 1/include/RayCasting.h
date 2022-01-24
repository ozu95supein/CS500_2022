#pragma once
#include "glm/glm.hpp"
#include "Camera.h"
#include <vector>
#include "SphereObject.h"
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
glm::vec3 CastRayToScene(Camera camera, Ray r, std::vector<SphereObject> scene, glm::vec3 sceneAmbient)
{
    //t values to compare which one is the closest
    float current_t = INFINITY;
    float smallest_t = INFINITY;
    //return black by default
    glm::vec3 nearest_sphere_color = glm::vec3(0.0f, 0.0f, 0.0f);
    //iterate through the objects and find the smallest t all the while keeping track of the color
    for (auto it = scene.begin(); it != scene.end(); ++it)
    {
        glm::vec3 CP = (r.RayOrigin_p - it->GetCenter()); //Sphere to Cam vector
        float sphere_radius = it->GetScale();                     //radius of sphere
        //gain discriminant
        auto a = glm::dot(r.direction_v, r.direction_v);
        auto b = 2.0f * (glm::dot(r.direction_v, CP));
        auto c = glm::dot(CP, CP) - (sphere_radius * sphere_radius);
        float discriminant = b * b - (4.0f * a * c);
        if (discriminant < 0)
        {
            //No intersection, onto the next object
            continue;
        }
        else if (discriminant > 0)
        {
            //2 intercetions, take the smaller one (the one witht he subtraction)
            current_t = ((-2.0f * (glm::dot(r.direction_v, CP))) - glm::sqrt(discriminant)) / (2 * (glm::dot(r.direction_v, r.direction_v)));
            //sanity check to ignore negatives
            if (current_t < 0)
            {
                continue;
            }
            //check if t is smaller
            if (current_t < smallest_t)
            {
                smallest_t = current_t;
                nearest_sphere_color = it->GetMaterialDiffuse();
            }
        }
        else if (discriminant == 0)
        {
            current_t = -(glm::dot(r.direction_v, CP)) / (glm::dot(r.direction_v, r.direction_v));
            //sanity check to ignore negatives
            if (current_t < 0)
            {
                continue;
            }
            //check if t is smaller
            if (current_t < smallest_t)
            {
                smallest_t = current_t;
                nearest_sphere_color = it->GetMaterialDiffuse();
            }
        }
    }
    //component multiplication with ambient lightsource
    nearest_sphere_color = nearest_sphere_color * sceneAmbient;
    return nearest_sphere_color;
}
