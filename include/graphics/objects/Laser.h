#ifndef LASER_HEADER_INCLUDED
#define LASER_HEADER_INCLUDED

#include<glm/glm.hpp>
#include<algorithm>
#include"graphics/geometry/RenderableObject.h"

class Laser :
	public RenderableObject
{
public:
	Laser();
	~Laser();
	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();

	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);

	void Update(const float&);

	glm::vec3 mTranslate;
};

Laser::Laser() :
	RenderableObject(),
	mTranslate(glm::vec3(0.0f))
{
	Init();
	mShader.LoadFromFile(ShaderProgram::VERTEX, "C:\\EBS\\OpenGL\\shaders\\laser.vert");
	mShader.LoadFromFile(ShaderProgram::FRAGMENT, "C:\\EBS\\OpenGL\\shaders\\laser.frag");
	mShader.CreateAndLink();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\laser.vert");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\laser.frag");
	mShader.UnUse();
}

Laser::~Laser()
{

}

int Laser::GetTotalVertices()
{
	return 2;
}

int Laser::GetTotalIndices()
{
	return 2;
}

GLenum Laser::GetPrimitiveType()
{
	return GL_LINES;
}

void Laser::FillVertexBuffer(GLfloat* buffer)
{
	glm::vec3* verts = (glm::vec3*)buffer;
	verts[0] = glm::vec3(0.0f, -1.0f, 0.0f);
	verts[1] = glm::vec3(0.0f, -0.85f, 0.0f);
}

void Laser::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	*id++ = 0;
	*id++ = 1;
}

void Laser::Update(const float& dt)
{
	mTranslate.y += (2.0f * dt);
}


#endif