#ifndef ASTROID_MANAGER_HEADER_INCLUDED
#define ASTROID_MANAGER_HEADER_INCLUDED

#include<list>
#include<queue>
#include<random>
#include<chrono>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"entities/Astroid.h"


class AstroidManager
{
public:
	static const unsigned int DEFAULT_MAX_ASTROIDS;
	static const float DEFAULT_Y_TERM_POS;
	static const float DEFAULT_STARTING_SPEED;
	static const unsigned int DEFAULT_MAX_ACTIVE_ASTROIDS;
	static const float DEFAULT_MAX_SPEED;
	static const float DEFAULT_MIN_SPAWN_TIME;
	static const float DEFAULT_MAX_SPAWN_TIME;
	static const float DEFAULT_START_MIN_SPAWN_TIME;
	static const float DEFAULT_START_MAX_SPAWN_TIME;
	

	AstroidManager();
	~AstroidManager();

	void Update(const float&);
	void Render();
	void MaxActiveAstroids(const unsigned int&);
	const unsigned int& MaxActiveAstroids() const;

	void StartingSpeed(const float&);
	const float& StartingSpeed() const;
	Astroid** mAstroidArray;
	std::queue<unsigned int> mAvailableIndexQueue;
	std::list<unsigned int> mActiveIndexList;
protected:
	const unsigned int mMaxAstroids;
	const float mTermYPos;
	const float mMaxSpeed;

	float mStartingSpeed;
	unsigned int mMaxActiveAstroids;
	

	std::pair<float, float> mSpawnMinMaxTimes;
	std::random_device mRd;
	std::mt19937 mGen;
	std::uniform_int_distribution<> mSpawnDist;
	std::uniform_real_distribution<> mPosDist;
	std::chrono::time_point<
		std::chrono::high_resolution_clock,
		std::chrono::duration<float, std::milli>> mLastSpawnTime;
	glm::mat4 mTranslateMat;
	glm::mat4 mIdentityMat;
	void Init();
	void TrySpawn();
};

const unsigned int AstroidManager::DEFAULT_MAX_ASTROIDS = 15;
const float AstroidManager::DEFAULT_Y_TERM_POS = -2.20f;
const float AstroidManager::DEFAULT_STARTING_SPEED = 0.26f;
const unsigned int AstroidManager::DEFAULT_MAX_ACTIVE_ASTROIDS = 10;
const float AstroidManager::DEFAULT_MAX_SPEED = 1.0f;
const float AstroidManager::DEFAULT_MIN_SPAWN_TIME = 200.0f;
const float AstroidManager::DEFAULT_MAX_SPAWN_TIME = 2000.0f;
const float AstroidManager::DEFAULT_START_MIN_SPAWN_TIME = 600.0f;
const float AstroidManager::DEFAULT_START_MAX_SPAWN_TIME = 2000.0f;

AstroidManager::AstroidManager() :
	mMaxAstroids(DEFAULT_MAX_ASTROIDS),
	mTermYPos(DEFAULT_Y_TERM_POS),
	mMaxSpeed(DEFAULT_STARTING_SPEED),
	mStartingSpeed(DEFAULT_STARTING_SPEED),
	mMaxActiveAstroids(DEFAULT_MAX_ACTIVE_ASTROIDS),
	mSpawnMinMaxTimes({ DEFAULT_START_MIN_SPAWN_TIME, DEFAULT_START_MAX_SPAWN_TIME }),
	mGen(mRd()),
	mSpawnDist(1, 10),
	mPosDist(-0.8f, 0.8f),
	mTranslateMat(glm::mat4(1.0f)),
	mIdentityMat(glm::mat4(1.0f))
{
	Init();
}
AstroidManager::~AstroidManager()
{
	delete[] mAstroidArray;
}

void AstroidManager::Update(const float& dt)
{
	TrySpawn();
	std::list<unsigned int>::iterator iter = mActiveIndexList.begin();
	while (iter != mActiveIndexList.end())
	{
		mAstroidArray[*iter]->Update(dt);
		if (mAstroidArray[*iter]->mTranslate.y <= mTermYPos)
		{
			mAvailableIndexQueue.push(*iter);
			iter = mActiveIndexList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void AstroidManager::Render()
{
	std::list<unsigned int>::iterator iter = mActiveIndexList.begin();
	while (iter != mActiveIndexList.end())
	{
		mTranslateMat = glm::translate(mIdentityMat, mAstroidArray[*iter]->mTranslate);
		mAstroidArray[*iter]->Render(glm::value_ptr(mTranslateMat));
#ifdef COLLISION_DEBUG
		mAstroidArray[*iter]->mDetectionSphere->Render(glm::value_ptr(mTranslateMat));
#endif
		++iter;
	}
}

void AstroidManager::MaxActiveAstroids(const unsigned int& max)
{
	mMaxActiveAstroids = (max < mMaxAstroids) ? max : mMaxAstroids;
}

const unsigned int& AstroidManager::MaxActiveAstroids() const
{
	return mMaxActiveAstroids;
}

void AstroidManager::StartingSpeed(const float& speed)
{
	mStartingSpeed = (speed < mMaxSpeed) ? speed : mMaxSpeed;
}

const float& AstroidManager::StartingSpeed() const
{
	return mStartingSpeed;
}

void AstroidManager::Init()
{
	mLastSpawnTime = std::chrono::high_resolution_clock::now();
	mAstroidArray = new Astroid*[mMaxAstroids];
	for (int i = 0; i < mMaxAstroids; ++i)
	{
		mAstroidArray[i] = new Astroid();
		mAvailableIndexQueue.push(i);
	}
}

void AstroidManager::TrySpawn()
{
	auto currtime = std::chrono::high_resolution_clock::now();
	float elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(
		currtime - mLastSpawnTime).count();

	if (elapsedtime >= mSpawnMinMaxTimes.second || (elapsedtime >= mSpawnMinMaxTimes.first && (3 < mSpawnDist(mGen))))
	{
		// Only spawn maximum allowed
		if (mActiveIndexList.size() < mMaxActiveAstroids)
		{
			unsigned int index = mAvailableIndexQueue.front();
			mAvailableIndexQueue.pop();
			mActiveIndexList.push_front(index);
			mAstroidArray[index]->Spawn(glm::vec3(mPosDist(mGen), 1.10f, 0.0f));
			mLastSpawnTime = std::chrono::high_resolution_clock::now();
		}
	}

}

#endif