#ifndef BASE_SHAPE_HEADER_INCLUDED
#define BASE_SHAPE_HEADER_INCLUDED
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<mutex>
#include"graphics/shape/PointData.h"
 
class BaseShape :
	public PointData<float>
{
protected:
	GLuint mVBO;
	std::mutex mUpdateMtx;
	bool mBufferOnNextDraw;
	virtual void Buffer();
public:
	BaseShape();
	virtual ~BaseShape();
	virtual void Initialize(GLuint);
	
	virtual void Draw();
	virtual void Scale(float);
	virtual void Shutdown();
};

void BaseShape::Initialize(GLuint vbo)
{
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	mVBO = vbo;
}
void BaseShape::Buffer()
{
	glBufferData(GL_ARRAY_BUFFER, mSize, mDataPtr.get(), GL_STATIC_DRAW);
	mBufferOnNextDraw = false;
}
BaseShape::BaseShape() :
	PointData<float>(),
	mVBO(0),
	mBufferOnNextDraw(true)
{

}

BaseShape::~BaseShape()
{

}

void BaseShape::Scale(float pct)
{

	std::lock_guard<std::mutex> locker(mUpdateMtx);
	float scaler = 1.0f + pct;
	for (unsigned int i = 0; i < mLength; ++i)
	{
		mDataPtr.get()[i] *= scaler;
	}
	mBufferOnNextDraw = true;
}

void BaseShape::Draw()
{

	std::lock_guard<std::mutex> locker(mUpdateMtx);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	if (mBufferOnNextDraw)
	{
		Buffer();
	}
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BaseShape::Shutdown()
{
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	mDataPtr = nullptr;
	mLength = 0;
	mSize = 0;
}

#endif