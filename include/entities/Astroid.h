#ifndef ASTROID_HEADER_INCLUDED
#define ASTROID_HEADER_INCLUDED
#include <glm/glm.hpp>
#include "graphics/geometry/RenderableObject.h"

class Astroid :
	public RenderableObject
{
public:
	static const float DEFAULT_SPEED;
	Astroid(const float& = DEFAULT_SPEED);
	~Astroid();
	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();
	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
	void Update(const float&);
	void Spawn(const glm::vec3&);
	void Speed(const float&);
	const float& Speed() const;
	glm::vec3 mTranslate;
protected:
	float mSpeed;
	
};

const float Astroid::DEFAULT_SPEED = 0.40f;

Astroid::Astroid(const float& speed) :
	RenderableObject(),
	mSpeed(speed)
{
	Init();
	mShader.LoadFromFile(ShaderProgram::VERTEX, "C:\\EBS\\OpenGL\\shaders\\astroid.vert");
	mShader.LoadFromFile(ShaderProgram::FRAGMENT, "C:\\EBS\\OpenGL\\shaders\\astroid.frag");
	mShader.CreateAndLink();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\astroid.vert");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\astroid.frag");
	mShader.UnUse();
}
Astroid::~Astroid()
{

}
int Astroid::GetTotalVertices()
{
	return 4;
}

int Astroid::GetTotalIndices()
{
	return 6;
}

GLenum Astroid::GetPrimitiveType()
{
	return GL_TRIANGLES;
}

void Astroid::FillVertexBuffer(GLfloat* buffer)
{
	glm::vec3* verts = (glm::vec3*)buffer;
	verts[0] = glm::vec3(0.05f, 1.20f, 0.0f);
	verts[1] = glm::vec3(0.05f, 1.10f, 0.0f);
	verts[2] = glm::vec3(-0.05f, 1.20f, 0.0f);
	verts[3] = glm::vec3(-0.05f, 1.10f, 0.0f);
}

void Astroid::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	*id++ = 0;
	*id++ = 1;
	*id++ = 3;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;
}

void Astroid::Spawn(const glm::vec3& pos)
{
	mTranslate.x = pos.x;
	mTranslate.y = pos.y;
}

void Astroid::Update(const float& dt)
{
	mTranslate.y -= dt * mSpeed;
}

void Astroid::Speed(const float& speed)
{
	mSpeed = speed;
}

const float& Astroid::Speed() const
{
	return mSpeed;
}

#endif