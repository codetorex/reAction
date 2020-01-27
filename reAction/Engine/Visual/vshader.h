#ifndef VSHADER_H
#define VSHADER_H

#include "ext/GL_2_0.h"
#include "../tools/rfilesystem.h"

class VShader
{
public:
	dword program;

	dword vshdr;
	dword fshdr;
	
	void LoadShader(char* fvertex,char* ffragment);

	VShader* nextshader;
	VShader* prevshader;
};

class VShaderManager
{
public:
	VShader* firstshader;
	VShader* lastshader;

	int shadercount;
};


#endif
