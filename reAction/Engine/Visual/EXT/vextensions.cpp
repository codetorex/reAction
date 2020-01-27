
#include "StdAfx.h"
#include "vextensions.h"


char* extlist;

void InitializeAllExtensions()
{
	LoadExtensionList();

	Initialize_GL_ARB_multitexture();
	Initialize_GL_ARB_vertex_program();
	Initialize_GL_EXT_texture3D();
	Initialize_GL_20();
}

void LoadExtensionList()
{
	extlist = new char [strlen((char*)glGetString(GL_EXTENSIONS))+1];
	strcpy (extlist,(char *)glGetString(GL_EXTENSIONS));
}

bool IsExtensionSupported(char* extname)
{
	if (strstr(extlist,extname) > 0)
	{
		return true;
	}
	return false;
}

char* GetExtensionList()
{
	return extlist;
}

PFNGLACTIVETEXTUREPROC				glActiveTexture			= NULL;
PFNGLCLIENTACTIVETEXTUREPROC		glClientActiveTexture	= NULL;
PFNGLMULTITEXCOORD1DPROC			glMultiTexCoord1d		= NULL;
PFNGLMULTITEXCOORD1DVPROC			glMultiTexCoord1dv		= NULL;
PFNGLMULTITEXCOORD1FPROC			glMultiTexCoord1f		= NULL;
PFNGLMULTITEXCOORD1FVPROC			glMultiTexCoord1fv		= NULL;
PFNGLMULTITEXCOORD1IPROC			glMultiTexCoord1i		= NULL;
PFNGLMULTITEXCOORD1IVPROC			glMultiTexCoord1iv		= NULL;
PFNGLMULTITEXCOORD1SPROC			glMultiTexCoord1s		= NULL;
PFNGLMULTITEXCOORD1SVPROC			glMultiTexCoord1sv		= NULL;
PFNGLMULTITEXCOORD2DPROC			glMultiTexCoord2d		= NULL;
PFNGLMULTITEXCOORD2DVPROC			glMultiTexCoord2dv		= NULL;
PFNGLMULTITEXCOORD2FPROC			glMultiTexCoord2f		= NULL;
PFNGLMULTITEXCOORD2FVPROC			glMultiTexCoord2fv		= NULL;
PFNGLMULTITEXCOORD2IPROC			glMultiTexCoord2i		= NULL;
PFNGLMULTITEXCOORD2IVPROC			glMultiTexCoord2iv		= NULL;
PFNGLMULTITEXCOORD2SPROC			glMultiTexCoord2s		= NULL;
PFNGLMULTITEXCOORD2SVPROC			glMultiTexCoord2sv		= NULL;
PFNGLMULTITEXCOORD3DPROC			glMultiTexCoord3d		= NULL;
PFNGLMULTITEXCOORD3DVPROC			glMultiTexCoord3dv		= NULL;
PFNGLMULTITEXCOORD3FPROC			glMultiTexCoord3f		= NULL;
PFNGLMULTITEXCOORD3FVPROC			glMultiTexCoord3fv		= NULL;
PFNGLMULTITEXCOORD3IPROC			glMultiTexCoord3i		= NULL;
PFNGLMULTITEXCOORD3IVPROC			glMultiTexCoord3iv		= NULL;
PFNGLMULTITEXCOORD3SPROC			glMultiTexCoord3s		= NULL;
PFNGLMULTITEXCOORD3SVPROC			glMultiTexCoord3sv		= NULL;
PFNGLMULTITEXCOORD4DPROC			glMultiTexCoord4d		= NULL;
PFNGLMULTITEXCOORD4DVPROC			glMultiTexCoord4dv		= NULL;
PFNGLMULTITEXCOORD4FPROC			glMultiTexCoord4f		= NULL;
PFNGLMULTITEXCOORD4FVPROC			glMultiTexCoord4fv		= NULL;
PFNGLMULTITEXCOORD4IPROC			glMultiTexCoord4i		= NULL;
PFNGLMULTITEXCOORD4IVPROC			glMultiTexCoord4iv		= NULL;
PFNGLMULTITEXCOORD4SPROC			glMultiTexCoord4s		= NULL;
PFNGLMULTITEXCOORD4SVPROC			glMultiTexCoord4sv		= NULL;

