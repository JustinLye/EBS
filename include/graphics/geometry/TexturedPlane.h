#ifndef TEXTURED_PLANE_HEADER_INCLUDED
#define TEXTURED_PLANE_HEADER_INCLUDED
#include<glm/glm.hpp>
#include"graphics/geometry/RenderableObject.h"

class TexturedPlane :
	public RenderableObject
{
public:
	TexturedPlane(const std::string&, const std::string&, const int& = 1000, const int& = 1000);
	virtual ~TexturedPlane();
	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();

	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
	
protected:
	virtual void InitShader(const std::string&, const std::string&);
private:
	int mWidth;
	int mDepth;
};


TexturedPlane::TexturedPlane(
	const std::string& vert_shader_path,
	const std::string& frag_shader_path,
	const int& width,
	const int& depth) :
	RenderableObject(),
	mWidth(width),
	mDepth(depth)
{
	InitShader(vert_shader_path, frag_shader_path);
	Init();
}

TexturedPlane::~TexturedPlane()
{

}

int TexturedPlane::GetTotalVertices()
{
	return 128*128;
}
int TexturedPlane::GetTotalIndices()
{
	return 6;
}
GLenum TexturedPlane::GetPrimitiveType()
{
	return GL_TRIANGLES;
}

void TexturedPlane::FillVertexBuffer(GLfloat* buffer)
{
	glm::vec3* vertices = (glm::vec3*)(buffer);
	int half_width = mWidth / 2;
	int half_depth = mDepth / 2;

	vertices[0] = glm::vec3(-half_width, 0, -half_depth);
	vertices[1] = glm::vec3(half_width, 0, -half_depth);
	vertices[2] = glm::vec3(half_width, 0, half_depth);
	vertices[3] = glm::vec3(-half_width, 0, half_depth);
}

void TexturedPlane::FillIndexBuffer(GLuint* buffer)
{
	GLuint* id = buffer;
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;
}

void TexturedPlane::InitShader(const std::string& vert_shader_path, const std::string& frag_shader_path)
{
	mShader.LoadFromFile(ShaderProgram::VERTEX, vert_shader_path);
	mShader.LoadFromFile(ShaderProgram::FRAGMENT, frag_shader_path);
	mShader.CreateAndLink();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AutoFillUniformsFromFile(vert_shader_path);
	mShader.AutoFillUniformsFromFile(frag_shader_path);
	glUniform1i(mShader("texture_map"), 0);
	mShader.UnUse();
}

#endif