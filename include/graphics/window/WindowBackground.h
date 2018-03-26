#ifndef WINDOW_BACKGROUND_HEADER_INCLUDED
#define WINDOW_BACKGROUND_HEADER_INCLUDED
#include<glad/glad.h>
struct Colors
{
	float mRed;
	float mGreen;
	float mBlue;
	float mAlpha;
	Colors(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) :
		mRed(r),
		mGreen(g),
		mBlue(b),
		mAlpha(a)
	{

	}
};

class WindowBackground
{
protected:
	Colors mColor;
public:
	WindowBackground(float = 0.0f, float = 0.0f, float = 0.0f, float = 0.0f);
	~WindowBackground();
	virtual void Draw();
	void Red(float);
	void Green(float);
	void Blue(float);
	void Alpha(float);
	float Red() const;
	float Green() const;
	float Blue() const;
	float Alpha() const;
};


WindowBackground::WindowBackground(float r, float g, float b, float a) :
	mColor(r, g, b, a)
{

}

WindowBackground::~WindowBackground()
{

}

void WindowBackground::Draw()
{
	glClearColor(
		mColor.mRed,
		mColor.mBlue,
		mColor.mGreen,
		mColor.mAlpha);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void WindowBackground::Red(float r)
{
	mColor.mRed = r;
}
void WindowBackground::Green(float g)
{
	mColor.mGreen = g;
}
void WindowBackground::Blue(float b)
{
	mColor.mBlue = b;
}
void WindowBackground::Alpha(float a)
{
	mColor.mAlpha = a;
}
float WindowBackground::Red() const
{
	return mColor.mRed;
}
float WindowBackground::Green() const
{
	return mColor.mGreen;
}
float WindowBackground::Blue() const
{
	return mColor.mBlue;
}
float WindowBackground::Alpha() const
{
	return mColor.mAlpha;
}

#endif