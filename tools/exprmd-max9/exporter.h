#ifndef __EXPORTER_H
#define __EXPORTER_H

#include "stdafx.h"
#include "cs/phyexp.h"
#include "cs/bipexp.h"
#include "cs/keytrack.h"
#include "resource.h"
#include <time.h>
#include "farray.h"

void AddDbg2(char* fmt,...);

#define MYEXP_CLASSID  Class_ID(0x4d6b417b, 0x232417c9)
#define MYEXP_EXT "rmd"
#define MYEXP_VER 010

#define DEBUGMODE 3
/*
	0 = Off
	1 = On list box
	2 = Log output
	3 = Both
*/
// File Header Flags
#define EXPORT_MESH			1 //*
#define EXPORT_LIGHT		2 //*
#define EXPORT_CAMERA		4 //*
#define EXPORT_KEYFRAME		8
#define EXPORT_PHYSIQUE		16 //*
#define EXPORT_SKELETAL		32 //*
#define EXPORT_MATERIALS	64 //*
#define EXPORT_CANIM		128
#define EXPORT_ENTITY		256
#define EXPORT_TANGENT		512


// Map Flags
#define MAP_DIFFUSE			1
#define MAP_OPACITY			2
#define MAP_BUMP			4
#define MAP_REFLECTION		8	
#define MAP_RAYTRACE		16	// if MAP_RAYTRACE && MAP_REFLECTION == 1 then generate raytraced reflection BOX
#define MAP_MULTI			32


#define ISEXP(a)	((exportmode & a) > 0)

typedef unsigned char byte;
class RMDWeightedVertex;
class RMDSkeleton;

class RMDHeader 
{
public:
	long	signature;
	long	objectcount;
	long	lightcount;
	long	cameracount;
	long	texturecount;
	long	entitycount;	
	long	ambientlight;
	long	directorysize; // aka chunk count
	//DWORD	exportparams;
	//DWORD	flags;
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
	RMDChunk	chunktype; // it will store type of chunks, like mesh material
	long		count;
	long		*offsets;
};

class RMDMesh 
{
public:
	RMDMesh();

	char	objname[64];
	
	long	matid;
	long	vertexcount;
	long	facecount;
	long	tvcount;
	long	normalcount;
	bool	hasphysique;

	float	center[3]; // x ,y,z of center
	float	boundingbox[6]; // x1,y1,z1, x2,y2,z2

	float*	vexs; // vertex * 3 x,y,z,x,y,z,x,y,z

	int*	faces; // face * 4 v1,v2,v3,mat,v1,v2,v3,mat,v1,v2,v3,mat
	/*int*	v1;
	int*	v2;
	int*	v3;
	int*	mat;*/

	float*	texcords; // tcord * 2 tu,tv,tu,tv
	//float*	tv;
	
	int*	tfaces; // tface * 3 t1,t2,t3,t1,t2,t3
	/*int*	t1;
	int*	t2;
	int*	t3;*/
	
	float*	normals; // normalcount *3  nx,ny,nz,nx,ny,nz
	/*float*	nx;
	float*	ny;
	float*	nz;*/


	//long*	boneid;
	RMDWeightedVertex* boneinfo;
};

class RMDLight 
{
public:
	RMDLight();
	char	lightname[64];
	long	lighttype;
	float	lightpos[3]; // lights position
	float	targetpos[3]; // targets position
	float	intensity;
	float	atten[4];
	byte	clr[3]; // color
};

class RMDCamera
{
public:
	RMDCamera();
	char cameraname[64];
	float camera[3]; // camera position
	float target[3]; // target position
};

class RMDMaterial // export simple materials
{
public:
	RMDMaterial();
	char		matname[64];
	long		flags;
	farray		submtls;
};

class RMDBoneKeyFrame
{
public:
	float pos[3]; // vector position
	float rot[3]; // euler rotation
	float mat[12];
	int time;
};

class RMDBone
{
public:
	RMDBone();
	int boneid;
	char bonename[128];
	char boneparent[128];
	int bonenamehash;
	//int parentid;
	int framec; // key frame count
	//RMDBoneKeyFrame m_init;
	float m_init[12];

	RMDBoneKeyFrame *kframes; // key frames array

	RMDSkeleton* sklt;
	RMDBone* parent;
	//farray childs;
};

class RMDWeightedVertex
{
public:
	RMDWeightedVertex();

	int ibc; // influenced bone count
	int* boneids;
	float* weights;
	
	int curbone;
	void PrepareMemory(int ibonecount);
	void AddBone(int boneid,float weight);
	void Normalize();
};

