

#ifndef VEXT_H
#define VEXT_H

#include "../../../StdAfx.h"
#include "GL_ARB_multitexture.h"
#include "GL_ARB_vertex_program.h"
#include "GL_EXT_texture3D.h"
#include "GL_2_0.h"


void	LoadExtensionList();
bool	IsExtensionSupported(char* extname);
char*	GetExtensionList();

void	Initialize_GL_ARB_multitexture();
void	Initialize_GL_ARB_vertex_program();
void	Initialize_GL_EXT_texture3D();
void	Initialize_GL_20();


void	InitializeAllExtensions();


#endif