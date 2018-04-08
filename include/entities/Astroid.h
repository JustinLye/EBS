#ifndef ASTROID_HEADER_INCLUDED
#define ASTROID_HEADER_INCLUDED
#include <glm/glm.hpp>
#include "graphics/objects/Collider.h"
#include "graphics/geometry/Sphere.h"



class Astroid :
	public RenderableObject
{
public:
	static const float DEFAULT_SPEED;
	static const glm::vec4 DEFAULT_COLOR;

	Astroid(const float& = DEFAULT_SPEED);
	~Astroid();
	int GetTotalVertices();
	int GetTotalIndices();
	GLenum GetPrimitiveType();
	void FillVertexBuffer(GLfloat*);
	void FillIndexBuffer(GLuint*);
	void SetCustomUniforms() override;
	void Update(const float&);
	void Spawn(const glm::vec3&);
	void Speed(const float&);
	const float& Speed() const;
	void Color(const glm::vec4&);
	glm::vec4 Color() const;
	glm::vec3 mTranslate;
	Collider mCollider;
#ifdef COLLISION_DEBUG
	class DetectionSphere :
		public Sphere
	{
	public:
		glm::vec3 GetOrigin();
		float GetRadius();
		void InitForRendering();
		glm::vec3 mOrigin;
		float mRadius;
	};
	DetectionSphere* mDetectionSphere;
#endif
protected:
	float mSpeed;
	glm::vec4 mColor;
};

const float Astroid::DEFAULT_SPEED = 0.40f;
const glm::vec4 Astroid::DEFAULT_COLOR = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

Astroid::Astroid(const float& speed) :
	RenderableObject(),
	mSpeed(speed),
	mColor(DEFAULT_COLOR)
{
#ifdef COLLISION_DEBUG
	mDetectionSphere = new DetectionSphere();
#endif
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

void Astroid::SetCustomUniforms()
{
	glUniform4fv(mShader("Color"), 1, glm::value_ptr(mColor));
}

void Astroid::Spawn(const glm::vec3& pos)
{
	mTranslate.x = pos.x;
	mTranslate.y = pos.y;
	mCollider.Center(glm::vec3(0.0f, 1.15f, 0.0f));
	mCollider.Radius(0.07f);
	mCollider.mTranslate.x = pos.x;
	mCollider.mTranslate.y = pos.y;
#ifdef COLLISION_DEBUG
	mDetectionSphere->mOrigin = glm::vec3(0.0f, 1.15f, 0.0f);
	mDetectionSphere->mRadius = 0.07f;
	mDetectionSphere->InitForRendering();
#endif
}

void Astroid::Update(const float& dt)
{
	mTranslate.y -= dt * mSpeed;
	mCollider.mTranslate.y -= dt*mSpeed;
}

void Astroid::Speed(const float& speed)
{
	mSpeed = speed;
}

const float& Astroid::Speed() const
{
	return mSpeed;
}

void Astroid::Color(const glm::vec4& color)
{
	mColor = color;
}

glm::vec4 Astroid::Color() const
{
	return mColor;
}
#ifdef COLLISION_DEBUG
glm::vec3 Astroid::DetectionSphere::GetOrigin()
{
	return mOrigin;
}

float Astroid::DetectionSphere::GetRadius()
{
	return mRadius;
}

void Astroid::DetectionSphere::InitForRendering()
{
	Init();
	mShader.LoadFromFile(ShaderProgram::VERTEX, "C:\\EBS\\OpenGL\\shaders\\sphere.vert");
	mShader.LoadFromFile(ShaderProgram::FRAGMENT, "C:\\EBS\\OpenGL\\shaders\\sphere.frag");
	mShader.CreateAndLink();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\sphere.vert");
	mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\sphere.frag");
	mShader.UnUse();
}
#endif
#endif