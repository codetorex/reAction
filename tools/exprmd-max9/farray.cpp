#include "StdAfx.h"
#include "farray.h"

#define MOD_64	& 0x3F

farray::farray()
{
	size = 0;
	count = 0;
	pArray = 0;
}

void farray::add(void *data)
{
	// Allocate Memory
	if (size == 0)	// if it is never allocated
	{
		size = 64;//increase size variable
		pArray = new void * [size]; // allocate new empty space
	}
	else
	{
		if ((count MOD_64) == 0) // expand memory
		{
			size+=64;	// increase size variable
			void ** bos = new void * [size]; // allocate new empty space
			memcpy(bos,pArray,count<<2);	// copy old memory block to new
			delete [] pArray;	// delete old block
			pArray = bos;	// attach new memory block
		}
	}
	pArray[count] = data;	// add data's pointer to array list
	count++; // increase count
}