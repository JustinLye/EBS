#ifndef BASE_SHADER_HEADER_INCLUDED
#define BASE_SHADER_HEADER_INCLUDED

#include<map>
#include<glad/glad.h>
#include"util/FileReader.h"
#include"util/UtilityDefs.h"
class BaseShader
{
protected:
	unsigned int mProgram;
	virtual void Create(const char*, const char*);
public:
	BaseShader(const char*, const char*);
	virtual ~BaseShader();
	virtual void Use();
	virtual unsigned int GetProgramId();

};

#endif