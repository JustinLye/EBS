#ifndef RENDERABLE_OBJECT_HEADER_INCLUDED
#define RENDERABLE_OBJECT_HEADER_INCLUDED
#include"graphics/shader/ShaderProgram.h"

class RenderableObject
{
public:
	RenderableObject();
	virtual ~RenderableObject();
	
	void Render(const float*);
	virtual int GetTotalVertices() = 0;
	virtual int GetTotalIndices() = 0;
	virtual GLenum GetPrimitiveType() = 0;

	virtual void FillVertexBuffer(GLfloat*) = 0;
	virtual void FillIndexBuffer(GLuint*) = 0;

	virtual void SetCustomUniforms();

	void Init();
	void Destroy();
protected:
	GLuint mVaoId;
	GLuint mVboVerticesId;
	GLuint mVboIndicesId;
	ShaderProgram mShader;
	GLenum mPrimitiveType;
	int mTotalVertices;
	int mTotalIndices;
};

RenderableObject::RenderableObject() :
	mVaoId(0),
	mVboVerticesId(0),
	mVboIndicesId(0),
	mShader(ShaderProgram()),
	mPrimitiveType(GL_TRIANGLES),
	mTotalVertices(0),
	mTotalIndices(0)
{

}

RenderableObject::~RenderableObject()
{
	Destroy();
}

void RenderableObject::Render(const float* MVP)
{
	mShader.Use();
	glUniformMatrix4fv(mShader("MVP"), 1, GL_FALSE, MVP);
	SetCustomUniforms();
	glBindVertexArray(mVaoId);
	glDrawElements(mPrimitiveType, mTotalIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	mShader.UnUse();
}

void RenderableObject::SetCustomUniforms()
{

}

void RenderableObject::Init()
{
	glGenVertexArrays(1, &mVaoId);
	glGenBuffers(1, &mVboVerticesId);
	glGenBuffers(1, &mVboIndicesId);

	mTotalVertices = GetTotalVertices();
	mTotalIndices = GetTotalIndices();
	mPrimitiveType = GetPrimitiveType();

	glBindVertexArray(mVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, mVboVerticesId);
	glBufferData(GL_ARRAY_BUFFER, mTotalVertices * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
	GLfloat* buffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	FillVertexBuffer(buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(mShader["vVertex"]);
	glVertexAttribPointer(mShader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndicesId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mTotalIndices * sizeof(GLuint), 0, GL_STATIC_DRAW);
	GLuint* index_buffer = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	FillIndexBuffer(index_buffer);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindVertexArray(0);

}

void RenderableObject::Destroy()
{
	mShader.DeleteProgram();
	glDeleteBuffers(1, &mVboVerticesId);
	glDeleteBuffers(1, &mVboIndicesId);
	glDeleteVertexArrays(1, &mVaoId);
}

#endif