
#include "StdAfx.h"
#include "vtexture.h"
#include "../Tools/rtools.h"
#include "../tools/rfilesystem.h"

using namespace RTools;

#define SIZE 256
#define MASK 0xFF

VTextureMemory mTextures;

VTextureMemory::VTextureMemory()
{
	firsttex = 0;
	lasttex = 0;
	texturecount = 0;
}

void VTextureMemory::AddTexture(VTexture* tx)
{
	if (firsttex == 0)
	{
		firsttex = tx;
		lasttex = tx;
		tx->nexttex = 0;
		tx->prevtex = 0;
		texturecount++;
	}
	else
	{
		lasttex->nexttex = tx;
		tx->prevtex = lasttex;
		tx->nexttex = 0;
		lasttex = tx;
		texturecount++;
	}
}

void VTextureMemory::RemoveTexture(VTexture* tx)
{
	VTexture *nx,*px;
	nx = tx->nexttex;
	px = tx->prevtex;

	if (px)
	{
		px->nexttex = nx;
	}
	else
	{
		firsttex = nx;
	}

	if (nx)
	{
		nx->prevtex = px;
	}
	else
	{
		lasttex = px;
	}

	tx->nexttex = 0;
	tx->prevtex = 0;
	
	texturecount--;
}

VTexture* VTextureMemory::GetTextureByPath(char* tpath,VTexture* start /* = 0 */)
{
	VTexture* nx = firsttex;
	long hs = shash(tpath);
	if (start)
	{
		nx = start;
	}

	while (nx)
	{
		if (nx->texturepathhash == hs)
		{
			if (strcmp(nx->texturepath,tpath) == 0)
			{
				return nx;
			}
		}
		nx = nx->nexttex;
	}
	return 0;
}

class VPerlinNoise
{
private:	
	float Mtx[9];
	BYTE p[SIZE];
	float gx[SIZE];
	float gy[SIZE];
	float gz[SIZE];
	float Axis[3];
	void Initialize(UINT nSeed);	
	void InitRotationMatrix(const float *pAxis, float r);
	float Noise1(float x);
	float Noise2(float x, float y);
	float Noise3(float x, float y, float z);
public:
	VPerlinNoise();
	void MakeNoise(VTexture *tex,float z,float scal);
};

VPerlinNoise::VPerlinNoise()
{
	Axis[0] = 0.16f;
	Axis[1] = 0.67f;
	Axis[2] = 0.43f;
}

void VPerlinNoise::MakeNoise(VTexture *tex,float z,float scal)
{
		Initialize(GetTickCount());
	InitRotationMatrix(Axis, 0.34521f);

	for( int iy = 0; iy < tex->theight; iy++ )
	{
		// Compute the starting position from the y and z coordinate
		float y = iy/scal;
		float p[3] = { y*Mtx[1] + z*Mtx[2], 
			           y*Mtx[4] + z*Mtx[5], 
					   y*Mtx[7] + z*Mtx[8] };

		// This represents movements along the x axis
		float x = 1/scal;
		float d[3] = { x*Mtx[0], x*Mtx[3], x*Mtx[6] };

		for( int ix = 0; ix < tex->twidth; ix++ )
		{
			BYTE n = BYTE(255*0.5f*(Noise3(p[0], p[1], p[2]) + 1));

			tex->setpixel(ix,iy,RGB(n,n,n));
			//map[ix+iy*256] = (n<<16) | (n<<8) | n;

			p[0] += d[0];
			p[1] += d[1];
			p[2] += d[2];
		}
	}
}

void VPerlinNoise::InitRotationMatrix(const float *pAxis, float r)
{
  // The axis vector must be of unit length
  float x, y, z, m;
  m = sqrtf(pAxis[0]*pAxis[0] + pAxis[1]*pAxis[1] + pAxis[2]*pAxis[2]);
  x = pAxis[0]/m;
  y = pAxis[1]/m;
  z = pAxis[2]/m;

  // Compute the rotation matrix  
  float c = (float)cos(r);
  float s = (float)sin(r);

  Mtx[0] = (x * x) * (1.0f - c) + c;
  Mtx[1] = (y * x) * (1.0f - c) + (z * s);
  Mtx[2] = (z * x) * (1.0f - c) - (y * s);

  Mtx[3] = (x * y) * (1.0f - c) - (z * s);
  Mtx[4] = (y * y) * (1.0f - c) + c;
  Mtx[5] = (z * y) * (1.0f - c) + (x * s);

  Mtx[6] = (x * z) * (1.0f - c) + (y * s);
  Mtx[7] = (y * z) * (1.0f - c) - (x * s);
  Mtx[8] = (z * z) * (1.0f - c) + c;
} 



//=============================================================================

//=============================================================================
void VPerlinNoise::Initialize(UINT nSeed)
{
	int i, j, nSwap;

	srand(nSeed);

	// Initialize the permutation table
	for(i = 0; i < SIZE; i++)
		p[i] = i;

	for(i = 0; i < SIZE; i++)
	{
		j = rand() & MASK;

		nSwap = p[i];
		p[i]  = p[j];
		p[j]  = nSwap;
	}

	// Generate the gradient lookup tables
	for(i = 0; i < SIZE; i++)
	{
		// Ken Perlin proposes that the gradients are taken from the unit 
		// circle/sphere for 2D/3D, but there are no noticable difference 
		// between that and what I'm doing here. For the sake of generality 
		// I will not do that.

		gx[i] = float(rand())/(RAND_MAX/2) - 1.0f; 
		gy[i] = float(rand())/(RAND_MAX/2) - 1.0f;
		gz[i] = float(rand())/(RAND_MAX/2) - 1.0f;
	}
}

//=============================================================================
float VPerlinNoise::Noise1(float x)
{
	// Compute what gradients to use
	int qx0 = (int)floorf(x);
	int qx1 = qx0 + 1;
	float tx0 = x - (float)qx0;
	float tx1 = tx0 - 1;

	// Make sure we don't come outside the lookup table
	qx0 = qx0 & MASK;
	qx1 = qx1 & MASK;

	// Compute the dotproduct between the vectors and the gradients
	float v0 = gx[qx0]*tx0;
	float v1 = gx[qx1]*tx1;

	// Modulate with the weight function
	float wx = (3 - 2*tx0)*tx0*tx0;
	float v = v0 - wx*(v0 - v1);

	return v;
}

