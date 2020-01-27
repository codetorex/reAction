
#ifndef VTERRAIN_H
#define VTERRAIN_H

#include "vcamera.h"
#include "../Mathematics/mvector.h"
#include "vtexture.h"
#include "vmodel.h"
#include "../Tools/rtools.h"

// not: temperature gunesten gelen isin miktari ve acisiyla olculur...


// todo: implement better manager
// description -
/* this is a 3 level tree data structure;
level0 is root
level1 is altitude
level2 is slope
level3 is temperature
so it manages the terrain ecosystem.
it maybe managed with better code but first requirement
is working one...
*/

// in future i will put it into linked list
class TEcosystemNode;
class TEcosystemData
{
public:
	TEcosystemData();

	TEcosystemNode* owner;

	int texcount;
	int curtex;
	VTexture** textures;
	int* texdensity;
	// add models here
	// add billboards here
	// add animal entitys here

	void PrepareData(int count);
	void AddTexture(VTexture* tx, float dens);

	VTexture* GetRandomTexture();

};

// linked list formatted
class TEcosystemNode
{
public:
	TEcosystemNode();
	
	TEcosystemNode* parentnode;
	
	int nodecount;
	TEcosystemNode* firstnode;
	TEcosystemNode* lastnode;
	
	TEcosystemNode* nextnode;
	TEcosystemNode* prevnode;
	
	
	union
	{
		float altitude; // yukseklik
		float startslope; // aci
		float temperature; // sicaklik
	};
	
	TEcosystemNode* GetNode(float dt);

	void AddNode(TEcosystemNode* nd);
	
	TEcosystemData* data;
};
// todo: make data between 0.0 and 1.0 and use clamped values
class TEcosystem // terrain flora
{
public:
	char name[64];
	
	TEcosystemNode root;

	void SortData(); // will sort all data
	TEcosystemNode* AddNode(float alt,float slope,float temp);
	TEcosystemData* GetNode(float alt,float slope,float temp);
	
	int		layers3d;
	dword*	oldglids;
	float*	rcoords;
	byte*	texpixels;
	dword	texture3d;

	void Create3DTexture();
	float GetRCoord(dword glid);

	// in future i will implement them
	void LoadEcosystemScript(char* fpath);
	void SaveEcosystemScript(char* fpath);
};

/*
John Ratcliff of Verant put it this way: 
"Synthesize a high resolution detail map for the polygons immediately 
around the camera position, to show a great deal of perceived 
local detail. For everything else, mountains and such off in 
the distance, the base texture map works fine."
*/

class VTerrain
{
public:
	VTerrain();
	~VTerrain();

	int			n; // size = 2^n height map
	int			width;
	int			height;

	float		realwidth; // it can be scaled...
	float		realheight;

	float*		heightmap;	// will be multidimensional array
	float*		normx; // we need to fast access so dividing it to 3 parts is better than mult 3 or mult 2 + 1 
	float*		normy; // so we can instantly access with hmap pointer
	float*		normz;
	float*		txu;
	float*		txv;
	float*		txs;
	int*		texids;

	float		miny;
	float		maxy;
	
	int			lod; // level of detail

	int			sx,ex;
	int			sy,ey;

	VTexture*	synths;

	VCamera*	cam;

	TEcosystem*	ecosystem;

	void		Calculate3DTexture();
	void		CalculateNormals();
	void		CalculateTextures();
	void		SynthesizeBaseTexture();
	void		LoadBaseTexture(char* fpath);
	void		GenerateFromHeightMap(VTexture* hmap);
	
	float		GetHeight(int x,int y);
	void		SetHeight(int x,int y,float h);
	
	void		WriteHeightMap(char* fpth);

	// Cesitli sistemler icin 3 farkli sekilde render edilebilir
	// 1. alani onceden 1 kere render etmek sora o textureleri kullanmak ( boktan )
	// 2. tile based bi yeri 2 defa render ederek
	// 3. 3d texture kullanarak
	// 4. shader ilen

	void		Render(); // directly render without blend ( not applicable in release )
	void		RenderWith3DTexture();
	void		RenderWithBaseTexture();

	void		RenderFakeWater();
};


#endif