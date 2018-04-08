#ifndef FONT_HEADER_INCLUDED
#define FONT_HEADER_INCLUDED
#ifdef FONT_DEBUG
#include<iostream>
#endif
#include<map>
#include<string>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	GLuint mTextureId;
	glm::ivec2 mSize;
	glm::ivec2 mBearing;
	GLuint mAdvance;
};

class Font
{
public:
	Font();
	virtual ~Font();
	virtual const char* PathToFont() = 0;
	virtual int FontWidth() = 0;
	virtual int FontHeight() = 0;
	
	void Init();
	void Destroy();
	const Character& operator[](const GLchar&) const;
protected:
	std::map<GLchar, Character> mCharacterMap;
private:
	static bool InitOpenGL;
	GLuint* mTextures;
};

bool Font::InitOpenGL = true;

Font::Font() :
	mTextures(nullptr)
{

}

Font::~Font()
{
	Destroy();
}

void Font::Destroy()
{
	if (mTextures != nullptr)
	{
		glDeleteTextures(128, mTextures);
		delete[] mTextures;
		mTextures = nullptr;
	}
}

void Font::Init()
{
	if (InitOpenGL)
	{
		InitOpenGL = false;
//		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	FT_Library ft;
	mTextures = new GLuint[128];
	glGenTextures(128, mTextures);
	if (FT_Init_FreeType(&ft))
	{
		throw std::runtime_error("Failed to init FreeType lib");
	}
	FT_Face face;
	if (FT_New_Face(ft, PathToFont(), 0, &face))
	{
		throw std::runtime_error("Failed to load font.");
	}
	FT_Set_Pixel_Sizes(face, FontWidth(), FontHeight());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
#ifdef FONT_DEBUG
			std::cout << "Failed to load Glyph " << c << '\n';
			continue;
#else
			Destroy();
			throw std::runtime_error("Failed to load Glyph");
#endif
		}
		glBindTexture(GL_TEXTURE_2D, mTextures[c]);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		mCharacterMap.insert(
		{
			(const GLchar)c,
			{
				mTextures[c],
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			}
		});
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

const Character& Font::operator[](const GLchar& character) const
{
	auto result = mCharacterMap.find(character);
	if (result == mCharacterMap.end())
	{
		throw std::runtime_error("Character not found.");
	}
	return result->second;
}


#endif