void Initialize_GL_ARB_multitexture()
{
	if( ! IsExtensionSupported("GL_ARB_multitexture") )
	{
		// TODO: ADD DEBUG REPORT HEA
		MessageBox(0,"GL_ARB_multitexture not supported!","Error",MB_OK);
		return;
	}

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)
				wglGetProcAddress("glActiveTextureARB");
	glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)
				wglGetProcAddress("glClientActiveTextureARB");

	glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)
				wglGetProcAddress("glMultiTexCoord1dARB");
	glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)
				wglGetProcAddress("glMultiTexCoord1dvARB");
	glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)
				wglGetProcAddress("glMultiTexCoord1fARB");
	glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)
				wglGetProcAddress("glMultiTexCoord1fvARB");
	glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)
				wglGetProcAddress("glMultiTexCoord1iARB");
	glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)
				wglGetProcAddress("glMultiTexCoord1ivARB");
	glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)
				wglGetProcAddress("glMultiTexCoord1sARB");
	glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)
				wglGetProcAddress("glMultiTexCoord1svARB");

	glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)
				wglGetProcAddress("glMultiTexCoord2dARB");
	glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)
				wglGetProcAddress("glMultiTexCoord2dvARB");
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)
				wglGetProcAddress("glMultiTexCoord2fARB");
	glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)
				wglGetProcAddress("glMultiTexCoord2fvARB");
	glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)
				wglGetProcAddress("glMultiTexCoord2iARB");
	glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)
				wglGetProcAddress("glMultiTexCoord2ivARB");
	glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)
				wglGetProcAddress("glMultiTexCoord2sARB");
	glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)
				wglGetProcAddress("glMultiTexCoord2svARB");

	glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)
				wglGetProcAddress("glMultiTexCoord3dARB");
	glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)
				wglGetProcAddress("glMultiTexCoord3dvARB");
	glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)
				wglGetProcAddress("glMultiTexCoord3fARB");
	glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)
				wglGetProcAddress("glMultiTexCoord3fvARB");
	glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)
				wglGetProcAddress("glMultiTexCoord3iARB");
	glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)
				wglGetProcAddress("glMultiTexCoord3ivARB");
	glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)
				wglGetProcAddress("glMultiTexCoord3sARB");
	glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)
				wglGetProcAddress("glMultiTexCoord3svARB");

	glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)
				wglGetProcAddress("glMultiTexCoord4dARB");
	glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)
				wglGetProcAddress("glMultiTexCoord4dvARB");
	glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)
				wglGetProcAddress("glMultiTexCoord4fARB");
	glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)
				wglGetProcAddress("glMultiTexCoord4fvARB");
	glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)
				wglGetProcAddress("glMultiTexCoord4iARB");
	glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)
				wglGetProcAddress("glMultiTexCoord4ivARB");
	glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)
				wglGetProcAddress("glMultiTexCoord4sARB");
	glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)
				wglGetProcAddress("glMultiTexCoord4svARB");
}


PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB  = NULL;
PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB  = NULL;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB  = NULL;
PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB  = NULL;
PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB  = NULL;
PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB  = NULL;
PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB  = NULL;
PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB  = NULL;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB  = NULL;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB  = NULL;
PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB  = NULL;
PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB  = NULL;
PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB  = NULL;
PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB  = NULL;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB  = NULL;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB  = NULL;
PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB  = NULL;
PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB  = NULL;
PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB  = NULL;
PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB  = NULL;
PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB  = NULL;
PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB  = NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB  = NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB  = NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB  = NULL;
PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB  = NULL;
PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB  = NULL;
PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB  = NULL;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB  = NULL;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB  = NULL;
PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB  = NULL;
PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB  = NULL;
PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB  = NULL;
PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB  = NULL;
PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB  = NULL;
PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB  = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB  = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB  = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB  = NULL;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB  = NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB  = NULL;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB  = NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB  = NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB  = NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB  = NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB  = NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB  = NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB  = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB  = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB  = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB  = NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB  = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB  = NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB  = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB  = NULL;
PFNGLGETPROGRAMIVARBPROC glGetProgramivARB  = NULL;
PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB  = NULL;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB  = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB  = NULL;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB  = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB  = NULL;
PFNGLISPROGRAMARBPROC glIsProgramARB  = NULL;