//=============================================================================
float VPerlinNoise::Noise2(float x, float y)
{
	// Compute what gradients to use
	int qx0 = (int)floorf(x);
	int qx1 = qx0 + 1;
	float tx0 = x - (float)qx0;
	float tx1 = tx0 - 1;

	int qy0 = (int)floorf(y);
	int qy1 = qy0 + 1;
	float ty0 = y - (float)qy0;
	float ty1 = ty0 - 1;

	// Make sure we don't come outside the lookup table
	qx0 = qx0 & MASK;
	qx1 = qx1 & MASK;

	qy0 = qy0 & MASK;
	qy1 = qy1 & MASK;

	// Permutate values to get pseudo randomly chosen gradients
	int q00 = p[(qy0 + p[qx0]) & MASK];
	int q01 = p[(qy0 + p[qx1]) & MASK];

	int q10 = p[(qy1 + p[qx0]) & MASK];
	int q11 = p[(qy1 + p[qx1]) & MASK];

	// Compute the dotproduct between the vectors and the gradients
	float v00 = gx[q00]*tx0 + gy[q00]*ty0;
	float v01 = gx[q01]*tx1 + gy[q01]*ty0;

	float v10 = gx[q10]*tx0 + gy[q10]*ty1;
	float v11 = gx[q11]*tx1 + gy[q11]*ty1;

	// Modulate with the weight function
	float wx = (3 - 2*tx0)*tx0*tx0;
	float v0 = v00 - wx*(v00 - v01);
	float v1 = v10 - wx*(v10 - v11);

	float wy = (3 - 2*ty0)*ty0*ty0;
	float v = v0 - wy*(v0 - v1);

	return v;
}

//=============================================================================
float VPerlinNoise::Noise3(float x, float y, float z)
{
	// Compute what gradients to use
	int qx0 = (int)floorf(x);
	int qx1 = qx0 + 1;
	float tx0 = x - (float)qx0;
	float tx1 = tx0 - 1;

	int qy0 = (int)floorf(y);
	int qy1 = qy0 + 1;
	float ty0 = y - (float)qy0;
	float ty1 = ty0 - 1;

	int qz0 = (int)floorf(z);
	int qz1 = qz0 + 1;
	float tz0 = z - (float)qz0;
	float tz1 = tz0 - 1;

	// Make sure we don't come outside the lookup table
	qx0 = qx0 & MASK;
	qx1 = qx1 & MASK;

	qy0 = qy0 & MASK;
	qy1 = qy1 & MASK;

	qz0 = qz0 & MASK;
	qz1 = qz1 & MASK;

	// Permutate values to get pseudo randomly chosen gradients
	int q000 = p[(qz0 + p[(qy0 + p[qx0]) & MASK]) & MASK];
	int q001 = p[(qz0 + p[(qy0 + p[qx1]) & MASK]) & MASK];

	int q010 = p[(qz0 + p[(qy1 + p[qx0]) & MASK]) & MASK];
	int q011 = p[(qz0 + p[(qy1 + p[qx1]) & MASK]) & MASK];

	int q100 = p[(qz1 + p[(qy0 + p[qx0]) & MASK]) & MASK];
	int q101 = p[(qz1 + p[(qy0 + p[qx1]) & MASK]) & MASK];

	int q110 = p[(qz1 + p[(qy1 + p[qx0]) & MASK]) & MASK];
	int q111 = p[(qz1 + p[(qy1 + p[qx1]) & MASK]) & MASK];

	// Compute the dotproduct between the vectors and the gradients
	float v000 = gx[q000]*tx0 + gy[q000]*ty0 + gz[q000]*tz0;
	float v001 = gx[q001]*tx1 + gy[q001]*ty0 + gz[q001]*tz0;  

	float v010 = gx[q010]*tx0 + gy[q010]*ty1 + gz[q010]*tz0;
	float v011 = gx[q011]*tx1 + gy[q011]*ty1 + gz[q011]*tz0;

	float v100 = gx[q100]*tx0 + gy[q100]*ty0 + gz[q100]*tz1;
	float v101 = gx[q101]*tx1 + gy[q101]*ty0 + gz[q101]*tz1;  

	float v110 = gx[q110]*tx0 + gy[q110]*ty1 + gz[q110]*tz1;
	float v111 = gx[q111]*tx1 + gy[q111]*ty1 + gz[q111]*tz1;

	// Modulate with the weight function
	float wx = (3 - 2*tx0)*tx0*tx0;
	float v00 = v000 - wx*(v000 - v001);
	float v01 = v010 - wx*(v010 - v011);
	float v10 = v100 - wx*(v100 - v101);
	float v11 = v110 - wx*(v110 - v111);

	float wy = (3 - 2*ty0)*ty0*ty0;
	float v0 = v00 - wy*(v00 - v01);
	float v1 = v10 - wy*(v10 - v11);

	float wz = (3 - 2*tz0)*tz0*tz0;
	float v = v0 - wz*(v0 - v1);

	return v;
}

VTexture::VTexture()
{
	bytes = 0;
	bytsize = 0;
	twidth = 0;
	theight = 0;
	type = 0;
	glID = 0;
	pcount = 0;
	bpp = 0;
	texturepath = 0;
	texturepathhash = 0;
	changed = false;
	draw_wrap = false;
	textureformat = t_null;
	mTextures.AddTexture(this);
}

VTexture::~VTexture()
{
	freebuffer();
	mTextures.RemoveTexture(this);
}

void VTexture::createbuffer(word width,word height,int typ)
{
	bpp = 3;
	if (typ == GL_RGBA) bpp = 4;
	bytsize = width * height * bpp; // rgba
	twidth = width;
	theight = height;
	type = typ;
	bytes = new byte [bytsize];
	pcount = width * height;

	memset(bytes,'\0',bytsize); // make it black
	textureformat = t_inmem;

	//memset(bytes,'\xFF',bytsize); // make it white
}

void VTexture::updatetex(bool updatemipmaps)
{
	glBindTexture(GL_TEXTURE_2D,glID);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,twidth,theight,type,GL_UNSIGNED_BYTE,bytes);
	if (updatemipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, type, twidth, theight,type, GL_UNSIGNED_BYTE, bytes);
	}
}

