/*
	Fast Array
	by Burak575

	25.06.2007

*/

#ifndef FARRAY_H
#define FARRAY_H

/*	
 *	Simple and Fast Array Class
 */
class farray
{
public:
	farray();		// Constructor

	void ** pArray;	// Memory Pointer
	long	size;	// Allocated Item Count
	long	count;	// Item Count
	
	void	add(void* data);	// Add Item
	
	//void*	operator[] (int i);	// Fetch Item
};


#endif