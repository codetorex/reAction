
#ifndef VMODEL_H
#define VMODEL_H

#include "../Mathematics/mathlib.h"
#include "vtexture.h"
#include "vmaterial.h"
#include "GUI/vgui.h"
#include "../Tools/rtools.h"
#include "../Tools/rfilesystem.h"
#include "../Visual/vcamera.h"


using namespace RTools;


#define RMD_MESH		1
#define RMD_LIGHT		2
#define RMD_CAMERA		4
#define RMD_KEYFRAME	8
#define RMD_PHYSIQUE	16
#define RMD_SKELETAL	32
#define RMD_TEXTURE		64
#define RMD_CANIM		128
#define RMD_ENTITY		256
#define RMD_TANGENT		512
#define RMD_ALL			RMD_MESH | RMD_TEXTURE | RMD_LIGHT | RMD_CAMERA | RMD_PHYSIQUE | RMD_SKELETAL

// Map Flags
#define MAP_DIFFUSE			1
#define MAP_OPACITY			2
#define MAP_BUMP			4
#define MAP_REFLECTION		8	
#define MAP_RAYTRACE		16	// if MAP_RAYTRACE && MAP_REFLECTION == 1 then generate raytraced reflection BOX
#define MAP_MULTI			32

struct RMDHeader
{
	long signature;
	long directorysize;
	char matbasepath[128]; // ilerde lazim olcak
};

enum RMDChunk
{
	c_object,
	c_light,
	c_camera,
	c_texture,
	c_entity,
	c_bonebase,
	c_boneanim
};

class RMDDirectory
{
public:
	RMDChunk chunktype;
	long count;
	long *offsets;
};

class RMDLight
{
public:
	char	lightname[64];
	long	lighttype;
	float	lightpos[3]; // lights position
	float	targetpos[3]; // targets position
	float	intensity;
	float	atten[4];
	color	clr; // color
};

class RMDCamera
{
public:
	char cameraname[64];
	float camera[3]; // camera position
	float target[3]; // target position

	void SetCamera(VCamera* cm);
};

class RMDBone;

class RMDWeightedVertex
{
public:
	RMDWeightedVertex();
	
	int ibc; // influenced bone count
	RMDBone** boneids;
	float* weights;
	
	void PrepareMemory(int ibonecount);
	void AddBone(int boneid,float weight);
};


class RMDMesh
{
public:
	char	objname[64];
	
	long	matid;
	long	vertexcount;
	long	facecount;
	long	tvcount;
	long	normalcount;
	bool	hasphysique;
	
	float	center[3]; // x ,y,z of center
	float	boundingbox[6]; // x1,y1,z1, x2,y2,z2
	
	float*	vexs;
	int*	faces;
	float*	texcords; 
	int*	tfaces; 
	float*	normals;
	RMDWeightedVertex* boneinfo;

	float*	vexsanim;

	void	GetTriangle(IN int faceid, OUT tri3* triangle, OUT int* pindices = 0); // later we can put vertex normals etc...
	void	GetVertices(IN int faceid, OUT vec3* v1, OUT vec3* v2, OUT vec3* v3, OUT int* pindices = 0);
	void	GetUV(IN int faceid,OUT vec2* v1, OUT vec2* v2, OUT vec2* v3); // or texture space
	void	GetUV(IN int faceid,OUT vec3* v1, OUT vec3* v2, OUT vec3* v3); // or texture space
	vec3	GetAverageNormals(IN int faceid);
	void	CalculateTangentSpace(IN int faceid, OUT vec3&  fnor, OUT vec3& ftan, OUT vec3& fbin);
};

class RMDMaterial
{
public:
	char	matname[64];
	long	flags;
	long	submatcount;
	union
	{
		VMaterial*	singlmat;
		VMaterial** multimat;
	};
};

class RMDBoneKeyFrame
{
public:
	float pos[3]; // vector position
	float rot[3]; // euler rotation
	//float mat[12];
	matrix4x4f mat;
	void mul_matrix(matrix4x4f* mt);

	int time;
};

class RMDSkeleton;

class RMDBone
{
public:
	//RMDBone();
	int boneid;
	char bonename[128];
	char boneparent[128];
	int bonenamehash;
	//int parentid;
	int framec; // key frame count
	matrix4x4f m_init;
	matrix4x4f m_final;
	//RMDBoneKeyFrame m_init;
	//matrix4x4f m_initmat;
	//matrix4x4f m_initfinal;

	void setchilds(int frameid);
	void drawchilds(int dz=0);

	void drawbase(int dz=0);
	
	RMDBoneKeyFrame *kframes; // key frames array
	
	RMDSkeleton* sklt;
	RMDBone* parent;
	RArray childs;
};

class RMDSkeleton
{
public:
	bool baseskeleton;
	int keystart;
	int keyend;
	int framerate;
	
	int bonecount;
	RMDBone** bones;
	
	RMDBone* GetBoneFromName(char* bonename);
	RMDBone* GetBoneByNameHash(int hsh);
	RMDBone* GetBoneById(int bid);
	void FixBoneConnections();
};


class RMDModel
{
public:
	RMDModel();

	RMDSkeleton*skanim;
	void		RenderSkeleton(int frameid);
	void		PrepareForSkeletalAnimation();

	void		BoneMul(RMDBone* bn,float weig,vec3* vx);

	void		RenderBaseSkeleton();
	void		RenderAnimSkeleton();
	
	RMDHeader	fileheader;
	RMDDirectory*directorys;
	RMDMesh		*meshes;
	RMDCamera	*cameras;
	RMDLight	*lights;
	RMDMaterial	*mats;
	RMDSkeleton	*skeleton;
	DWORD		meshcount;
	DWORD		cameracount;
	DWORD		lightcount;
	DWORD		matcount;

	vec3		center;
	vec3		highcorner;
	vec3		lowcorner;


	char*		rmdname;
	char*		filepath;
	char*		matbasepath;
	RFile*		rmdfp;
	//int			LoadRMD(char* rmdpath,long loadparams = RMD_ALL); // loads rmd data chunks
	void		OpenRMD(char* rmdpath); // just opens RMD file
	void		LoadDirectorys();
	void		LoadMeshes();
	void		LoadMaterials();
	void		LoadCameras();
	void		LoadLights();
	void		LoadBaseSkeleton();
	void		LoadAnimSkeleton();
	void		CloseRMD();

	void		FixBoneConnections(); // connects bones with id to memory class

	void		Render();

private:
	RMDDirectory*	GetDirectory(RMDChunk chnktyp);
	void			ReadBoneKeyframe(RMDBoneKeyFrame* bkf);

};

class VRMDMemory // holds list of all data loaded into memory
{
public:
	RMDModel		*models;
	RMDCamera		*cameras;
	RMDLight		*lights;
	RMDMesh			*meshes;

	int				modelcount;
	int				cameracount;
	int				lightcount;
	int				meshcount;
	int				materialcount;
	long			usedmem;

	void*			GetObjectByName(char* objname); // searches for objname in all lists and returns its pointer
	RMDCamera*		GetCameraByName(char* camname);
	RMDLight*		GetLightByName(char* ligname);
	RMDModel*		GetModelByName(char* mdlname);
	RMDMesh*		GetMeshByName(char* meshname);

	RMDModel*		LoadRMD(char* rmdpath,char* basetd=0,long loadparams = RMD_ALL);
};

extern VRMDMemory mModels;
RMDModel*	LoadRMD(char* rmdpath,char* basetd = 0,long loadparams = RMD_ALL);

// use getmaterialmemory for accessing material memory

#endif