void VTexture::freebuffer()
{
	if ( bytes != NULL)
	{
		delete [] bytes;
	}
	bytes = 0;
	bytsize = 0;
	twidth = 0;
	theight = 0;
	type = 0;
	glID = 0;
	draw_wrap = false;
	if (texturepath)
	{
		delete [] texturepath;
		texturepath = 0;
	}
	changed = false;
	texturepathhash = 0;
	textureformat = t_null;
}

void VTexture::deltexid()
{
	if ( glID != NULL)
	{
		glDeleteTextures(1,&glID);
	}
}

long VTexture::getpixel(word x,word y)
{
	byte* pBase;
	long retq;

	if (x>=twidth) x = twidth-1;
	if (y>=theight) y = theight-1;
	
	pBase = bpp*((y*twidth) + x) + bytes;

	if (type == GL_RGB)
	{
		_asm
		{
			mov ebx,dword ptr [pBase]
			mov eax,dword ptr [ebx]
			and eax,0x00FFFFFF
			//shl eax,8
			//shr eax,8
			mov retq,eax

		}
	}
	else
	{
		_asm
		{
			mov ebx,dword ptr [pBase]
			mov eax,dword ptr [ebx]
			mov retq,eax
		}
	}
	return retq;
}

void VTexture::setpixel(word x,word y,long pClr)
{
	byte *pBase;
	if ( draw_wrap )
	{
		x = x % twidth;
		y = y % theight;
	}
	else
	{
		if (x>twidth-1) x = twidth-1;
		if (y>theight-1) y = theight-1;
	}
	pBase = bpp*((y*twidth) + x) + bytes;

	if (type == GL_RGB)
	{
		_asm
		{
			mov eax,pClr
			mov ebx,pBase
			mov byte ptr [ebx],al
			shr eax,8
			inc ebx
			mov byte ptr [ebx],al
			inc ebx
			mov byte ptr [ebx],ah
		}
	}
	else
	{
		_asm
		{
			mov ebx,pClr
			mov eax,pBase	
			mov dword ptr [eax],ebx
		}
	}
	changed = true;
}

void VTexture::addpixel(word x,word y,long pClr)
{
	VColor24Bit tpix;
	VColor24Bit tpi2;
	tpix = getpixel(x,y);
	tpi2 = pClr;
	short a,b,c;
	a = tpix.r + tpi2.r;
	b = tpix.g + tpi2.g;
	c = tpix.b + tpi2.b;
	a > 255 ? tpix.r = 255 : tpix.r = (byte)a ;
	b > 255 ? tpix.g = 255 : tpix.g = (byte)b ;
	c > 255 ? tpix.b = 255 : tpix.b = (byte)c ;
	setpixel(x,y,tpix.RGBOut());
}

void VTexture::drawline(int x,int y,int x2,int y2,long color)
{
	//int pix = color.getRGB();
	int dy = y2 - y;
	int dx = x2 - x;
	int stepx, stepy;
	
	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;                                                  // dy is now 2*dy
	dx <<= 1;                                                  // dx is now 2*dx
	
	setpixel(x,y,color);
	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
		while (x != x2) 
		{
			if (fraction >= 0) 
			{
				y += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
			x += stepx;
			fraction += dy;                                    // same as fraction -= 2*dy
			setpixel(x,y,color);
		}
	} 
	else
	{
		int fraction = dx - (dy >> 1);
		while (y != y2) 
		{
			if (fraction >= 0) 
			{
				x += stepx;
				fraction -= dy;
			}
			y += stepy;
			fraction += dx;
			setpixel(x,y,color);
		}
	}
}

void VTexture::perp(word x, word y, word x2, word y2, byte thick,long pClr)
{
    int sx = (x2 > 0)? 1: -1;
    int sy = (y2 > 0)? 1: -1;
    int ax = 2*sx*x2;
    int ay = 2*sy*y2;
    //int x = x;
    //int y = y;
    int T = thick;

    if (ax > ay) {
        int d = ay - ax/2;
        while (1) {
            if (T-- <= 0) return;
            if (d >= 0) {
                y += sy;
                setpixel(x,y,pClr);
                d -= ax;
            }
            x += sx;
            d += ay;
            setpixel(x,y,pClr);
        }
    }
    else {
        int d = ax - ay/2;
        while (1) {
            if (T-- <= 0) return;
            if (d >= 0) {
                x += sx;
                setpixel(x,y,pClr);
                d -= ay;
            }
            y += sy;
            d += ax;
            setpixel(x,y,pClr);
        }
    }	
}

