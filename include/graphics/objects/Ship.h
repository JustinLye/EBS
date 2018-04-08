#ifndef SHIP_HEADER_INCLUDED	
#define SHIP_HEADER_INCLUDED
#include<glm/glm.hpp>
#include<algorithm>
#include"graphics/geometry/RenderableObject.h"

class Ship :
	public RenderableObject
{

public:
	Ship();
	~Ship();
	int GetTotalVertices();
	int GetTotalIndices();
	virtual GLenum GetPrimitiveType();
	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
	glm::vec3 mTranslate;
	glm::vec3 mCurrentPosition;
	glm::vec3 mTargetPosition;
	glm::vec3 mMidPoint;
	float mLeftMomentum;
	float mRightMomentum;
	float mMaxSpeed;
	float mMomentum;
	enum DIRECTION : unsigned int
	{
		LEFT,
		RIGHT
	};
	void Strafe(const DIRECTION&);
	void Update(const float&);
protected:
	
};

Ship::Ship() :
	RenderableObject(),
	mTranslate(glm::vec3(0.0f)),
	mTargetPosition(glm::vec3(0.0f)),
	mCurrentPosition(glm::vec3(0.0f)),
	mMidPoint(glm::vec3(0.0f)),
	mLeftMomentum(0.0f),
	mRightMomentum(0.0f),
	mMaxSpeed(0.50f),
	mMomentum(0.0f)
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

int Ship::GetTotalVertices()
{
	return 3;
}

int Ship::GetTotalIndices()
{
	return 3;
}

GLenum Ship::GetPrimitiveType()
{
	return GL_TRIANGLES;
}

void Ship::FillVertexBuffer(GLfloat* buffer)
{
	glm::vec3* verts = (glm::vec3*)buffer;
	verts[0] = glm::vec3(-0.08, -0.90, 0.0);
	verts[1] = glm::vec3(0.0, -0.75, 0.0f);
	verts[2] = glm::vec3(0.08, -0.90, 0.0f);

}

void Ship::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
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