void Initialize_GL_ARB_vertex_program()
{
	if( ! IsExtensionSupported("GL_ARB_vertex_program") )
	{
		// TODO: ADD DEBUG REPORT HEA
		MessageBox(0,"GL_ARB_vertex_program not supported!","Error",MB_OK);
		return;
	}
	glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC)wglGetProcAddress("glVertexAttrib1dARB");
	glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC)wglGetProcAddress("glVertexAttrib1dvARB");
	glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC)wglGetProcAddress("glVertexAttrib1fARB");
	glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC)wglGetProcAddress("glVertexAttrib1fvARB");
	glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC)wglGetProcAddress("glVertexAttrib1sARB");
	glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC)wglGetProcAddress("glVertexAttrib1svARB");
	glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC)wglGetProcAddress("glVertexAttrib2dARB");
	glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC)wglGetProcAddress("glVertexAttrib2dvARB");
	glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC)wglGetProcAddress("glVertexAttrib2fARB");
	glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC)wglGetProcAddress("glVertexAttrib2fvARB");
	glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC)wglGetProcAddress("glVertexAttrib2sARB");
	glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC)wglGetProcAddress("glVertexAttrib2svARB");
	glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC)wglGetProcAddress("glVertexAttrib3dARB");
	glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC)wglGetProcAddress("glVertexAttrib3dvARB");
	glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");
	glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC)wglGetProcAddress("glVertexAttrib3fvARB");
	glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC)wglGetProcAddress("glVertexAttrib3sARB");
	glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC)wglGetProcAddress("glVertexAttrib3svARB");
	glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC)wglGetProcAddress("glVertexAttrib4NbvARB");
	glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC)wglGetProcAddress("glVertexAttrib4NivARB");
	glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC)wglGetProcAddress("glVertexAttrib4NsvARB");
	glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC)wglGetProcAddress("glVertexAttrib4NubARB");
	glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC)wglGetProcAddress("glVertexAttrib4NubvARB");
	glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC)wglGetProcAddress("glVertexAttrib4NuivARB");
	glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC)wglGetProcAddress("glVertexAttrib4NusvARB");
	glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC)wglGetProcAddress("glVertexAttrib4bvARB");
	glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC)wglGetProcAddress("glVertexAttrib4dARB");
	glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC)wglGetProcAddress("glVertexAttrib4dvARB");
	glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC)wglGetProcAddress("glVertexAttrib4fARB");
	glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC)wglGetProcAddress("glVertexAttrib4fvARB");
	glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC)wglGetProcAddress("glVertexAttrib4ivARB");
	glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC)wglGetProcAddress("glVertexAttrib4sARB");
	glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC)wglGetProcAddress("glVertexAttrib4svARB");
	glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC)wglGetProcAddress("glVertexAttrib4ubvARB");
	glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC)wglGetProcAddress("glVertexAttrib4uivARB");
	glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC)wglGetProcAddress("glVertexAttrib4usvARB");
	glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
	glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
	glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");
	glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
	glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
	glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC)wglGetProcAddress("glDeleteProgramsARB");
	glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
	glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC)wglGetProcAddress("glProgramEnvParameter4dARB");
	glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC)wglGetProcAddress("glProgramEnvParameter4dvARB");
	glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC)wglGetProcAddress("glProgramEnvParameter4fARB");
	glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC)wglGetProcAddress("glProgramEnvParameter4fvARB");
	glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC)wglGetProcAddress("glProgramLocalParameter4dARB");
	glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)wglGetProcAddress("glProgramLocalParameter4dvARB");
	glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)wglGetProcAddress("glProgramLocalParameter4fARB");
	glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)wglGetProcAddress("glProgramLocalParameter4fvARB");
	glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramEnvParameterdvARB");
	glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramEnvParameterfvARB");
	glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramLocalParameterdvARB");
	glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramLocalParameterfvARB");
	glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)wglGetProcAddress("glGetProgramivARB");
	glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC)wglGetProcAddress("glGetProgramStringARB");
	glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC)wglGetProcAddress("glGetVertexAttribdvARB");
	glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC)wglGetProcAddress("glGetVertexAttribfvARB");
	glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC)wglGetProcAddress("glGetVertexAttribivARB");
	glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)wglGetProcAddress("glGetVertexAttribPointervARB");
	glIsProgramARB = (PFNGLISPROGRAMARBPROC)wglGetProcAddress("glIsProgramARB");

}


