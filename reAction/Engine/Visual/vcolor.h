#ifndef VCOLOR_H
#define VCOLOR_H

#include "../../StdAfx.h"
#define RGBA(r,g,b,a)	((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))
#define MONO(c)         ((COLORREF)(((BYTE)(c)|((WORD)((BYTE)(c))<<8))|(((DWORD)(BYTE)(c))<<16)))
#define CLRA(c,a)		(((DWORD)c) | ((DWORD)(BYTE)(c)<<24))

#define CRED		0
#define CGRN		1
#define	CBLU		2
#define CALP		3

union color
{
	dword	lclr;
	byte	bclr[4];
};

// todo: finish it
class VColor
{
public:
	color mclr;

};

class VColor32Bit;

class VColor24Bit
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	VColor24Bit		operator /	(const float w);
	VColor24Bit&	operator *	(const float w);
	VColor24Bit		operator +	(const VColor24Bit &w);
	VColor24Bit		operator /= (const float w);
	VColor24Bit&	operator =	(const VColor24Bit &w);
	VColor24Bit&	operator =	(const long w);
	bool			operator == (const VColor24Bit &w);		
	
	void Blend(VColor24Bit* dest_clr,float tween); // blends with another color
	void Blend(VColor32Bit* dest_clr);
	
	void Mono(); // converts to mono
	void LoadRGB(byte* adr);
	void WriteBGR(byte* adr);
	void WriteRGB(byte* adr);
	long RGBOut();
	void CopyColor(byte*src,byte*dst);
};

class VColor32Bit // it has alpha channel
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	
	VColor32Bit operator / (float w);
	VColor32Bit operator * (float w);
	VColor32Bit operator /= (float w);
	VColor32Bit operator = (VColor32Bit &w);
	VColor32Bit& operator = (const long w);

	void Blend(VColor32Bit dest_clr,float tween); // blends with another color
	void Blend(VColor32Bit dest_clr);
	long RGBAOut();
	void Mono(); // converts to mono
};


#endif