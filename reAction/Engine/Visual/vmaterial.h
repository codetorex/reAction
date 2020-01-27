#ifndef VMATERIAL_H
#define VMATERIAL_H

#include "vtexture.h"
#include "vcolor.h"
#include "vshader.h"

#define mp_generictexture	0
#define mp_reflective		1
#define mp_fromfile			2

// MATERIAL BUILDER kod ve ARABIRIM ilen bissuru materiali ayarlama olabilir

// ilk basta tek class yapalim gerekirse bu base olur derive yapariz
class VMaterial
{
public:
	char				*matname;
	char				*matpath;
	long				matpathhash;
	long				flags;

	int					mp_shadertype;
	VShader*			mp_shader;
	VTexture*			mp_diffusemap;
	color				mp_ambientclr;
	color				mp_diffuseclr;
	color				mp_specularclr;
	float				mp_selfillumination;
	color				mp_selfilluminationclr;
	float				mp_alpha;
	VTexture*			mp_opacitymap;
	VTexture*			mp_bumpmap;
	VTexture*			mp_reflection;

	VMaterial*			nextmat;
	VMaterial*			prevmat;

	void LoadMaps();
	void SetParameter(char* paramname,char* paramdata);
	void AttachMaterial();
	void DetachMaterial();

};

class VMaterialMemory
{
public:
	VMaterialMemory();
	VMaterial*	firstmat;
	VMaterial*	lastmat;
	int			matcount;

	void AddMaterial(VMaterial* mt);
	void RemoveMaterial(VMaterial* mt);

	VMaterial* LoadRMT(char* rmtpath);
	VMaterial* GetMaterialByPath(char* mpath,VMaterial* start = 0);
};

// make this function check from meoery
VMaterial* LoadRMT(char* rmtpath);

extern VMaterialMemory mMaterials;

#endif