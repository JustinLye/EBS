#ifndef SPHERE_HEADER_INCLUDED
#define SPHERE_HEADER_INCLUDED

#include<glm/glm.hpp>
#include"graphics/geometry/RenderableObject.h"

class Sphere :
	public RenderableObject
{
public:
	Sphere();
	virtual ~Sphere();
	virtual glm::vec3 GetOrigin() = 0;
	virtual float GetRadius() = 0;
	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();

	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
protected:

};

Sphere::Sphere() :
	RenderableObject()
{

}

Sphere::~Sphere()
{

}

int Sphere::GetTotalVertices()
{
	return 36;
}

int Sphere::GetTotalIndices()
{
	return 37;
}

GLenum Sphere::GetPrimitiveType()
{
	return GL_LINE_STRIP;
}

void Sphere::FillVertexBuffer(GLfloat* buffer)
{
	float r = GetRadius();
	glm::vec3 origin = GetOrigin();
	glm::vec3* verts = (glm::vec3*)buffer;
	for (int i = 0; i < 36; ++i)
	{
		float rads = glm::radians((float)(i+1)*10.0f);
		verts[i].x = origin.x + r * glm::cos(rads);
		verts[i].y = origin.y + r * glm::sin(rads);
		verts[i].z = 0.0f;
	}
}

void Sphere::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	for (int i = 0; i < 36; ++i)
	{
		*id++ = i;
	}
	*id = 0;
}

#endif