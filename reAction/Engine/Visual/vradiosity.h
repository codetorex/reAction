#ifndef VRADIOSITY_H
#define VRADIOSITY_H

#include "../Mathematics/mathlib.h"
#include "vcolor.h"
#include "vmodel.h"
#include "vskybox.h"

#define MAX_LIGHTMAPSIZE	1024
#define MAX_ANGLE			5.0f	// 5 degrees of freedom
#define MAP_TOTALPADDING	2		// 2 means left + right padding or top + bottom padding
#define MAP_HALFPADDING		1		// padding for one side
#define DETAIL_SIZE			128		// lower values will fastener render process

#define RAD_FRONT			0x0
#define RAD_BOTTOM			0x1

class VRScanline;
class VRLightmap;
class VRadiosity;

class VREnvironment
{
public:
	VTexture hemicube[2];
	VTexture empty;
	VRadiosity *vrad;
	RMDModel *mdl;
	VSkyBox *sky;
	VCamera *cam;
	dword rbase; // precachebase

	void LoadHemicube(char* fpath1,char* fpath2);
	void SetupEnvironment(RMDModel& eMdl, VSkyBox& eSky, VCamera &eCam);
	void Render();
	void Render2();
	void Precache();
	void RenderMultiplier(int direction);
	void GatherTexture(bool half,vec3 OUT &clr);
};

class VRFace
{
public:
	RMDMesh *obj;
	int faceid;

	tri3 triangle;
	
	vec3 fnor; // normal
	vec3 ftan; // tangent
	vec3 fbin; // binormal

	void SetData(RMDMesh* cmesh, int face);
	void TraceLightRay(ray3 &lray, RMDLight& lght, int px,int py ,vec3 & p3d,VRLightmap* lmap);
	void VisulizeRay(ray3 &lray, RMDLight& lght, int px,int py ,vec3 & p3d,VRLightmap* lmap);
	void Radiate(int px,int py, vec3 &p3d,VRLightmap* lmap);
};

class VRLinePart // render line part
{
public:
	VRScanline* vsline; // scanline

	int startx;
	int endx;
	vec3 startv;
	vec3 endv;
	vec3 startn; // start normal
	vec3 endn; // end normal
	VRFace* cface;

	VRLinePart* nextpart;

	void CalculateInitialLight();
	void CalculateRadiation();
	void VisualizeRay(int x);
};

class VRScanline // render scanline
{
public:
	VRScanline();
	VRLightmap *lmap;

	VRLinePart *first;
	VRLinePart *last;
	int partcount;
	int y;

	VRLinePart* CreatePart();
};

class VRLightmap // render surface
{
public:
	VRadiosity* vrad; 
	VRScanline* slines;
	VTexture *stexture; // stores final bounce
	VTexture *sinitial; // stores initial light
	long remainingarea;
	VPackNode mainquad;
	bool is_ilc; // is initial light calculated?
	
	void AddLinePart(int y,int x1,int x2,VRFace* fc,tri2* tSpc);
	void FillTriangle(VRFace* fc,tri2* tSpc);
	void FillQuad(VRFace* fc,tri2* tSpc,VPackNode* pnode);
	void InitializeLightmap(int width,int height);
	VPackNode* AllocateQuad(int width,int height);
	static VRLightmap* CreateNewLightmap(VRadiosity* rad);
	VRLinePart* GetLinePart(int x,int y); // get line for pixel coordinates
	
	void CalculateInitialLight();
	void CalculateLightmap();
};

// this finds faces which shares same plane. so it finds planes
// also used for transform it...
class VRPlanar
{
public:
	VRPlanar();

	VRadiosity* vrad; 

	int objid;

	vec3 highcorner;
	vec3 lowcorner;
	vec3 center;

	double cx,cy,cz;

	vec3 fn;

	int facecount;

	RArray triangles;
	RArray faceids;

	VRLightmap*	alightmap; // the lightmap which this plane allocated
	VPackNode*	alpnode; // light map pack node

	void AddFace(int fid, tri3 *tri,bool* pFaces);
	void FindNeighbours(RMDMesh* cmesh,int fid,int* fvi,bool* pFaces);
	void FindEdgeNeighbours(RMDMesh* cmesh,int fid,int sv1,int sv2, bool* pfaces);
	void FindQuads(RMDMesh* cmesh,int fid,int* fvi,bool* pFaces);
	bool FindQuad(RMDMesh* cmesh,int fid,int sv1,int sv2, bool* pfaces,int sv3);
	int	 GetCorner(int* fvi,int sv1,int sv2);
	void GetMidPoint();
	void RotateToNewNormal(vec3 nn); // rotates plane to have it new normal
	void Project2D(); // makes it usable to texture mapping
	void Vec3Test(vec3*v1 ,vec3* v2,bool dir = true); // used for finding min max
	void Tri3Test(tri3* tri); // used for find min max with all vertexes of a triangle
	void MoveToZero(); // makes lowest corner = 0,0,0
	void Scale(float sz);
	void Render(); // render with opengl
	void AllocateLightmap(RArray* lightmaps);
	void ConvertToTextureCoords(tri2* tSpc);
};

/*
	todo: find as quads
	allocate on lightmap if quad.
	if not quad allocate triangle.
  */
// radiosity simulator version 9.0 beta
class VRadiosity
{
public:
	VRadiosity();

	RArray		lightmaps;
	RArray		fplanes; // found planes
	RMDModel	*mdl;
	tri2		**tSpc; // 2d triangles found on texture space
	VRFace		**faces;
	VREnvironment radenv; // will used for radiate function

	void PrepareGeometry(float ptsr = 0.234376f); // PixelToSceneRatio
	void CalculateRadiosity();

	void TestRender();
	void TestRenderLighting();
	void TestRenderNormals();
	void TestRenderParts();
	VRFace* TestProjectRay(ray3 &r,vec3 &p,tri2& tx);
};


#endif