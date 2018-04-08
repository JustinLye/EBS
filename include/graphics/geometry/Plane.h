#ifndef PLANE_HEADER_INCLUDED
#define PLANE_HEADER_INCLUDED

#include<glm/glm.hpp>

class Plane
{
public:
	enum Where : unsigned int
	{
		COPLANAR,
		FRONT,
		BACK
	};
	Plane();
	Plane(const glm::vec3&, const glm::vec3&); ///< Construct plane with normal and point
	~Plane();
	static Plane MakePlane(const glm::vec3&, const glm::vec3&, const glm::vec3&);
	Where Classify(const glm::vec3&);
	float GetDistanceFromPoint(const glm::vec3&);

	glm::vec3 mNormal;
	float mDistance;

	static constexpr float EPSILON = 0.0001f;


};

Plane::Plane() :
mNormal(glm::vec3(0)),
mDistance(0)
{

}

Plane::Plane(const glm::vec3& normal, const glm::vec3& point) :
	mNormal(normal),
	mDistance(-glm::dot(normal, point))
{

}

Plane::~Plane()
{

}

Plane Plane::MakePlane(const glm::vec3& point_1, const glm::vec3& point_2, const glm::vec3& point_3)
{
	glm::vec3 e1 = point_2 - point_1;
	glm::vec3 e2 = point_3 - point_1;
	glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
	return Plane(normal, point_1);
}

Plane::Where Plane::Classify(const glm::vec3& point)
{
	float result = GetDistanceFromPoint(point);
	if (result > EPSILON)
	{
		return FRONT;
	}
	else if (result < EPSILON)
	{
		return BACK;
	}
	else
	{
		return COPLANAR;
	}
}

float Plane::GetDistanceFromPoint(const glm::vec3& point)
{
	return glm::dot(mNormal, point) + mDistance;
}

#endif