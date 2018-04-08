#ifndef CAMERA_HEADER_INCLUDED
#define CAMERA_HEADER_INCLUDED


#include<glm/gtc/matrix_transform.hpp>
#include"graphics/geometry/Plane.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	const glm::mat4& View() const;
	const glm::mat4& Projection() const;
	const glm::vec3& Position() const;
	const float& FOV() const;
	const float& AspectRatio() const;

	void Position(const glm::vec3&);
	void FOV(const float&);
	void Projection(const float&, const float&, const float& = 0.10f, const float& = 1000.0f);

	virtual void Update() = 0;
	virtual void Rotate(const float&, const float&, const float&);

	void CalcFrustumPoints();
	void GetFrustumPlanes(glm::vec4[6]);

	bool IsPointInFrustum(const glm::vec3&);
	bool IsSphereInFrustum(const glm::vec3&, const float&);
	bool IsBoxInFrustum(const glm::vec3&, const glm::vec3&);
	
	glm::vec3 mFarPoints[4];
	glm::vec3 mNearPoints[4];

protected:
	float mYaw;
	float mPitch;
	float mRoll;
	float mFov;
	float mAspectRatio;
	float mZNear;
	float mZFar;
	glm::vec3 mLook;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mPosition;
	glm::mat4 mView;
	glm::mat4 mProjection;

	Plane mPlanes[6]; ///<Frustum planes

	static glm::vec3 UP; ///<World up
};

glm::vec3 Camera::UP = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() :
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFov(0.0f),
	mAspectRatio(0.0f),
	mZNear(0.10f),
	mZFar(1000.0f),
	mLook(glm::vec3(0.0f)),
	mUp(Camera::UP),
	mRight(glm::vec3(0.0f)),
	mPosition(glm::vec3(0.0f)),
	mView(glm::mat4(0.0f)),
	mProjection(glm::mat4(0.0f))
{

}

Camera::~Camera()
{

}

const glm::mat4& Camera::View() const
{
	return mView;
}

const glm::mat4& Camera::Projection() const
{
	return mProjection;
}

const glm::vec3& Camera::Position() const
{
	return mPosition;
}

const float& Camera::FOV() const
{
	return mFov;
}

const float& Camera::AspectRatio() const
{
	return mAspectRatio;
}

void Camera::Position(const glm::vec3& position)
{
	mPosition = position;
}

void Camera::FOV(const float& fov_degrees)
{
	mFov = fov_degrees;
	mProjection = glm::perspective(mFov, mAspectRatio, mZNear, mZFar);
}

void Camera::Projection(const float& fovy, const float& aspect_ratio, const float& z_near, const float& z_far)
{
	mProjection = glm::perspective(fovy, aspect_ratio, z_near, z_far);
	mZNear = z_near;
	mZFar = z_far;
	mFov = fovy;
	mAspectRatio = aspect_ratio;
}

void Camera::Rotate(const float& yaw, const float& pitch, const float& roll)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);
	mRoll = glm::radians(roll);
	Update();
}

void Camera::CalcFrustumPoints()
{
	glm::vec3 near_position = mPosition + mLook*mZNear;
	glm::vec3 far_position = mPosition + mLook*mZFar;

	float tan_dist = 2.0f * tan(glm::radians(mFov / 2.0f));
	float height_near = tan_dist*mZNear;
	float width_near = height_near * mAspectRatio;
	float height_far = tan_dist*mZFar;
	float width_far = height_far * mAspectRatio;
	float half_height_near = height_near / 2.0f;
	float half_width_near = width_near / 2.0f;
	float half_height_far = height_far / 2.0f;
	float half_width_far = width_far / 2.0f;

	mFarPoints[0] = far_position + mUp*half_height_far - mRight*half_width_far;
	mFarPoints[1] = far_position - mUp*half_height_far - mRight*half_width_far;
	mFarPoints[3] = far_position - mUp*half_height_far + mRight*half_width_far;
	mFarPoints[4] = far_position + mUp*half_height_far + mRight*half_width_far;

	mNearPoints[0] = near_position + mUp*half_height_near - mRight*half_width_near;
	mNearPoints[1] = near_position - mUp*half_height_near - mRight*half_width_near;
	mNearPoints[2] = near_position - mUp*half_height_near + mRight*half_width_near;
	mNearPoints[3] = near_position + mUp*half_height_near - mRight*half_width_near;

	mPlanes[0] = Plane::MakePlane(mNearPoints[3], mNearPoints[0], mFarPoints[0]);
	mPlanes[1] = Plane::MakePlane(mNearPoints[1], mNearPoints[2], mFarPoints[2]);
	mPlanes[2] = Plane::MakePlane(mNearPoints[0], mNearPoints[1], mFarPoints[1]);
	mPlanes[3] = Plane::MakePlane(mNearPoints[2], mNearPoints[3], mFarPoints[2]);
	mPlanes[4] = Plane::MakePlane(mNearPoints[0], mNearPoints[3], mNearPoints[2]);
	mPlanes[5] = Plane::MakePlane(mFarPoints[3], mFarPoints[0], mFarPoints[1]);

}

void Camera::GetFrustumPlanes(glm::vec4 planes[6])
{
	for (int i = 0; i < 6; ++i)
	{
		planes[i] = glm::vec4(mPlanes[i].mNormal, mPlanes[i].mDistance);
	}
}

bool Camera::IsPointInFrustum(const glm::vec3& point)
{
	for (int i = 0; i < 6; ++i)
	{
		if (mPlanes[i].GetDistanceFromPoint(point) < 0)
		{
			return false;
		}
	}
	return true;
}

bool Camera::IsSphereInFrustum(const glm::vec3& center, const float& radius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (mPlanes[i].GetDistanceFromPoint(center) < -radius)
		{
			return false;
		}
	}
	return true;
}

bool Camera::IsBoxInFrustum(const glm::vec3& min, const glm::vec3& max)
{
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 point = min;
		mPlanes[i].mNormal.x >= 0 ? point.x = max.x : point.x = min.x;
		mPlanes[i].mNormal.y >= 0 ? point.y = max.y : point.y = min.y;
		mPlanes[i].mNormal.z >= 0 ? point.z = max.z : point.z = min.z;
		if (mPlanes[i].GetDistanceFromPoint(point) < 0)
		{
			return false;
		}
	}
	return true;
}

#endif
