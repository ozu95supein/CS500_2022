#include "RayCasting.h"
#include "SphereObject.h"
#include "BoxObject.h"
#include "LightObject.h"
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
glm::vec3 CastRayRecursiveBounce(SceneStruct scene, Ray r, int RemainingBounces, bool use_default_amb)
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
    std::vector<BoxObject>::iterator nearest_box_obj_it;
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
        if (current_sphere_t < 0)
        {
            continue;
        }
        //check if t is smaller
        if (current_sphere_t < smallest_sphere_t_sofar)
        {
            smallest_sphere_t_sofar = current_sphere_t;
            nearest_sphere_obj_it = it;
        }
    }

    //iterate of the box in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    for (std::vector<BoxObject>::iterator it = scene.mSceneBoxes.begin(); it != scene.mSceneBoxes.end(); ++it)
    {
        current_box_t = it->IntersectWithRay(r);
        if (current_box_t <= 0)
        {
            continue;
        }
        //check if t is smaller
        if (current_box_t < smallest_box_t_sofar)
        {
            smallest_box_t_sofar = current_box_t;
            nearest_box_obj_it = it;
        }
    }
    
    //iterate of the light in the scene, get current t, compare with smallest t, keep trak of nearest object and there t
    for (std::vector<LightObject>::iterator it = scene.mSceneLights.begin(); it != scene.mSceneLights.end(); ++it)
    {
        current_light_t = it->IntersectWithRay(r);
        if (current_light_t < 0)
        {
            continue;
        }
        //check if t is smaller
        if (current_light_t < smallest_light_t_sofar)
        {
            smallest_light_t_sofar = current_light_t;
            nearest_light_obj_it = it;
        }
    }

    RAY_HIT_TYPE e_hit_type = RAY_HIT_TYPE::E_NO_HIT;

    //Sanity check for checking if absolutely nothing intersected with ray
    if ((smallest_sphere_t_sofar < 0.0f) && (smallest_box_t_sofar < 0.0f) && (smallest_light_t_sofar < 0.0f))
    {
        //NO INTERSECTION, return ambient lighting, which is usually set to 0,0,0
        e_hit_type = RAY_HIT_TYPE::E_NO_HIT;
        if (use_default_amb)
        {
            return glm::vec3(0.2, 0.2, 0.2);
        }
        return scene.mSceneAmbient;
    }
    //need to check whic one is the closest object, need to discard negative t values
    //first check Spheres and Boxes
    //check if both are negative, if yes, then check light t directly
    if ((current_sphere_t <= 0.0f) && (current_box_t <= 0.0f))
    {
        if (current_light_t < 0.0f)
        {
            e_hit_type = RAY_HIT_TYPE::E_NO_HIT;
            if (use_default_amb)
            {
                return glm::vec3(0.2, 0.2, 0.2);
            }
            return scene.mSceneAmbient;
        }
        else //we hit a light
        {
            e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
        }
    }
    //check if both are positive
    else if ((current_sphere_t > 0.0f) && (current_box_t > 0.0f))
    {
        if (current_sphere_t < current_box_t)
        {
            //sphere is closer, now we check with light
            // if light is negative, we dont need to check
            if (current_light_t <= 0.0f)
            {
                e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
            }
            else if (current_sphere_t < current_box_t)
            {
                e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
            }
            else
            {
                e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
            }
        }
        else
        {
            //box is closer, now we check with light
            // if light is negative, we dont need to check
            if (current_light_t <= 0.0f)
            {
                e_hit_type = RAY_HIT_TYPE::E_BOX_HIT;
            }
            else if (current_sphere_t < current_box_t)
            {
                if (current_box_t == INFINITY)
                {
                    e_hit_type = RAY_HIT_TYPE::E_NO_HIT;
                }
                e_hit_type = RAY_HIT_TYPE::E_BOX_HIT;
            }
            else
            {
                e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
            }
        }
    }
    //one of them is negative, we take the positive one
    else
    {
        if (current_sphere_t > 0)
        {
            //sphere is closer, now we check with light
            // if light is negative, we dont need to check
            if (current_light_t <= 0.0f)
            {
                e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
            }
            else if (current_sphere_t < current_box_t)
            {
                e_hit_type = RAY_HIT_TYPE::E_SPHERE_HIT;
            }
            else
            {
                e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
            }
        }
        else
        {
            //box is closer, now we check with light
            // if light is negative, we dont need to check
            if (current_light_t <= 0.0f)
            {
                e_hit_type = RAY_HIT_TYPE::E_BOX_HIT;
            }
            else if (current_sphere_t < current_box_t)
            {
                e_hit_type = RAY_HIT_TYPE::E_BOX_HIT;
            }
            else
            {
                e_hit_type = RAY_HIT_TYPE::E_LIGHT_HIT;
            }
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
            if (use_default_amb)
            {
                return glm::vec3(0.2, 0.2, 0.2);
            }
            return scene.mSceneAmbient;
        }
    } 
    else //if we still have bounces
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
                glm::vec3 dirtolight = glm::vec3(0.0, 2.0, 0.0) - pi;
                
                sphere_normal = glm::normalize(sphere_normal);
                //make offseted point
                glm::vec3 p_offset = pi + MY_EPSILON * sphere_normal;
                glm::vec3 rand_dir = myRand_vec3(); 
                glm::vec3 dir = rand_dir + sphere_normal;
                dir = glm::normalize(dir);
                Ray nr(p_offset, dir);

                int b = RemainingBounces - 1;
                return nearest_sphere_obj_it->GetMaterialDiffuse() * CastRayRecursiveBounce(scene, nr, b, use_default_amb);
            }
            else if (e_hit_type == RAY_HIT_TYPE::E_BOX_HIT)
            {
                //generate a new random ray, nr, with the point of intersection, the normal, and call this function again recursively like this:
                //get point of intersection, NOTE WE NEED TO OFFSET THIS
                glm::vec3 pi = r.RayOrigin_p + (r.direction_v * smallest_box_t_sofar);
                glm::vec3 box_normal =  nearest_box_obj_it->GetNormalOfIntersection(pi, MY_EPSILON);
                //make offseted point
                glm::vec3 p_offset = pi + MY_EPSILON * box_normal;
                glm::vec3 dir = myRand_vec3();
                Ray nr(p_offset, dir);
                int b = RemainingBounces - 1;
                return nearest_box_obj_it->GetMaterialDiffuse() * CastRayRecursiveBounce(scene, nr, b, use_default_amb);
            }
        }
        
    }
    if (use_default_amb)
    {
        return glm::vec3(0.2, 0.2, 0.2);
    }
    return scene.mSceneAmbient;
}