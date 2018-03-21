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
	std::mutex mUpdateMtx;
public:
	BaseShape();
	virtual ~BaseShape();
	virtual void Initialize();
	virtual void Draw();
	virtual void Scale(float);
	virtual void Shutdown();
};

void BaseShape::Buffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mSize, mDataPtr.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void BaseShape::Scale(float pct)
{
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	float scaler = 1.0f + pct;
	for (unsigned int i = 0; i < mLength; ++i)
	{
		mDataPtr.get()[i] *= scaler;
	}
}

void BaseShape::Draw()
{
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void BaseShape::Shutdown()
{
	mDataPtr = nullptr;
	mLength = 0;
	mSize = 0;
}

#endif