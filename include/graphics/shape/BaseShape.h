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
	GLuint mAttribArrayIndex;
	virtual void Buffer();
	std::mutex mUpdateMtx;
public:
	BaseShape();
	virtual ~BaseShape();
	virtual void Initialize(GLuint, GLuint = 0);
	virtual void Draw();
	virtual void Scale(float);
	virtual void Shutdown();
};

void BaseShape::Buffer()
{
	std::cout << "Thread: " << std::this_thread::get_id() << ' ' << __FUNCTION__ << '\n';
	glBufferData(GL_ARRAY_BUFFER, mSize, mDataPtr.get(), GL_STATIC_DRAW);	
}


BaseShape::BaseShape() :
	PointData<float>(),
	mVBO(0),
	mAttribArrayIndex(0)
{

}

BaseShape::~BaseShape()
{

}

void BaseShape::Initialize(GLuint VBO, GLuint AttribArrayIndex)
{
	mVBO = VBO;
	Buffer();

	
}

void BaseShape::Scale(float pct)
{
	std::cout << "Thread: " << std::this_thread::get_id() << ' ' << __FUNCTION__ << '\n';
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	float scaler = 1.0f + pct;
	for (unsigned int i = 0; i < mLength; ++i)
	{
		mDataPtr.get()[i] *= scaler;
	}
	Buffer();
}

void BaseShape::Draw()
{
	std::cout << "Thread: " << std::this_thread::get_id() << ' ' << __FUNCTION__ << '\n';
	std::lock_guard<std::mutex> locker(mUpdateMtx);
	std::cout << "[";
	for (int i = 0; i < mLength-1; ++i)
	{
		std::cout << mDataPtr.get()[i] << ',';
	}
	std::cout << mDataPtr.get()[mLength-1] << "]\n";
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BaseShape::Shutdown()
{
	mDataPtr = nullptr;
	mLength = 0;
	mSize = 0;
}

#endif