PFNGLTEXIMAGE3DPROC glTexImage3D					= NULL;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D				= NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D		= NULL;

void Initialize_GL_EXT_texture3D()
{
	/*if( ! IsExtensionSupported("GL_ARB_multitexture") )
	{
		// TODO: ADD DEBUG REPORT HEA
		MessageBox(0,"GL_ARB_multitexture not supported!","Error",MB_OK);
		return;
	}*/
	
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)
		wglGetProcAddress("glTexImage3D");

	glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)
		wglGetProcAddress("glTexSubImage3D");
	
	glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)
		wglGetProcAddress("glCopyTexSubImage3D");
}


PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = NULL;
PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLGETUNIFORMFVPROC glGetUniformfv = NULL;
PFNGLGETUNIFORMIVPROC glGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = NULL;
PFNGLISPROGRAMPROC glIsProgram = NULL;
PFNGLISSHADERPROC glIsShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM2IPROC glUniform2i = NULL;
PFNGLUNIFORM3IPROC glUniform3i = NULL;
PFNGLUNIFORM4IPROC glUniform4i = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = NULL;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;



void Initialize_GL_20()
{
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)wglGetProcAddress("glStencilOpSeparate");
	glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)wglGetProcAddress("glStencilFuncSeparate");
	glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)wglGetProcAddress("glStencilMaskSeparate");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)wglGetProcAddress("glGetAttachedShaders");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)wglGetProcAddress("glGetShaderSource");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glGetUniformfv = (PFNGLGETUNIFORMFVPROC)wglGetProcAddress("glGetUniformfv");
	glGetUniformiv = (PFNGLGETUNIFORMIVPROC)wglGetProcAddress("glGetUniformiv");
	glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)wglGetProcAddress("glGetVertexAttribdv");
	glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)wglGetProcAddress("glGetVertexAttribfv");
	glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)wglGetProcAddress("glGetVertexAttribiv");
	glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)wglGetProcAddress("glGetVertexAttribPointerv");
	glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
	glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
	glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
	glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)wglGetProcAddress("glVertexAttrib1d");
	glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)wglGetProcAddress("glVertexAttrib1dv");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)wglGetProcAddress("glVertexAttrib1s");
	glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)wglGetProcAddress("glVertexAttrib1sv");
	glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)wglGetProcAddress("glVertexAttrib2d");
	glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)wglGetProcAddress("glVertexAttrib2dv");
	glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)wglGetProcAddress("glVertexAttrib2f");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)wglGetProcAddress("glVertexAttrib2s");
	glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)wglGetProcAddress("glVertexAttrib2sv");
	glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)wglGetProcAddress("glVertexAttrib3d");
	glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)wglGetProcAddress("glVertexAttrib3dv");
	glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)wglGetProcAddress("glVertexAttrib3s");
	glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)wglGetProcAddress("glVertexAttrib3sv");
	glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)wglGetProcAddress("glVertexAttrib4Nbv");
	glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)wglGetProcAddress("glVertexAttrib4Niv");
	glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)wglGetProcAddress("glVertexAttrib4Nsv");
	glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)wglGetProcAddress("glVertexAttrib4Nub");
	glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)wglGetProcAddress("glVertexAttrib4Nubv");
	glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)wglGetProcAddress("glVertexAttrib4Nuiv");
	glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)wglGetProcAddress("glVertexAttrib4Nusv");
	glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)wglGetProcAddress("glVertexAttrib4bv");
	glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)wglGetProcAddress("glVertexAttrib4d");
	glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)wglGetProcAddress("glVertexAttrib4dv");
	glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)wglGetProcAddress("glVertexAttrib4iv");
	glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)wglGetProcAddress("glVertexAttrib4s");
	glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)wglGetProcAddress("glVertexAttrib4sv");
	glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)wglGetProcAddress("glVertexAttrib4ubv");
	glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)wglGetProcAddress("glVertexAttrib4uiv");
	glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)wglGetProcAddress("glVertexAttrib4usv");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}