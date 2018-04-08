#ifndef LASER_HEADER_INCLUDED
#define LASER_HEADER_INCLUDED

#include<glm/glm.hpp>
#include<algorithm>
#include"graphics/objects/Collider.h"
#ifdef COLLISION_DEBUG
#include"graphics/geometry/Sphere.h"
#else
#include"graphics/geometry/RenderableObject.h"
#endif

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
	Collider mCollider;
#ifdef COLLISION_DEBUG
	class DetectionSphere:
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
};

Laser::Laser() :
	RenderableObject(),
	mTranslate(glm::vec3(0.0f))
{
#ifdef COLLISION_DEBUG
	mDetectionSphere = new DetectionSphere();
	mDetectionSphere->mOrigin = glm::vec3(0.0f, -0.92f, 0.0f);
	mDetectionSphere->mRadius = 0.01f;
	mDetectionSphere->InitForRendering();
#endif
	mCollider.Center(glm::vec3(0.0f, -0.92f, 0.0f));
	mCollider.Radius(0.01f);
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
	verts[1] = glm::vec3(0.0f, -0.92f, 0.0f);
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
	mCollider.mTranslate.y += (2.0f * dt);
}


#ifdef COLLISION_DEBUG
glm::vec3 Laser::DetectionSphere::GetOrigin()
{
	return mOrigin;
}

float Laser::DetectionSphere::GetRadius()
{
	return mRadius;
}

void Laser::DetectionSphere::InitForRendering()
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