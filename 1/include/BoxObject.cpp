#include "BoxObject.h"
#include "RayCasting.h"
BoxObject::BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff)
{
	mCornerPosition = c;
	mLegth = l;
	mWidth = w;
	mHeight = h;
	mMaterialDiffuse = mat_diff;
	//generate Halfspaces
	//FRONT
	glm::vec3 c1 = c;
	glm::vec3 n1 = glm::normalize(glm::cross(l, h));
	BoxPlanes.push_back(HalfSpace(n1, c1));
	//BACK
	glm::vec3 c2 = c + w;
	glm::vec3 n2 = -n1;
	BoxPlanes.push_back(HalfSpace(n2, c2));

	//LEFT
	glm::vec3 c3 = c;
	glm::vec3 n3 = glm::normalize(glm::cross(h, w));
	BoxPlanes.push_back(HalfSpace(n3, c3));

	//RIGHT
	glm::vec3 c4 = c + l;
	glm::vec3 n4 = -n3;
	BoxPlanes.push_back(HalfSpace(n4, c4));

	//BOTTOM
	glm::vec3 c5 = c;
	glm::vec3 n5 = glm::normalize(glm::cross(w, l));
	BoxPlanes.push_back(HalfSpace(n5, c5));

	//TOP
	glm::vec3 c6 = c + h;
	glm::vec3 n6 = -n5;
	BoxPlanes.push_back(HalfSpace(n6, c6));

}

BoxObject::~BoxObject()
{

}
glm::vec3 BoxObject::GetCenter()
{
	return glm::vec3(0.0, 0.0, 0.0);
}
glm::vec3 BoxObject::GetCorner()
{
	return mCornerPosition;
}
glm::vec3 BoxObject::GetLength()
{
	return mLegth;
}
glm::vec3 BoxObject::GetWidth()
{
	return mWidth;
}
glm::vec3 BoxObject::GetHeight()
{
	return mHeight;
}
float BoxObject::GetScale()
{
	return 0;
}
glm::vec3 BoxObject::GetMaterialDiffuse()
{
	return glm::vec3(0.0, 0.0, 0.0);
}
// No intersection interval is [0, -1]
void GetRayPlaneIntersectionInterval(Ray r, HalfSpace plane)
{
	glm::vec2 Interval(0.0f, INFINITY);
	if (glm::dot(r.direction_v, plane.normal) < 0)
	{
		if (glm::dot((r.RayOrigin_p - plane.point), plane.normal) > 0)
		{
			float t = -((glm::dot((r.RayOrigin_p - plane.point), plane.normal)) / (glm::dot(r.direction_v, plane.normal)));
			Interval = glm::vec2(t, INFINITY);
		}
		else
		{
			Interval = glm::vec2(0.0f, INFINITY);
		}
	}
	else if(glm::dot(r.direction_v, plane.normal) < 0)
	{
		if (glm::dot((r.RayOrigin_p - plane.point), plane.normal) < 0)
		{
			float t = -((glm::dot((r.RayOrigin_p - plane.point), plane.normal)) / (glm::dot(r.direction_v, plane.normal)));
			Interval = glm::vec2(0.0f, t);
		}
		else
		{
			Interval = glm::vec2(0, -1);
		}
	}
	else
	{
		Interval = glm::vec2(0, -1);
	}
	//set the interval of the plane to that of the interval
	plane.HalfSpaceInterval = Interval;
}
float BoxObject::IntersectWithRay(Ray r)
{
	//loop through halfspaces and set the intervals
	for (int i = 0; i < BoxPlanes.size(); i++)
	{
		GetRayPlaneIntersectionInterval(r, BoxPlanes[i]);
	}
	glm::vec2 CurrentInterval(0.0f, INFINITY);
	//iterate through and modify Current Interval
	for (int i = 0; i < BoxPlanes.size(); i++)
	{
		//skip over invalid intervals
		if (BoxPlanes[i].HalfSpaceInterval == glm::vec2(0, -1))
		{
			continue;
		}
		//max of the min
		float l = std::max(CurrentInterval.x, BoxPlanes[i].HalfSpaceInterval.x);
		//min of the max
		float r = std::min(CurrentInterval.y, BoxPlanes[i].HalfSpaceInterval.y);
		CurrentInterval.x = l;
		CurrentInterval.y = r;
	}
	//check if the interval is valid
	//in interval [x, y], if y =< x, it is invalid
	if (CurrentInterval.x >= CurrentInterval.y)
	{
		//return default value of -1
		return -1.0f;
	}
	//otherise, we return the x value
	return CurrentInterval.x;
}
glm::vec3 BoxObject::GetNormalOfIntersection(glm::vec3 pi, float epsilon_range)
{
	float current_d;
	//iterate through the halspaces
	for (int i = 0; i < BoxPlanes.size(); i++)
	{
		glm::vec3 vector_sub = pi - BoxPlanes[i].point;
		current_d = glm::dot(BoxPlanes[i].normal, vector_sub);
		//check if distance is within the range
		if (abs(current_d) <= epsilon_range)
		{
			return BoxPlanes[i].normal;
		}
	}
	//default should return 0 normal, but this is wrong, we shoulndt get here
	return BoxPlanes[0].normal;
}
