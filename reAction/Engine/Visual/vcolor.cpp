#include "StdAfx.h"
#include "vcolor.h"
#include "../Mathematics/mvector.h"

VColor24Bit& VColor24Bit::operator = (const VColor24Bit &w)
{
	r = w.r;
	g = w.g;
	b = w.b;
	return *this;
}

bool VColor24Bit::operator == (const VColor24Bit &w)
{
	if ( w.r == r && w.g == g && w.b == b)
	{
		return true;
	}
	return false;
}

VColor24Bit& VColor24Bit::operator = (const long w)
{
	_asm
	{
		mov ebx,[this]
		mov eax,w
		mov byte ptr [ebx]this.r,al
		mov byte ptr [ebx]this.g,ah
		shr eax,16
		mov byte ptr [ebx]this.b,al
	}
	return *this;
}

VColor24Bit VColor24Bit::operator + ( const VColor24Bit &w)
{
	r += w.r;
	g += w.g;
	b += w.b;
	if (r>255) r= 255;
	if(g>255) g=255;
	if (b>255) b = 255;
	return *this;
}

void VColor24Bit::LoadRGB(byte* adr)
{
	r = *adr;
	g = *(adr+1);
	b = *(adr+2);
}

void VColor24Bit::WriteBGR(byte* adr)
{
	*adr = b;
	*(adr+1) = g;
	*(adr+2) = r;
}
void VColor24Bit::WriteRGB(byte* adr)
{
	*adr = r;
	*(adr+1) = g;
	*(adr+2) = b;
}

void VColor24Bit::Mono()
{
	int ts;
	ts = r;
	ts += g;
	ts += b;
	ts /= 3;
	r = (byte)ts;
	g = (byte)ts;
	b = (byte)ts;
}

void VColor24Bit::Blend(VColor24Bit* dest_clr,float tween)
{

	float x,y,z;

	x = ((float)dest_clr->r * tween ) + (float)r * (1.0f - tween);
	y = ((float)dest_clr->g * tween ) + (float)g * (1.0f - tween);
	z = ((float)dest_clr->b * tween ) + (float)b * (1.0f - tween);
	r = (byte)x;
	g = (byte)y;
	b = (byte)z;
}

void VColor24Bit::Blend(VColor32Bit* dest_clr)
{
	float x,y,z,p;
	p = dest_clr->a/255.0f;
	x = ((float)dest_clr->r * p ) + (float)r * (1.0f - p);
	y = ((float)dest_clr->g * p ) + (float)g * (1.0f - p);
	z = ((float)dest_clr->b * p ) + (float)b * (1.0f - p);
	r = (byte)x;
	g = (byte)y;
	b = (byte)z;
}

VColor24Bit& VColor24Bit::operator * (const float w)
{
	float x;
	x = (float)r * w;
	r = (byte)x;
	x = (float)g * w;
	g = (byte)x;
	x = (float)b * w;
	b = (byte)x;
	return *this;
}

long VColor24Bit::RGBOut()
{
	return (RGB(r,g,b));
}

void VColor24Bit::CopyColor(byte*src,byte*dst)
{
	*dst = *src;
	*(dst+1) = *(src+1);
	*(dst+2) = *(src+2);
}

VColor32Bit& VColor32Bit::operator = (const long w)
{
	char* dt;
	dt = (char*)&w;
	r = dt[0];
	g = dt[1];
	b = dt[2];
	a = dt[3];
	
	/*_asm
	{
		mov ebx,[this]
		mov eax,w
		mov byte ptr [ebx]this.r,al
		mov byte ptr [ebx]this.g,ah
		shr eax,16
		mov byte ptr [ebx]this.b,al
		mov byte ptr [ebx]this.a,ah
	}*/
	return *this;	
}

void VColor32Bit::Blend(VColor32Bit dest_clr)
{
/*	float x,y,z,fa,p;
	p = dest_clr->a/255.0f;
	x = ((float)dest_clr->r ) + (float)r * (1.0f - p);
	y = ((float)dest_clr->g ) + (float)g * (1.0f - p);
	z = ((float)dest_clr->b ) + (float)b * (1.0f - p);
	fa= ((float)dest_clr->a ) + (float)b * (1.0f - p);
	r = (byte)x;
	g = (byte)y;
	b = (byte)z;
	a = (byte)fa;*/

	/*float fr,fg,fb,a1,fa;
	a1 = a/255.0f;
	fr = (float)r + (float)dest_clr.r * (1.0f - a1);
	fg = (float)g + (float)dest_clr.g * (1.0f - a1);
	fb = (float)b + (float)dest_clr.b * (1.0f - a1);
	fa = (float)a + (float)dest_clr.a * (1.0f - a1);
	r = (byte)fr;
	g = (byte)fg;
	b = (byte)fb;
	a = (byte)fa;*/
}

long VColor32Bit::RGBAOut()
{
	return RGBA(r,g,b,a);
}