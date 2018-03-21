#ifndef POINT_DATA_HEADER_INCLUDED
#define POINT_DATA_HEADER_INCLUDED
#include<memory>

template<class T>
class PointData
{
protected:
	static void Deleter(T*);
	std::shared_ptr<T> mDataPtr;
	unsigned int mSize;
	unsigned int mLength;
public:
	PointData();
	virtual ~PointData();

	virtual void SetPointData(T*, const unsigned int&);

	std::shared_ptr<T> DataPtr();
	unsigned int Length();
	unsigned int Size();
	
};

template<class T>
void PointData<T>::Deleter(T* ptr)
{
	std::cout << __FUNCTION__ << ' ' << __LINE__ << '\n';
	if (ptr != nullptr)
	{
		std::cout << __FUNCTION__ << ' ' << __LINE__ << '\n';
		delete[] ptr;
	}
	std::cout << __FUNCTION__ << ' ' << __LINE__ << '\n';
}

template<class T>
PointData<T>::PointData() :
	mDataPtr(std::shared_ptr<T>(nullptr, &PointData::Deleter)),
	mLength(0),
	mSize(0)	
{

}

template<class T>
void PointData<T>::SetPointData(T* ptr, const unsigned int& data_length)
{
	mDataPtr = std::shared_ptr<T>(new T[data_length], &PointData::Deleter);
	for (unsigned int i = 0; i < data_length; ++i)
	{
		mDataPtr.get()[i] = ptr[i];
	}
	mLength = data_length;
	mSize = sizeof(T)*data_length;
}

template<class T>
std::shared_ptr<T> PointData<T>::DataPtr()
{
	return mDataPtr;
}

template<class T>
unsigned int PointData<T>::Length()
{
	return mLength;
}

template<class T>
unsigned int PointData<T>::Size()
{
	return mSize;
}

template<class T>
PointData<T>::~PointData()
{

}

#endif