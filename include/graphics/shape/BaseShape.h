#ifndef BASE_SHAPE_HEADER_INCLUDED
#define BASE_SHAPE_HEADER_INCLUDED
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"graphics/shape/PointData.h"
 
class BaseShape :
	public PointData<float>
{
protected:
	GLuint VAO;
	GLuint VBO;
	virtual void Buffer();
public:
	BaseShape();
	virtual ~BaseShape();
	virtual void Initialize();
	virtual void Draw();
	virtual void Shutdown();
};

void BaseShape::Buffer()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mSize, mDataPtr.get(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

BaseShape::BaseShape() :
	PointData<float>(),
	VAO(0),
	VBO(0)
{

}

BaseShape::~BaseShape()
{

}

void BaseShape::Initialize()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	Buffer();
}

void BaseShape::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void BaseShape::Shutdown()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	mDataPtr = nullptr;
	mLength = 0;
	mSize = 0;
}

#endif