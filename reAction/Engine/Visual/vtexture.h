
// reAction Texture Processor
// version 0.1 beta

#ifndef VTEXTURE_H
#define VTEXTURE_H

#include "../../StdAfx.h"
#include "vcolor.h"

enum
{
	RTXF_BASE_MAP		= 0x00000001,
	RTXF_BUMP_MAP		= 0x00000002,
	RTXF_HEIGHT_MAP		= 0x00000004,
	RTXF_TANGENT_SPACE	= 0x00000008,
	RTXF_NORMAL_MAP		= 0x00000010
};

struct RTRHeader 
{
	char	signature[4];	// "RTR\0"
	byte	sp_width;		// sub part width
	byte	sp_height;		// sub part height
	byte	loss;			// loss sublevel
	long	partcount;		// total part count
};

struct RTXHeader 
{
	char	signature[4];
	long	id;
	byte	version[2];
	word	width;
	word	height;
	byte	bits;

	word	mipmaps;
	word	frames;
	word	faces;
	word	zdepth;

	long	flags;

};


// Pack Classes

class VPackNode
{
public:
	VPackNode();

	long x,y;
	long width;
	long height;

	bool				leftempty;
	bool				rightempty;

	VPackNode			*left;
	VPackNode			*right;

	VPackNode*			InsertTriangle(long width,long height,bool *side); // returns addrss
	VPackNode*			InsertQuad(long width,long height);
};


// todo: add texture mapping functions
// 1- give pixel for given texture coordinates and 3d triangle and a point on the triangle
enum tformat
{
	t_null,
	t_inmem,
	t_bmp,
	t_rtx,
	t_tex
};
// Image X and Y starts from 1
class VTexture // texture is editable in memory bitmap
{
public:
	VTexture();
	~VTexture();
	byte*	bytes;	// image data will stored here
	long	bytsize;	// size of bytes
	long	pcount;		// pixel count
	long	bpp;	// bytes per pixel
	
	word	twidth;		// texture width
	word	theight;	// texture height
	int		type;		// bits per pixel 32 or 24 but GL_RGBA or GL_RGB
	dword	glID;

	tformat	textureformat;
	char*	texturepath;
	bool	changed;
	long	texturepathhash;

	bool	draw_wrap;

	VTexture* nexttex;
	VTexture* prevtex;

	// starting
	void createbuffer		(word width,word height,int typ);	// creates buffer
	void freebuffer			(); // deletes data from buffer sets everything to zero
	void convertbuffer		(int typ);

	// file i/o
	void writebmp			(char* path);	// writes bmp
	void loadbmp			(char* path,bool txid = false,long strt=0);	// loads from bmp
	void loadtex			(char* path,bool txid = false,long strt=0);
	void screenshot			(char* path);

	// drawing
	void setpixel			(word x,word y,long pClr);	// pClr maybe VColor24Bit or VColor32Bit pointer
	long getpixel			(word x,word y);
	void addpixel			(word x,word y,long pClr);
	void drawline			(int x,int y,int x2,int y2,long pClr);
	void wideline			(word x,word y,word x2,word y2,byte thick,long pClr);
	void perp				(word x, word y, word x2, word y2, byte thick,long pClr);//perpendicular
	void clear				(long pClr);
	void filltriangle		(int x1,int y1,int x2,int y2,int x3,int y3,long pClr);
	void drawcircle			(word x,word y,word radius,long pClr);
	void filltriangle2		(float* vertices,long pClr);

	// basic commands
	void subimage			(VTexture* src,word fx,word fy,word tx,word ty,word w,word h); // from x,y and to x,y then width,height.
	void tileimage			(VTexture* src,word fx,word fy,word fw,word fh,word tx,word ty,word tw,word th);
	void matrixmul			(float m[],word mw,word mh,bool mabs = true);	// matrix multiplicion
	void monochrome			();												// converts image to monochrome
	void blend				(VTexture* dest,float tween = 0.5f);
	void add				(VTexture* dest);
	void gettexid			(int minfilter = GL_LINEAR_MIPMAP_LINEAR,int magfilter = GL_LINEAR_MIPMAP_LINEAR);	// get texture id
	void deltexid			(); // delete texture id glDeleteTextures (1, &uiGLID);
	void updatetex			(bool updatemipmaps = false);	
	void resize				(word dwidth,word dheight);
	void rotate				(word cx,word cy,word angle,bool rsz=true);
	void colorkey			(long clr,byte alpha = 0); // converts clr to alpha
	void monoalpha			(); // converts monochrome version of image to alpha channel
	void combinealpha		(VTexture* alpha,bool spoil=true); // converts alpha's image to monochrome then loads as alpha of itself

	// matrix presets
	void sharpen			();
	void emboss				();
	void edged				();
	void blur				();

	// utilities
	void histogram			();															// take histogram
	void invert				();
	void lineargradient		(word sx,word sy,word ex,word ey,long sclr,long eclr);		// linear gradient
	void radialgradient		(word sx,word sy,word radius,long sclr,long eclr);
	void reflectedgradient	(word sx,word sy,word ex,word ey,long sclr,long eclr);
	void cellular			(int pc,long sclr,int style,bool ast);
	void perlinnoise		(float z,float scal=16.0f);	
};

class VTCombination // rtx - reaction texture combination
{
public:
	RTXHeader			myheader;
	VTexture*			lotsoftex;

	void LoadRTX		(char* path);
	void SaveRTX		(char* path);
	
	void AddTexture		(VTexture* tex,word mipmap=0,word frame=0,word face=0,word zslice=0,long vers=1);
	long GetTexture		(word mipmap=0,word frame=0,word face=0,word zslice=0,long vers=1);
};

class VTReCombination
{
public:
	RTRHeader myheader;

	void		LoadRTR			(char* rtrfile);	// opens rtr file loads headers
	void		CreateRTR		(char* rtrfile);	// creates first and last rtr file from header information

	void		DivideTexture	(VTexture* vtex);	// divides texture and gives integrates with RTR
	VTexture*	ReBuildTexture	(char* file,long stp=0); // stp = start pointer
};

// elimine etmek için soyle bisey olabilir
// her klasor adindan 0-32 arasi bir rakam uretilir
// boyle 3 derinlige kadar index cikarilabilir
// yada isimden hash alcan
// hasha gore siralican
// hashin ilk bytesine gore indexlicen

class VTextureMemory
{
public:
	VTextureMemory();
	VTexture*	firsttex;
	VTexture*	lasttex;
	int			texturecount;


	void AddTexture(VTexture* tx);
	void RemoveTexture(VTexture* tx);

	VTexture* GetTextureByPath(char* tpath,VTexture* start = 0);
};

extern VTextureMemory mTextures;

// make these function check from memory for already loaded textures
VTexture* LoadBMP(char* fpath);
VTexture* LoadTEX(char* fpath);

#endif