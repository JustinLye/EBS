#ifndef LASER_CANNON_HEADER_INCLUDED
#define LASER_CANNON_HEADER_INCLUDED

#include<queue>
#include<list>
#include<chrono>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"graphics/objects/Laser.h"

class LaserCannon
{
public:
	static const unsigned int DEFAULT_AMMO_CAPACITY;
	static const unsigned int DEFAULT_SHOT_COOLDOWN_MS;
	static const float DEFAULT_TERMINATION_Y_VALUE;
	static const unsigned int DEFAULT_OVERHEAT_TIMEOUT_MS;
	LaserCannon(const unsigned int& = DEFAULT_AMMO_CAPACITY, const unsigned int& = DEFAULT_SHOT_COOLDOWN_MS, const float& = DEFAULT_TERMINATION_Y_VALUE, const unsigned int& = DEFAULT_OVERHEAT_TIMEOUT_MS);
	virtual ~LaserCannon();
	virtual void Update(const float&);
	virtual void Fire(const glm::vec3&);
	unsigned int ShotsRemaining() const;
	const unsigned int& AmmoCapacity() const;
	const unsigned int& ShotCoolDownMs() const;
protected:
	const unsigned int mAmmoCapacity;
	const unsigned int mShotCoolDownMs;
	const float mTerminationYValue;
	const unsigned int mOverHeatTimeOutMs;
	unsigned int mCurrentCoolDown;
	glm::mat4 mIdentityMat;
	glm::mat4 mTranslateMat;
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double,std::milli>> mLastTimeFired;
	Laser** mLaserMag;
	std::queue<unsigned int> mAvailableIndexQueue;
	std::list<unsigned int> mActiveIndexList;
	void InitCannon();
	void FireCannon(const glm::vec3&);
	

};

const unsigned int LaserCannon::DEFAULT_AMMO_CAPACITY = 20;
const unsigned int LaserCannon::DEFAULT_SHOT_COOLDOWN_MS = 128;
const float LaserCannon::DEFAULT_TERMINATION_Y_VALUE = 2.20f;
const unsigned int LaserCannon::DEFAULT_OVERHEAT_TIMEOUT_MS = 500;

LaserCannon::LaserCannon(const unsigned int& ammo_cap, const unsigned int& shot_cooldown_ms, const float& term_y_val, const unsigned int& overheat_timeout_ms) :
	mAmmoCapacity(ammo_cap),
	mShotCoolDownMs(shot_cooldown_ms),
	mTerminationYValue(term_y_val),
	mIdentityMat(glm::mat4(1.0f)),
	mTranslateMat(glm::mat4(1.0f)),
	mOverHeatTimeOutMs(overheat_timeout_ms),
	mCurrentCoolDown(mShotCoolDownMs)
{
	InitCannon();
}

LaserCannon::~LaserCannon()
{
	mLastTimeFired = std::chrono::high_resolution_clock::now() - std::chrono::duration<double, std::milli>(mShotCoolDownMs);
	for (int i = 0; i < mAmmoCapacity; ++i)
	{
		mLaserMag[i]->Destroy();
		delete mLaserMag[i];
	}
}

void LaserCannon::Update(const float& dt)
{
	std::list<unsigned int>::iterator iter = mActiveIndexList.begin();
	while (iter != mActiveIndexList.end())
	{
		Laser* laser = mLaserMag[*iter];
		laser->Update(dt);
		if (laser->mTranslate.y > mTerminationYValue)
		{
			mAvailableIndexQueue.push(*iter);
			iter = mActiveIndexList.erase(iter);
		}
		else
		{
			mTranslateMat = glm::translate(mIdentityMat, laser->mTranslate);
			laser->Render(glm::value_ptr(mTranslateMat));
			iter++;
		}
	}
}

void LaserCannon::Fire(const glm::vec3& pos)
{
	FireCannon(pos);
}

unsigned int LaserCannon::ShotsRemaining() const
{
	return mAvailableIndexQueue.size();
}

const unsigned int& LaserCannon::AmmoCapacity() const
{
	return mAmmoCapacity;
}

const unsigned int& LaserCannon::ShotCoolDownMs() const
{
	return mShotCoolDownMs;
}

void LaserCannon::InitCannon()
{
	mLaserMag = new Laser*[mAmmoCapacity];
	for (int i = 0; i < mAmmoCapacity; ++i)
	{
		mLaserMag[i] = new Laser();
		mAvailableIndexQueue.push(i);
	}
}

void LaserCannon::FireCannon(const glm::vec3& pos)
{
	auto currtime = std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(currtime - mLastTimeFired).count() >= mCurrentCoolDown)
	{
		if (!mAvailableIndexQueue.empty())
		{
			unsigned int i = mAvailableIndexQueue.front();
			mAvailableIndexQueue.pop();
			mActiveIndexList.push_front(i);
			mLaserMag[i]->mTranslate.y = pos.y;
			mLaserMag[i]->mTranslate.x = pos.x;
			mLastTimeFired = std::chrono::high_resolution_clock::now();
			mCurrentCoolDown = mShotCoolDownMs;
		}
		else
		{
			// Penalty for overheating
			mCurrentCoolDown += mOverHeatTimeOutMs;
		}
	}
}


#endif