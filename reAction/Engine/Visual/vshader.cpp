#include "stdafx.h"
#include "vshader.h"


void VShader::LoadShader(char* fvertex,char* ffragment)
{
	RFile	*fv,*ff;
	char	*vs,*fs;

	vshdr = glCreateShader(GL_VERTEX_SHADER);
	fshdr = glCreateShader(GL_FRAGMENT_SHADER);

	fv = ROpenFile(fvertex,"rb");
	fv->getfilesize();
	vs = new char [fv->filesize+1];
	fv->getdata(vs,1,fv->filesize);
	vs[fv->filesize] = 0;
	RCloseFile(fv);

	ff = ROpenFile(ffragment,"rb");
	ff->getfilesize();
	fs = new char [ff->filesize+1];
	ff->getdata(fs,1,ff->filesize);
	fs[ff->filesize] = 0;
	RCloseFile(ff);

	const char * vv = vs;
	const char * fsf = fs;
	
	glShaderSource(vshdr,1,&vv,NULL);
	glShaderSource(fshdr,1,&fsf,NULL);

	glCompileShader(vshdr);
	glCompileShader(fshdr);

	free(vs);
	free(fs);

	program = glCreateProgram();

	glAttachShader(program,vshdr);
	glAttachShader(program,fshdr);

	glLinkProgram(program);
	glUseProgram(program);
}