
#include "StdAfx.h"
#include "mathlib.h"

fxd::fxd()
{
	a = 0;
}

fxd::fxd(float b)
{
	*this = b;
}

fxd::fxd(int b)
{
	*this = b;
}

fxd& fxd::operator = (float b)
{
	int ri;
	b *= 16.0f;
	__asm
	{
		fld  b
		fistp ri
	}

	a = ri;
    return *this;
}

fxd& fxd::operator = (int b)
{
	a = b << 4;
	return *this;
}

fxd::operator int()
{
	return (a >> 4);
}

fxd::operator float()
{
	return ((float)a / 16.0f);
}

fxd fxd::operator + (fxd b)
{
	fxd c;
	c.a = a + b.a;
	return c;
}

fxd fxd::operator - (fxd b)
{
	fxd c;
	c.a = a - b.a;
	return c;	
}

fxd fxd::operator * (fxd b)
{
	fxd c;
	c.a = ((a) * (b.a)) >> 4;
	return c;
}

fxd fxd::operator / (fxd b)
{
	fxd c;
	c.a = ((a << 4) / b.a);
	return c;
}




// 8 bit fixed functions

fxd8::fxd8()
{
	a = 0;
}

fxd8::fxd8(float b)
{
	*this = b;
}

fxd8::fxd8(int b)
{
	*this = b;
}

fxd8& fxd8::operator = (float b)
{
	int ri;
	b *= 256.0f;
	__asm
	{
		fld  b
		fistp ri
	}
	
	a = ri;
    return *this;
}

fxd8& fxd8::operator = (int b)
{
	a = b << 8;
	return *this;
}

fxd8::operator int()
{
	return (a >> 8);
}

fxd8::operator float()
{
	return ((float)a / 256.0f);
}

fxd8 fxd8::operator + (fxd8 b)
{
	fxd8 c;
	c.a = a + b.a;
	return c;
}

fxd8 fxd8::operator - (fxd8 b)
{
	fxd8 c;
	c.a = a - b.a;
	return c;	
}

fxd8 fxd8::operator * (fxd8 b)
{
	fxd8 c;
	c.a = ((a >> 4) * (b.a >> 4));
	return c;
}

fxd8 fxd8::operator / (fxd8 b)
{
	fxd8 c;
	c.a = ((a << 4) / (b.a >> 4));
	return c;
}