void VTexture::wideline(word x,word y,word x2,word y2,byte thick,long pClr)
{
	int dx = x2-x;
    int dy = y2-y;
    int sx = (dx > 0)? 1: -1;
    int sy = (dy > 0)? 1: -1;
    int ax = 2*sx*dx;
    int ay = 2*sy*dy;
//    int x = x;
//    int y = y;
	
    if (ax > ay) 
	{
        int d = ay - ax/2;
        while (1) 
		{
            setpixel(x,y,pClr);
            perp(x,y,dy,-dx, thick/2,pClr);
            perp(x,y,-dy,dx, thick-thick/2-1,pClr);
            if (x == x2)  return;
            if (d >= 0) 
			{
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    }
    else {
        int d = ax - ay/2;
        while (1) 
		{
            setpixel(x,y,pClr);
            perp(x,y,dy,-dx, thick/2,pClr);
            perp(x,y,-dy,dx, thick-thick/2-1,pClr);
            if (y == y2)  return;
            if (d >= 0) 
			{
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

void VTexture::clear(long pClr)
{
	byte *pDest;
	long ln;
	pDest = bytes;
	ln = pcount;
	if ( type == GL_RGB)
	{
		_asm
		{
			mov ecx,ln // for loop
			mov edi,pDest
			mov ebx,pClr
			mov eax,ebx
			shr ebx,16

		l1: mov byte ptr [edi],al
			inc edi
			mov byte ptr [edi],ah
			inc edi
			mov byte ptr [edi],bl
			inc edi
			dec ecx
			jnz l1
		}
	}
	else
	{
		_asm
		{
			mov ecx,ln
			mov edi,pDest
			mov eax,pClr

		l2: mov dword ptr [edi],eax
			add edi,4
			dec ecx
			jnz l2
		}
	}

}

void VTexture::loadtex(char* path,bool txid,long strt/* =0 */)
{
	RFile* tex = ROpenFile(path,"rb");
	char a;
	int w,h;

	tex->getdata(&w,4,1);
	tex->getdata(&h,4,1);
	tex->getdata(&a,1,1);

	freebuffer();
	createbuffer(w,h, a==1 ? GL_RGBA : GL_RGB);
	
	tex->getdata(bytes,1,bytsize);

	RCloseFile(tex);

	if (txid)
	{
		gettexid();
	}

	// set tex memory data
	textureformat = t_tex;
	changed = false;
	int sl = strlen(path);
	texturepath = new char [sl+1];
	strcpy(texturepath,path);
	texturepathhash = shash(texturepath);
	
}

void VTexture::loadbmp(char* path,bool txid,long strt/* =0 */)
{
	RFile *bmp;
	BITMAPINFOHEADER ih;
	BITMAPFILEHEADER fh;
	byte* arow;
	byte alig;
	byte* adr;
	long y;
	
	
	bmp = ROpenFile(path,"rb");
	bmp->setpos(strt,SEEK_SET);
	bmp->getdata(&fh,1,sizeof(BITMAPFILEHEADER));
	bmp->getdata(&ih,1,sizeof(BITMAPINFOHEADER));

	long row_wid = ih.biWidth * 3;

	freebuffer();
	createbuffer((word)ih.biWidth,(word)ih.biHeight,GL_RGB);
	
	arow = new byte [row_wid]; // row buffer for writing
	alig = row_wid % 4;
	
	for (y=0;y<theight;y++)
	{
		// algortihm: find adr, load rgb color, convert to bgr and put to arow
		adr = bytes + (twidth * 3 * (theight-y-1)); // reversed row
		bmp->getdata(arow,1,row_wid);
		if (alig != 0)
		{
			bmp->setpos(4-alig,SEEK_CUR);
		}
		_asm
		{
			mov esi,arow
			mov edi,adr
			mov ecx,ih.biWidth

		l1: mov al,byte ptr[esi] // B
			inc esi
			mov ah,byte ptr[esi] // G
			inc esi
			mov bl,byte ptr[esi] // R
			mov byte ptr [edi],bl // R
			inc edi
			mov byte ptr [edi],ah // G
			inc edi
			mov byte ptr [edi],al // B
			inc esi
			inc edi
			dec ecx
			jnz l1
		}
	}
	
	delete [] arow;

	RCloseFile(bmp);

	if (txid)
	{
		gettexid();
	}

	// set tex memory data
	textureformat = t_bmp;
	changed = false;
	int sl = strlen(path);
	texturepath = new char [sl+1];
	strcpy(texturepath,path);
	texturepathhash = shash(texturepath);
}

void VTexture::screenshot(char* path)
{
	//glBindTexture(GL_TEXTURE_2D,glID);
	RFile *bmp;
	BITMAPINFOHEADER ih;
	BITMAPFILEHEADER fh;
	int y;
	//byte* adr;
//	byte* arow;
	long mult = bpp;
	long zr = 0;
	byte alig;
	long row_wid;
	row_wid = twidth * 3;
	
	bmp = ROpenFile(path,"wb");

	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biPlanes = 1;
	ih.biBitCount = 24;
	ih.biCompression = 0;
	ih.biSizeImage = twidth * theight * mult;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
	ih.biClrUsed = 0;
	ih.biClrImportant = 0;
	ih.biWidth = twidth;
	ih.biHeight = theight;

	fh.bfSize = sizeof(BITMAPFILEHEADER);
	fh.bfType = 0x4D42;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmp->setdata(&fh,1,sizeof(BITMAPFILEHEADER));
	bmp->setdata(&ih,1,sizeof(BITMAPINFOHEADER));

	glReadPixels( 0, 0, twidth, theight, GL_BGR_EXT , GL_UNSIGNED_BYTE, bytes ); // GL_BGR

	//arow = new byte [row_wid]; // row buffer for writing
	alig = row_wid % 4;
	byte* adr = bytes;
	if (alig != 0)
	{
		for (y=0;y<theight;y++)
		{
			bmp->setdata(adr,1,row_wid);
			bmp->setdata(&zr,1,4 - alig);
			adr += row_wid;
		}
	}
	else
	{
		for (y=0;y<theight;y++)
		{
			bmp->setdata(adr,1,row_wid);
			adr += row_wid;
		}
	}

	RCloseFile(bmp);
}

void VTexture::writebmp(char* path)
{
	RFile *bmp;
	BITMAPINFOHEADER ih;
	BITMAPFILEHEADER fh;
	word y;
	byte* adr;
	byte* arow;
	long mult = bpp;
	long zr = 0;
	byte alig;
	long row_wid;
	row_wid = twidth * 3;
	
	bmp = ROpenFile(path,"wb");

	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biPlanes = 1;
	ih.biBitCount = 24;
	ih.biCompression = 0;
	ih.biSizeImage = twidth * theight * mult;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
	ih.biClrUsed = 0;
	ih.biClrImportant = 0;
	ih.biWidth = twidth;
	ih.biHeight = theight;

	fh.bfSize = sizeof(BITMAPFILEHEADER);
	fh.bfType = 0x4D42;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmp->setdata(&fh,1,sizeof(BITMAPFILEHEADER));
	bmp->setdata(&ih,1,sizeof(BITMAPINFOHEADER));

	arow = new byte [row_wid]; // row buffer for writing
	alig = row_wid % 4;
	
	for (y=0;y<theight;y++)
	{
		// algortihm: find adr, load rgb color, convert to bgr and put to arow
		adr = bytes + (twidth * mult * (theight-y-1)); // reversed row
		_asm
		{
			mov edx,mult
			sub edx,2

			mov esi,adr
			mov edi,arow
			mov ecx,ih.biWidth

		l1: mov al,byte ptr [esi]
			inc esi
			mov ah,byte ptr [esi]
			inc esi
			mov bl,byte ptr [esi]
			mov byte ptr [edi],bl
			inc edi
			mov byte ptr [edi],ah
			inc edi
			mov byte ptr [edi],al
			add esi,edx
			inc edi
			dec ecx
			jnz l1
			//loop l1
		}
		bmp->setdata(arow,1,row_wid);
		if (alig != 0)
		{
			bmp->setdata(&zr,1,4 - alig);
		}
	}
	
	delete [] arow;
	RCloseFile(bmp);
}

void VTexture::gettexid(int minfilter,int magfilter)
{
	glGenTextures(1, &glID);
   
	glBindTexture(GL_TEXTURE_2D, glID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, type, twidth, theight, 0, type, GL_UNSIGNED_BYTE, bytes);
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, twidth, theight,type, GL_UNSIGNED_BYTE, bytes);
}

void VTexture::matrixmul(float m[],word mw,word mh,bool mabs)
{
	int sumr=0;
	int sumg=0;
	int sumb=0;

	int i,j,k,l;
	int iix,iiy;
	int teas;

	VTexture tempb;
	tempb.createbuffer(twidth,theight,type);
//	int ast;
	VColor24Bit color;

//	mini = new byte [mw*mh*3];

	int suma=0;
	for (i = 0;i<mw*mh;i++)
	{
		suma+= (int)m[i];
	}
	if (suma == 0) suma = 1;
//	cout << suma;

	for (i=0;i<twidth;i++){
		for (j=0;j<theight;j++){
			for (k=1;k<mw+1;k++){
				for (l=1;l<mh+1;l++){
					iix = i + k-1;
					iiy = j + l-1;
					if (iix > twidth) iix = twidth-1;
					if (iiy > theight) iiy = theight-1;

					color = getpixel(iix,iiy);
					teas = ((k-1)*mw) + (l -1);
					sumr += color.r * (int)m[teas];
					sumg += color.g * (int)m[teas];
					sumb += color.b * (int)m[teas];
					//cout << m[teas] << endl;
	
				}
			}
			sumr /= suma;
			sumg /= suma;
			sumb /= suma;
			if (mabs)
			{
				sumr = abs(sumr);
				sumg = abs(sumg);
				sumb = abs(sumb);
			}
			else
			{
				if (sumr < 0) sumr = 0;
				if (sumg < 0) sumg = 0;
				if (sumb < 0) sumb = 0;
			}
			if (sumr > 255) sumr = 255;
			if (sumg > 255) sumg = 255;
			if (sumb > 255) sumb = 255;


			tempb.setpixel(i,j,RGB(sumr,sumg,sumb));
			sumr = sumg = sumb = 0;
		}
	}

	//delete [] bytes;
	memcpy(bytes,tempb.bytes,bytsize);


}

void VTexture::sharpen()
{
	float mymat [] = { 0,-1,0,-1,5,-1,0,-1,0};
	matrixmul(mymat,3,3,false);
}

void VTexture::emboss()
{
	float mymat [] = {3,0,0,0,-1,0,0,0,-1 }; // Emboss
	matrixmul(mymat,3,3,false);
}

void VTexture::edged()
{
	float mymat [] = {-1,-1,-1,0,0,0,1,1,1};
	matrixmul(mymat,3,3,false);
}

void VTexture::blur()
{
	float mymat [] = { 1,2,1,2,4,2,1,2,1};
	matrixmul(mymat,3,3,false);
}

void VTexture::monochrome()
{
	byte *pSrc;
	VColor24Bit a;
	int i;
	for (i=0,pSrc = bytes;i<pcount;pSrc+=bpp,i++)
	{
		a.LoadRGB(pSrc);
		a.Mono();
		a.WriteRGB(pSrc);
	}
}

void VTexture::monoalpha()
{
	byte *pSrc=bytes;
	VColor24Bit a;
	int i;
	byte *pDest=bytes;
	if (type == GL_RGB)
	{
		byte* byt2;
		byt2 = new byte [pcount*4];
		pDest = byt2;
		for (i=0;i<pcount;pSrc+=bpp,pDest+=4,i++)
		{
			a.LoadRGB(pSrc);
			a.WriteRGB(pDest);
			a.Mono();
			*(pDest+3) = a.r;
		}
		delete [] bytes;
		bytes = byt2;
		type = GL_RGBA;
		bpp = 4;
		bytsize = 4 * pcount;
	}
	else
	{
		for (i=0;i<pcount;pSrc+=bpp,i++)
		{
			a.LoadRGB(pSrc);
			a.Mono();
			*(pSrc+3) = a.r;
		}
	}
}

void VTexture::colorkey(long clr,byte alpha) 
{
	byte *pSrc=bytes;
	VColor24Bit a,ky;
	ky = clr;
	int i;
	byte *pDest=bytes;
	if (type == GL_RGB)
	{
		byte* byt2;
		byt2 = new byte [pcount*4];
		pDest = byt2;
		for (i=0;i<pcount;pSrc+=3,pDest+=4,i++)
		{
			a.LoadRGB(pSrc);
			a.WriteRGB(pDest);
			if ( ky == a)
			{
				*(pDest+3) = alpha;
			}
			else
			{
				*(pDest+3) = 255;
			}
			
		}
		delete [] bytes;
		bytes = byt2;
		byt2 = 0;	
		type = GL_RGBA;
		bpp = 4;
		bytsize = 4 * pcount;
	}
	else
	{
		for (i=0;i<pcount;pSrc+=4,i++)
		{
			a.LoadRGB(pSrc);
			a.WriteRGB(pDest);
			if ( ky == a)
			{
				*(pSrc+3) = alpha;
			}
			else
			{
				*(pSrc+3) = 255;
			}
		}
	}
}

void VTexture::blend(VTexture* dest,float tween /* = 0::5f */)
{
	//VColor24Bit src,dst;
	// implement resize code

}

void VTexture::invert()
{
	byte *pSrc;
	pSrc = bytes;

	if ( type == GL_RGB)
	{
		_asm
		{
			mov esi,pSrc
			mov ebx,[this]
			mov ecx,[ebx]this.pcount

		l1: mov eax,dword ptr [esi]
			not eax
			mov byte ptr [esi],al
			inc esi
			mov byte ptr [esi],ah
			inc esi
			shr eax,16
			mov byte ptr [esi],al
			inc esi
			dec ecx
			jnz l1
		}
	}
	else
	{
		_asm
		{
			mov esi,pSrc
			mov ebx,[this]
			mov ecx,[ebx]this.pcount

		l2: not dword ptr [esi]
			add esi,4
			dec ecx
			jnz l2
		}
	}


}


void VTexture::cellular(int pc,long sclr,int style,bool ast)
{
	int* xcord;
	int* ycord;
	float* distb;
	xcord = new int [pc];
	ycord = new int [pc];
	distb = new float [twidth * theight];
	float di;
	float tm=999999.0f;
	float tm2 = 999999.0f;

	float mindist = 999999.0f ;
	float maxdist = 0 ;
	int he;
	int x,y,i;
	float ntm;
	float la;
	int lcha;
	VColor24Bit pal,st;
	pal = sclr;

	for (i=0;i<pc;i++)
	{
		xcord[i] = (rand()/(int)(((unsigned)RAND_MAX ) / twidth));
		ycord[i] = (rand()/(int)(((unsigned)RAND_MAX ) / theight));
	}

	for (y=0;y<theight;y++)
	{
		for(x=0;x<twidth;x++)
		{
			for (i=0;i<pc;i++)
			{
				di = (float)sqrt( (xcord[i]-x)*(xcord[i]-x) + (ycord[i]-y)*(ycord[i]-y));
				if (di < tm){tm = di;lcha=i;}
			}
			if (style == 0) ntm = tm;
			if (style == 3) ntm = tm*tm;
			
			if (style == 1)
			{
				for (i=0;i<pc;i++)
				{
					di = (float)sqrt( (xcord[i]-x)*(xcord[i]-x) + (ycord[i]-y)*(ycord[i]-y));
					if (di <tm2 && i!=lcha) tm2 = di;
				}
				ntm = tm2 - tm;
			}
			if (style == 2)
			{
				for (i=0;i<pc;i++)
				{
					di = (float)sqrt( (xcord[i]-x)*(xcord[i]-x) + (ycord[i]-y)*(ycord[i]-y));
					if (di <tm2 && i!=lcha) tm2 = di;
				}
				ntm = tm2 * tm;
			}

			he = (y*theight) + x;			
			distb[he] = ntm;
			if (tm < mindist) mindist = ntm;
			if (tm > maxdist) maxdist = ntm;
			tm = 999999.0f;
			tm2 = 999999.0f;
		}
	}

	for (y=0;y<theight;y++)
	{
		for(x=0;x<twidth;x++)
		{
			he = (y*theight) + x;
			la =((distb[he]-mindist) / (maxdist-mindist));
			if (ast) la = 1-la;
			if (la > 1) la = 1;
			if (la < 0) la = 0;
			st.r = (byte)((float)pal.r * la);
			st.g = (byte)((float)pal.g * la);
			st.b = (byte)((float)pal.b * la);
			setpixel(x,y,st.RGBOut());
		}
	}
	delete [] xcord; xcord = 0;
	delete [] ycord; ycord = 0;
	delete [] distb; distb = 0;

}

void VTexture::resize( word dwidth,word dheight )
{
	if (dwidth == twidth && dheight == theight ) return; 
	float nwm,nhm; // new width multiplier, new height multiplier
	VColor24Bit p1,p2;
	nhm = (float)theight / (float)dheight;
	word x,y;
	float f;
	VTexture *tsrc=this;
	VTexture rw,rh;

	if ( dwidth != twidth)
	{
		// Adjust Width
		rw.createbuffer(dwidth,tsrc->theight,type);
		nwm = (float)tsrc->twidth / (float)dwidth;
		
		word Ff,Fc;
		for (y=0;y<tsrc->theight;y++)
		{
			for (x=0;x<dwidth;x++)
			{
				f = nwm * (float)x;
				Ff = (word)floor(f);
				Fc = Ff+1;
				p1 = tsrc->getpixel(Ff,y);
				p2 = tsrc->getpixel(Fc,y);
				p1.Blend(&p2,f-Ff);
				rw.setpixel(x,y,p1.RGBOut());
				
			}
		}
		tsrc = &rw;
	}
	if ( dheight != theight)
	{
		// Adjust Height
		rh.createbuffer(tsrc->twidth,dheight,type);
		nhm = (float)tsrc->theight / (float)dheight;
		word Ff,Fc;
		for (x=0;x<tsrc->twidth;x++)
		{		
			for (y=0;y<dheight;y++)
			{

				f = nhm * (float)y;
				Ff = (word)floor(f);
				Fc = Ff+1;
				p1 = tsrc->getpixel(x,Ff);
				p2 = tsrc->getpixel(x,Fc);
				p1.Blend(&p2,f-Ff);
				rh.setpixel(x,y,p1.RGBOut());
			}
		}
		tsrc = &rh;
	}
	bpp = tsrc->bpp;
	bytsize = tsrc->bytsize;
	twidth = tsrc->twidth;
	theight = tsrc->theight;
	type = tsrc->type;
	pcount = tsrc->pcount;
	byte *tmp;
	tmp = tsrc->bytes;
	// xchange buffer because
	// when ~VTexture() occurs it will destroy unnecessary one
	tsrc->bytes = bytes;
	bytes = tmp;
}

void VTexture::combinealpha( VTexture* alpha,bool spoil )
{
	alpha->monochrome();
	alpha->resize(twidth,theight);
	long i;
	byte *pSrc,*pDest;
	convertbuffer(GL_RGBA); // create alpha channel
	pSrc = alpha->bytes;
	pDest = bytes;	
	for (i=0;i<pcount;i++,pSrc+=alpha->bpp,pDest+=4)
	{
		*(pDest+3)=*pSrc; //put alpha channel r	
	}
	// Mission Accomplished
}

void VTexture::convertbuffer( int typ )
{
	int i;
	byte *pSrc;
	VColor24Bit tmp;
	if (type== GL_RGB && typ == GL_RGBA )
	{
		byte *pDest,*pD2;
		bytsize = twidth*theight*4;
		pDest = new byte [bytsize];
		pD2 = pDest;
		memset(pDest,'\0',bytsize); // put 0
		for (i=0,pSrc=bytes;i<pcount;pSrc+=3,i++,pDest+=4)
		{
			*pDest = *pSrc;
			*(pDest+1) = *(pSrc+1);
			*(pDest+2) = *(pSrc+2);
			//tmp.CopyColor(pSrc,pDest);	// Copy Colors
		}
		bpp = 4;
		delete [] bytes;
		bytes = pD2;
		type = GL_RGBA;
	}
	if (type == GL_RGBA && type == GL_RGB)
	{
		byte *pDest;
		bytsize = twidth*theight*3;
		pDest = new byte [bytsize];
		for (i=0,pSrc=bytes;i<pcount;pSrc+=4,i++,pDest+=3)
		{
			tmp.CopyColor(pSrc,pDest);	// Copy Colors
		}
		bpp = 3;
		delete [] bytes;
		bytes = pDest;
		type = GL_RGB;
	}
}

void VTexture::perlinnoise(float z,float scal)
{
	VPerlinNoise pte;
	pte.MakeNoise(this,z,scal);
}

void VTexture::tileimage(VTexture* src,word fx,word fy,word fw,word fh,word tx,word ty,word tw,word th)
{
	word dx,dy;
	word sx,sy;
	long clrs;

	dy = ty;
	while (dy < ty+th)
	{
		sy = fy;
		while(sy < fy+fh)
		{
			dx = tx;
			while (dx < tx + tw)
			{
				sx = fx;
				while(sx < fx+fw)
				{
					clrs = src->getpixel(sx,sy);
					setpixel(dx,dy,clrs);
					sx++;
					dx++;
					if (dx >= tx+tw)
					{
						break;
					}
				}	
			}
			sy++;
			dy++;
			if (dy>=ty+th)
			{
				break;
			}
		}
	}
}

void VTexture::subimage(VTexture* src,word fx,word fy,word tx,word ty,word w,word h)
{
	word sx,sy;
	word dx,dy;
	long clrs;
	VColor32Bit t32;
	VColor24Bit t24;

	for (sy=fy,dy=ty;sy<fy+h,dy<ty+h;sy++,dy++)
	{
		for (sx=fx,dx=tx;sx<fx+w,dx<tx+w;sx++,dx++)
		{
			clrs = src->getpixel(sx,sy);
			//clrd = getpixel(dx,dy);
			if (type == GL_RGB)
			{
				if (src->type == GL_RGB)
				{
					setpixel(dx,dy,clrs);
				}
				else
				{
					t32 = clrs;
					t24 = getpixel(dx,dy);
					t24.Blend(&t32);
					setpixel(dx,dy,t24.RGBOut());
				}
			}
			else
			{
				if (src->type == GL_RGB)
				{	
					//t32 = clrs;
					//t32.a = 255;
					clrs = clrs | (255<<24);
					setpixel(dx,dy,clrs );
					// TODO: IMPLEMENT THIS
				}
				else
				{
					// her ikiside alphali
					// TODO: FIX THIS BUG
					//t32 = clrs;
					/*t32d = getpixel(dx,dy);
					t32d.Blend(t32);*/
					//setpixel(dx,dy,t32d.RGBAOut());
					setpixel(dx,dy,clrs);
				}
			}
		}
	}
}

VPackNode::VPackNode()
{
	rightempty = true;
	leftempty = true;

	left = 0;
	right = 0;

	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

/*	Recursively Texture Packing Algorithm Modified for triangular packing
 *	Thanks to http://www.blackpawn.com/texts/lightmaps/default.html
 */
VPackNode* VPackNode::InsertTriangle(long wid,long heg,bool *side)
{
	VPackNode* newNode;
	if (left != 0 && right != 0) // we're not a leaf
	{
		newNode = left->InsertTriangle(wid,heg,side);
		if (newNode != NULL) return newNode;

		return right->InsertTriangle(wid,heg,side);
	}
	else // we are leaf
	{
		if (!rightempty && !leftempty) // this is not empty area
		{
			return NULL;
		}	

		if (wid > width || heg > height) // too big
		{
			return NULL;	
		}

		if (wid == width && heg == height ) // fits exactly
		{
			if (leftempty)
			{
				leftempty = false;
				*side = true; // left = true
				return this;
			}
			if (rightempty) 
			{
				rightempty = false;
				*side = false;
				return this;
			}
		}
		else
		{
			if ( !leftempty || !rightempty)
			{
				return NULL; // shit, some of them is not empty and texture is not equal to size
			}
		}

		// we gone split it to kids
		left = new VPackNode;
		right = new VPackNode;

		long dw,dh;
		dw = width - wid;
		dh = height - heg;

		if ( dw > dh)
		{
			left->x = x;
			left->y = y;
			left->width = wid;//-1;
			left->height = height;

			right->x = x+wid;
			right->y = y;
			right->width = width - wid;
			right->height = height;
		}
		else
		{
			left->x = x;
			left->y = y;
			left->width = width;
			left->height = heg;

			right->x = x;
			right->y = y+heg;
			right->width = width;
			right->height = height-heg;
		}

		return left->InsertTriangle(wid,heg,side);
	}
}

VPackNode* VPackNode::InsertQuad(long wid,long heg)
{
	VPackNode* newNode;
	if (left != 0 && right != 0) // we're not a leaf
	{
		newNode = left->InsertQuad(wid,heg);
		if (newNode != NULL) return newNode;
		
		return right->InsertQuad(wid,heg);
	}
	else // we are leaf
	{
		if (!leftempty && !leftempty) // this is not empty area
		{
			return NULL;
		}	
		
		if (wid > width || heg > height) // too big
		{
			return NULL;	
		}
		
		if (wid == width && heg == height ) // fits exactly
		{
			leftempty = false;
			rightempty = false;
			return this;
		}
		
		// we gone split it to kids
		left = new VPackNode;
		right = new VPackNode;
		
		long dw,dh;
		dw = width - wid;
		dh = height - heg;
		
		if ( dw > dh)
		{
			left->x = x;
			left->y = y;
			left->width = wid;//-1;
			left->height = height;
			
			right->x = x+wid;
			right->y = y;
			right->width = width - wid;
			right->height = height;
		}
		else
		{
			left->x = x;
			left->y = y;
			left->width = width;
			left->height = heg;
			
			right->x = x;
			right->y = y+heg;
			right->width = width;
			right->height = height-heg;
		}
		
		return left->InsertQuad(wid,heg);
	}
}


void VTexture::filltriangle(int x1,int y1,int x2,int y2,int x3,int y3,long pClr)
{
	int Ax,Ay,Bx,By,Cx,Cy;
	int Sx,Sy,Ex,Ey;


	if (y1 <= y2 && y1 <= y3) // t1 is the most smaller one
	{
		Ax = x1;
		Ay = y1;
		if (y2 <= y3)
		{
			Bx = x2;
			By = y2;
			Cx = x3;
			Cy = y3;
		}
		else
		{
			Bx = x3;
			By = y3;
			Cx = x2;
			Cy = y2;
		}
	}
	else
	{
		if (y2 <= y1 && y2 <= y3) // t2 is the most smaller one
		{
			Ax = x2;
			Ay = y2;
			if (y1 <= y3)
			{
				Bx = x1;
				By = y1;
				Cx = x3;
				Cy = y3;
			}
			else
			{
				Bx = x3;
				By = y3;
				Cx = x1;
				Cy = y1;
			}
		}
		else
		{
			if (y3 <= y1 && y3 <= y2) // t2 is the most smaller one
			{
				Ax = x3;
				Ay = y3;
				if (y1 <= y2)
				{
					Bx = x1;
					By = y1;
					Cx = x2;
					Cy = y2;
				}
				else
				{
					Bx = x2;
					By = y2;
					Cx = x1;
					Cy = y1;
				}
			}
		}
	}

	int dx1,dx2,dx3;
	if (By-Ay > 0) dx1=(Bx-Ax)/(By-Ay); else dx1=Bx - Ax;
	if (Cy-Ay > 0) dx2=(Cx-Ax)/(Cy-Ay); else dx2=0;
	if (Cy-By > 0) dx3=(Cx-Bx)/(Cy-By); else dx3=0;

	Sx = Ex = Ax;
	Sy = Ey = Ay;

	if(dx1 > dx2) {
		for(;Sy<=By;Sy++,Ey++,Sx+=dx2,Ex+=dx1)
			drawline(Sx,Sy,Ex,Sy,pClr);
		Ex=Bx;
		Ey=By;
		for(;Sy<=Cy;Sy++,Ey++,Sx+=dx2,Ex+=dx3)
			drawline(Sx,Sy,Ex,Sy,pClr);
	} else {
		for(;Sy<=By;Sy++,Ey++,Sx+=dx1,Ex+=dx2)
			drawline(Sx,Sy,Ex,Sy,pClr);
		Sx=Bx;
		Sy=By;
		for(;Sy<=Cy;Sy++,Ey++,Sx+=dx3,Ex+=dx2)
			drawline(Sx,Sy,Ex,Sy,pClr);
	}

}

int iround(float x)
{
	int t;
	
	__asm
	{
		fld  x
		fistp t
	}
	
	return t;
}

void VTexture::filltriangle2(float* vertices,long pClr)
{
	int X1 = iround(16.0f * vertices[0]);
	int Y1 = iround(16.0f * vertices[1]);

	int X2 = iround(16.0f * vertices[2]);
	int Y2 = iround(16.0f * vertices[3]);

	int X3 = iround(16.0f * vertices[4]);
	int Y3 = iround(16.0f * vertices[5]);

    int DX12 = X1 - X2;
    int DX23 = X2 - X3;
    int DX31 = X3 - X1;
	
    int DY12 = Y1 - Y2;
    int DY23 = Y2 - Y3;
    int DY31 = Y3 - Y1;

    int FDX12 = DX12 << 4;
    int FDX23 = DX23 << 4;
    int FDX31 = DX31 << 4;
	
    int FDY12 = DY12 << 4;
    int FDY23 = DY23 << 4;
    int FDY31 = DY31 << 4;

    int minx = (min(X1, min(X2, X3)) + 0xF) >> 4;
    int maxx = (max(X1, max(X2, X3)) + 0xF) >> 4;
    int miny = (min(Y1, min(Y2, Y3)) + 0xF) >> 4;
    int maxy = (max(Y1, max(Y2, Y3)) + 0xF) >> 4;

    int C1 = DY12 * X1 - DX12 * Y1;
    int C2 = DY23 * X2 - DX23 * Y2;
    int C3 = DY31 * X3 - DX31 * Y3;

    if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
    if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
    if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;
	
    int CY1 = C1 + DX12 * (miny << 4) - DY12 * (minx << 4);
    int CY2 = C2 + DX23 * (miny << 4) - DY23 * (minx << 4);
    int CY3 = C3 + DX31 * (miny << 4) - DY31 * (minx << 4);

    for(int y = miny; y < maxy; y++)
    {
        int CX1 = CY1;
        int CX2 = CY2;
        int CX3 = CY3;
		
        for(int x = minx; x < maxx; x++)
        {
            if(CX1 > 0 && CX2 > 0 && CX3 > 0)
            {
				setpixel(x,y,pClr);
			}
			
            CX1 -= FDY12;
            CX2 -= FDY23;
            CX3 -= FDY31;
        }
		
        CY1 += FDX12;
        CY2 += FDX23;
        CY3 += FDX31;
		

	}

}

/*void Rasterizer::triangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
    (char*&)colorBuffer += miny * stride;
	
    // Half-edge constants
	
    // Correct for fill convention
	
    }
}*/



void VTexture::drawcircle(word x,word y,word radius,long pClr)
{
	float aci;
	float nx,ny;
	int inx,iny;
	float rd;
	float radian;

	rd = 60.0f / radius;
	for (aci = 0.0f;aci < 360.0f ; aci+=rd)
	{
		radian = DEGTORAD(aci);
		nx = (float)x + (cos(radian) * radius);
		ny = (float)y + (sin(radian) * radius);
		inx = floor(nx);
		iny = floor(ny);
		
		//drawline(x,y,inx,iny,pClr);
		setpixel(inx,iny,pClr);
	}
}

VTexture* LoadBMP(char* fpath)
{
	VTexture* bostex = new VTexture;
	bostex->loadbmp(fpath);
	return bostex;
}

VTexture* LoadTEX(char* fpath)
{
	VTexture* bostex = new VTexture;
	bostex->loadtex(fpath);
	return bostex;
}
