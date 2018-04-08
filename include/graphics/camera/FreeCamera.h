#ifndef FREE_CAMERA_HEADER_INCLUDED
#define FREE_CAMERA_HEADER_INCLUDED

#include<glm/gtx/euler_angles.hpp>
#include"graphics/camera/Camera.h"

class FreeCamera :
	public Camera
{
public:
	FreeCamera();
	~FreeCamera();

	const glm::vec3& Translation() const;
	const float& Speed() const;
	bool MovementDecay() const;

	void Update();
	void Walk(const float&);
	void Strafe(const float&);
	void Lift(const float&);
	void Translation(const glm::vec3&);
	void Speed(const float&);
	void MovementDecay(bool);

protected:
	float mSpeed;
	glm::vec3 mTranslation;
	bool mMovementDecay;
};

FreeCamera::FreeCamera() :
	Camera(),
	mSpeed(1.0f),
	mTranslation(glm::vec3(0.0f)),
	mMovementDecay(false)
{

}

FreeCamera::~FreeCamera()
{

}

const glm::vec3& FreeCamera::Translation() const
{
	return mTranslation;
}

const float& FreeCamera::Speed() const
{
	return mSpeed;
}

bool FreeCamera::MovementDecay() const
{
	return mMovementDecay;
}

void FreeCamera::Update()
{
	glm::mat4 rotate_mat = glm::yawPitchRoll(mYaw, mPitch, mRoll);
	mPosition += mTranslation;
	
	if (!mMovementDecay)
	{
		mTranslation = glm::vec3(0.0f);
	}

	mLook = glm::vec3(rotate_mat * glm::vec4(0, 0, 1, 0));
	mUp = glm::vec3(rotate_mat * glm::vec4(0, 1, 0, 0));
	mRight = glm::cross(mLook, mUp);

	glm::vec3 target = mPosition + mLook;
	mView = glm::lookAt(mPosition, target, mUp);
}

void FreeCamera::Walk(const float& delta_time)
{
	mTranslation += (mLook * mSpeed * delta_time);
	Update();
}

void FreeCamera::Strafe(const float& delta_time)
{
	mTranslation += (mRight * mSpeed * delta_time);
	Update();
}

void FreeCamera::Lift(const float& delta_time)
{
	mTranslation += (mUp * mSpeed * delta_time);
	Update();
}

void FreeCamera::Translation(const glm::vec3& translation)
{
	mTranslation = translation;
}

void FreeCamera::Speed(const float& speed)
{
	mSpeed = speed;
}

void FreeCamera::MovementDecay(bool decay)
{
	mMovementDecay = decay;
}

#endif