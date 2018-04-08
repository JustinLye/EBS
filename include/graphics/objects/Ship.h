#ifndef SHIP_HEADER_INCLUDED	
#define SHIP_HEADER_INCLUDED
#include<glm/glm.hpp>
#include<algorithm>
#include"graphics/geometry/Triangle.h"

class Ship :
	public Triangle
{

public:
	Ship();
	~Ship();
	glm::vec3 Vertex0();
	glm::vec3 Vertex1();
	glm::vec3 Vertex2();
	glm::vec3 mTranslate;
	glm::vec3 mTargetPosition;
	glm::vec3 mLeftWing;
	glm::vec3 mRightWing;
	glm::vec3 mNose;
	glm::vec3 LeftSide() const;
	glm::vec3 RightSide() const;
	glm::vec3 Nose() const;
	std::pair<glm::vec3, float> SphereOfDetection() const;
	float mMaxSpeed;
	enum DIRECTION : unsigned int
	{
		LEFT,
		RIGHT
	};
	void Strafe(const DIRECTION&);
	void Update(const float&);
protected:
	float mAvgYPos;
	float mRadius;
};

Ship::Ship() :
	Triangle(),
	mTranslate(glm::vec3(0.0f)),
	mTargetPosition(glm::vec3(0.0f)),
	mLeftWing(glm::vec3(-0.08f, -0.90f, 0.0f)),
	mRightWing(glm::vec3(0.08f, -0.90f, 0.0f)),
	mNose(glm::vec3(0.0f, -0.75f, 0.0f)),
	mMaxSpeed(0.80f),
	mAvgYPos(0.85f),
	mRadius(0.085f)
{
	Init();
	mShader.LoadFromFile(ShaderProgram::VERTEX, "C:\\EBS\\OpenGL\\shaders\\ship.vert");
	mShader.LoadFromFile(ShaderProgram::FRAGMENT, "C:\\EBS\\OpenGL\\shaders\\ship.frag");
	mShader.CreateAndLink();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\ship.vert");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\ship.frag");
	mShader.UnUse();
}

Ship::~Ship()
{

}


glm::vec3 Ship::Vertex0()
{
	return glm::vec3(-0.08f, -0.90f, 0.0f);
}

glm::vec3 Ship::Vertex1()
{
	return glm::vec3(0.0f, -0.75f, 0.0f);
}

glm::vec3 Ship::Vertex2()
{
	return glm::vec3(0.08f, -0.90f, 0.0f);
}


void Ship::Update(const float& dt)
{
	float distance = mTargetPosition.x - mTranslate.x;

	if (std::abs(distance) > 0.01f)
	{
		mTranslate.x += distance*dt*mMaxSpeed;
		if (mTranslate.x < -1.0f)
		{
			mTranslate.x = -1.0f;
		}
		else if (mTranslate.x > 1.0f)
		{
			mTranslate.x = 1.0f;
		}
	}
}

glm::vec3 Ship::LeftSide() const
{
	return glm::vec3(mLeftWing.x + mTranslate.x, -0.90f, 0.0f);
}

glm::vec3 Ship::RightSide() const
{
	return glm::vec3(mRightWing.x + mTranslate.x, -0.90f, 0.0f);
}

glm::vec3 Ship::Nose() const
{
	return glm::vec3(mNose.x + mTranslate.x, -0.75f, 0.0f);
}

std::pair<glm::vec3, float> Ship::SphereOfDetection() const
{
	float x_pos = ((mLeftWing.x + mTranslate.x + mRightWing.x + mTranslate.x + mNose.x + mTranslate.x) / 3.0f);
	return { glm::vec3(x_pos, mAvgYPos, 0.0f), mRadius };
	
}

void Ship::Strafe(const DIRECTION& direction)
{
	switch (direction)
	{
	case LEFT:
		mTargetPosition.x = mTranslate.x - 1;
		break;
	case RIGHT:
		mTargetPosition.x = mTranslate.x + 1;
		break;
	default:
		throw std::runtime_error("Unknown direction");
		break;
	};
}

#endif