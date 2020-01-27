
#ifndef VSKYBOX_H
#define VSKYBOX_H

#include "vcamera.h"
#include "../Mathematics/mvector.h"
#include "vtexture.h"


#define SKYBOX_UP		0
#define SKYBOX_DOWN		1
#define SKYBOX_RIGHT	2
#define SKYBOX_LEFT		3
#define SKYBOX_FRONT	4
#define SKYBOX_BACK		5


class VSkyBox
{
public:
	VCamera* cam;
	vec3 size;
	VTexture day[6];
	VTexture night[6];
	float daytime; // 0.0 = night , 1.0 = day

	void LoadSkyBox(char* sboxname); // *.RSB is Reaction SkyBox maybe combined with model file
	void SaveSkyBox(char* fpath);
	
	void RenderSkyBox();
	void RenderSkyBox(vec3 *pos);
};

#endif