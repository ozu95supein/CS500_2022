#include "RayCasting.h"
#include "SphereObject.h"
#include "BoxObject.h"
#include "LightObject.h"
//returns a color in vec3 form with ranges from 0.0 to 1.0
//OLD RAYCAST FUNCTION
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
//returns a random float form 0 to 1
float myRand_0_to_1()
{
    float r = rand() / RAND_MAX;
    return r;
}
float myRand_neg1_to1()
{
    float r = myRand_0_to_1();
    r = 2.0f * r;
    r -= -1.0f;
    return r;
}
glm::vec3 myRand_vec3()
{
    float theta = myRand_0_to_1() * 2.0f * PI;
    float phi = acos(myRand_neg1_to1());
    float x = sin(phi) * cos(theta);
    float y = cos(phi) * sin(theta);
    float z = cos(phi);
    glm::vec3 v = glm::vec3(x, y, z);
    return v;
}
glm::vec3 CastRayRecursiveBounce(SceneStruct scene, Ray r, int RemainingBounces)
{
    //make floats for t values for current t, the value we calculate, and values for smallest 
    //  t sofar, which are the smallest, non-negative, t values we keep track of
    float current_sphere_t = INFINITY;
    float current_box_t = INFINITY;
    float current_light_t = INFINITY;
    float smallest_sphere_t_sofar = INFINITY;
    float smallest_box_t_sofar = INFINITY;
    float smallest_light_t_sofar = INFINITY;

    std::vector<SphereObject>::iterator nearest_sphere_obj_it;
    std::vector<SphereObject>::iterator nearest_box_obj_it;
    std::vector<LightObject>::iterator nearest_light_obj_it;

    // integer to track nearest object
    // (-1 == default option, not set)
    // (0 == NO INTERSECT) AKA all t values are -1
    // (1 == Sphere)
    // (2 == Box)
    // (3 == Light)
    int nearest_object = -1;
    //iterate of the spheres in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    for (std::vector<SphereObject>::iterator it = scene.mSceneSpheres.begin(); it != scene.mSceneSpheres.end(); ++it)
    {
        current_sphere_t = it->IntersectWithRay(r);
        //check if t is smaller
        if (current_sphere_t < smallest_sphere_t_sofar)
        {
            smallest_sphere_t_sofar = current_sphere_t;
            nearest_sphere_obj_it = it;
        }
    }

    //iterate of the box in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    //TODO
    
    //iterate of the light in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    for (std::vector<LightObject>::iterator it = scene.mSceneLights.begin(); it != scene.mSceneLights.end(); ++it)
    {
        current_light_t = it->IntersectWithRay(r);
        //check if t is smaller
        if (current_light_t < smallest_light_t_sofar)
        {
            smallest_light_t_sofar = current_light_t;
            nearest_light_obj_it = it;
        }
    }


    //Sanity check for checking if absolutely nothing intersected with ray
    if ((smallest_sphere_t_sofar == -1.0f) && (smallest_box_t_sofar == -1.0f) && (smallest_light_t_sofar == -1.0f))
    {
        //NO INTERSECTION, return ambient lighting, which is usually set to 0,0,0
        return scene.mSceneAmbient;
    }
    //need to check whic one is the closest object, need to discard negative t values
    RAY_HIT_TYPE e_hit_type = RAY_HIT_TYPE::E_NO_HIT;
    // 1. Check if all are positive, if yes, we use the lowest one
    if ((smallest_sphere_t_sofar > 0.0f) && (smallest_light_t_sofar > 0.0f))
    {
        if (smallest_sphere_t_sofar < smallest_light_t_sofar)
        {
            // we hit a sphere
            e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
        }
        else
        {
            //we hit a light
            e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
        }
    }
    // 2. CHECK if both are negative, if yes, return ambient
    else if ((smallest_sphere_t_sofar < 0.0f) && (smallest_light_t_sofar < 0.0f))
    {
        //NO INTERSECTION, return ambient lighting, which is usually set to 0,0,0
        return scene.mSceneAmbient;
    }
    // 3. one of them is negative, we take the "Larger" (and positive) value
    else
    {
        if (smallest_sphere_t_sofar > smallest_light_t_sofar)
        {
            // we hit a sphere
            e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
        }
        else
        {
            //we hit a light
            e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
        }
    }

    //if remaining bounces are == 0
    if (RemainingBounces == 0)
    {
        //  if intersecting object is a light we return lightcolor matdiffuse
        if (e_hit_type == RAY_HIT_TYPE::E_LIGHT_HIT)
        {
            return nearest_light_obj_it->GetMaterialDiffuse();
        }
        //  else if intersecting object is not a light we return ambient
        else
        {
            return scene.mSceneAmbient;
        }
    } 
    //else if we still have bounces
    {
        //  if closest object is a light
        if (e_hit_type == RAY_HIT_TYPE::E_LIGHT_HIT)
        {
            return nearest_light_obj_it->GetMaterialDiffuse();
        }
        else   
        {
            if (e_hit_type == RAY_HIT_TYPE::E_SPHERE_HIT)
            {
                //generate a new random ray, nr, with the point of intersection, the normal, and call this function again recursively like this:
                //get point of intersection, NOTE WE NEED TO OFFSET THIS
                glm::vec3 pi = r.RayOrigin_p + (r.direction_v * smallest_sphere_t_sofar);
                //calculate the normal of the sphere
                //  get center of sphere and pi, make a vector and normalize
                glm::vec3 sphere_normal = pi - nearest_sphere_obj_it->GetCenter();
                sphere_normal = glm::normalize(sphere_normal);
                //make offseted point
                glm::vec3 p_offset = pi + MY_EPSILON * sphere_normal;
                glm::vec3 dir = myRand_vec3();
                Ray nr(p_offset, dir);
                int b = RemainingBounces - 1;
                return nearest_sphere_obj_it->GetMaterialDiffuse() * CastRayRecursiveBounce(scene, nr, b);
            }
        }
        
    }
    return scene.mSceneAmbient;
}