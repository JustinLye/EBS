#ifndef TRIANGLE_HEADER_INCLUDED
#define TRIANGLE_HEADER_INCLUDED

#include<glm/glm.hpp>
#include"graphics/geometry/RenderableObject.h"

class Triangle :
	public RenderableObject
{
public:
	Triangle();
	virtual ~Triangle();

	virtual glm::vec3 Vertex0() = 0;
	virtual glm::vec3 Vertex1() = 0;
	virtual glm::vec3 Vertex2() = 0;

	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();

	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
	GLenum GetPolygonMode() override;

};

Triangle::Triangle() :
	RenderableObject()
{

}

Triangle::~Triangle()
{

}

int Triangle::GetTotalVertices()
{
	return 3;
}

int Triangle::GetTotalIndices()
{
	return 3;
}

GLenum Triangle::GetPrimitiveType()
{
	return GL_TRIANGLES;
}

void Triangle::FillVertexBuffer(GLfloat* buffer)
{
	glm::vec3* verts = (glm::vec3*)buffer;
	verts[0] = Vertex0();
	verts[1] = Vertex1();
	verts[2] = Vertex2();
}

void Triangle::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
}

GLenum Triangle::GetPolygonMode()
{
	return GL_FILL;
}

#endif