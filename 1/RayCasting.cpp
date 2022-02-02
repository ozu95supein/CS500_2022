#include "RayCasting.h"
#include "SphereObject.h"
#include "BoxObject.h"
#include "LightObject.h"
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
glm::vec3 CastRayRecursiveBounce(SceneStruct scene, Ray r, int RemainingBounces)
{
    //make floats for t values for current t, the value we calculate, and values for smallest 
    //  t sofar, which are the smallest, non-negative, t values we keep track of
    float current_sphere_t, current_box_t, current_light_t, smallest_sphere_t_sofar, smallest_box_t_sofar, smallest_light_t_sofar = INFINITY;
    SphereObject* nearest_sphere_obj = NULL;
    BoxObject* nearest_box_obj = NULL;
    LightObject* nearest_light_obj = NULL;

    //iterate of the spheres in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    
    //iterate of the box in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    //iterate of the light in the scene, get current t, compare with smallest t, keep trak of nearest object and there t

    //out of the 3 objects and t values, pick the nearest, that is our intersecting object
    //once we have the nearest object check the remaining bounces
    //if remaining bounces are == 0
    //  if intersecting object is a light
    //      return lightcolor matdiffuse
    //  else if intersecting object is not a light
    //      return ambient light
    //  else
    //      return ambient light
    //else if we still have bounces
    //  if closest object is a light
    //    return light color matdiffuse
    //  else 
    //      generate a new random ray, nr, with the point of intersection, the normal, and call this function again recursively like this:
    //      return object_color * CastRayRecursiveBounce();
}