class RMDSkeleton
{
public:
	bool baseskeleton;
	int keystart;
	int keyend;
	int framerate;

	int bonecount;
	farray bones;

	RMDBone* GetBoneFromName(char* bonename);
	void FixBoneConnections();
};



TCHAR *GetString(int id);
extern ClassDesc* GetSceneExportDesc();
extern HINSTANCE g_hInstance;


class MyExporter : public SceneExport 
{
public:
	// + general				********************
    bool			m_exportSelected;
    bool			m_suppressPrompts;
	DWORD			exportmode;
	long			elaptimer;
	HWND*			DebugDlgs;

    Interface*		m_ip;
    ExpInterface*	m_expip;
    FILE*			m_fileStream;

	int				skeletonexportmode; // 0 = pose, 1 = anim

	RMDHeader		fileheader;
	farray			om;	// array for meshes
	farray			ol;	// array for lights
	farray			oc; // array for cameras
	farray			ot; // array for textures
	farray			od; // array for directorys
	RMDSkeleton		os; // skeleton (currently its not an array)
	/*RMDMesh		*om;
	RMDLight		*ol;
	RMDCamera		*oc;
	RMDTexture		*ot;*/

	// + exp_general.cpp
	void			StartExporting();
	void			PrepareHeader();
	void			EnumerateNodes();
	RMDDirectory*	GetDirectoryByType(RMDChunk chktype);

	// + exp_physique.cpp
	Modifier*		FindPhysiqueModifier (INode* nodePtr);
	void			ExportPhysiqueData(TimeValue t, ObjectState *os, INode *node,RMDMesh* msh);

	// + exp_mesh.cpp
	void			ExportGeomObject(INode * node);
	Point3			GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);

	// + exp_camera.cpp
	void			ExportCamera(INode * node);

	// + exp_light.cpp
	void			ExportLight(INode * node);

	// + exp_material.cpp
	long			ExportMaterial(Mtl*	matnode);
	void			ColorToByteArray(Color cp, byte* bp);
	void			LoadMaterialData(RMDMaterial* rmdm,Mtl* maxm);

	// + exp_bone.cpp
	void			ExportSkeletalPose();
	void			ExportSkeletalAnimation();
	bool			skeletalEnum(INode* node);
	void			GetBoneCount();
	void			BoneCounter(INode* node);
	void			ExportBone(INode* node, Control* c);
	void			MatrixToBoneKeyframe(INode* node,RMDBoneKeyFrame* kfm,TimeValue t);
	float			ReduceRotation(float fl);
	void			MatrixToFloat(INode* node,RMDBoneKeyFrame* kfm,TimeValue t);


	// + exp_write.cpp
	void			CreateDirectoryStructure();
	void			WriteToFile();
	void			WriteHeader(FILE* fp,RMDHeader* hdr);
	void			WriteDirectory(FILE* fp,RMDDirectory* rdr);
	void			WriteMesh(FILE* fp,RMDMesh* msh);
	void			WriteCamera(FILE* fp,RMDCamera* cam);
	void			WriteLight(FILE* fp, RMDLight* lig);
	void			WriteMaterial(FILE* fp,RMDMaterial* mt);
	void			WriteBoneKeyframe(FILE* fp,RMDBoneKeyFrame* kf);
	void			WriteBaseSkeleton(FILE* fp,RMDSkeleton* sk);
	void			WriteBaseBone(FILE* fp,RMDBone* bn);
	void			WriteAnimSkeleton(FILE* fp,RMDSkeleton* sk);
	void			WriteAnimBone(FILE* fp,RMDBone* bn);
	void			WriteVertexLink(FILE* fp,RMDWeightedVertex* vtx);

	// + tools.cpp
	void			MxToM(Matrix3 mat,float m[12]);
	Matrix3			Mat2GL(const Matrix3& _mat);
	Point3			Ver2GL(const Point3& _p);
	void			AddDbg(char* fmt,...);
	Point3			crossp( const Point3 &v1,  const Point3 &v2 );
	float			dotp( const Point3 &v1,  const Point3 &v2 );
	long			shash(char *key);

	// + exporter.cpp
    MyExporter();
    ~MyExporter();
    int				ExtCount();
    const TCHAR *	Ext(int n);
    const TCHAR *	LongDesc();
    const TCHAR *	ShortDesc();
    const TCHAR *	AuthorName();
    const TCHAR *	CopyrightMessage();
    const TCHAR *	OtherMessage1();
    const TCHAR *	OtherMessage2();
    unsigned int	Version();
    void			ShowAbout(HWND hWnd);
    int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
    BOOL			SupportsOptions(int ext, DWORD options);

};

#endif