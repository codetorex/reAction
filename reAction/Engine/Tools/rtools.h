
// reAction Tool Functions

#ifndef RTOOLS_H
#define RTOOLS_H

#include "../../StdAfx.h"

int PowerOfTwo(dword num); // finds the number how much it power of two

namespace RTools
{
	bool	IsFlagSet	(long flag,long bits);
	long	CharSum		(char* adr,long len);
	long	CharSumPath	(char* adr,long len);
	char*	GetFileName (char* adr,long len);
	void	QuickInfo	(const char* fmt, ...);
	long	shash		(char *key);
};

class RCollection
{
private:
	void Precache();
	long* pDataPointers; //
	
public:	
	void	Add			(void* pData);
	void	Clear		();
	long	Count;
	void*	Item		(int indx);
	void	Remove		(int indx);
};

/*	
 *	Simple and Fast Array Class
 *	todo: make better one of this for hold multiple data types
 */
class RArray
{
public:
	RArray();		// Constructor
	~RArray();

	void ** pArray;	// Memory Pointer
	long	size;	// Allocated Item Count
	long	count;	// Item Count
	
	void	add(void* data);	// Add Item
	void	remove(int id); // SLOOW func.
	//void*	operator[] (int i);	// Fetch Item
};


// todo: implement this
class RLinkNode
{
	void*		data;
	RLinkNode*	nextnode;
	RLinkNode*	prevnode;
};

class RLinkedList
{
	RLinkNode*	firstnode;
	RLinkNode*	lastnode;

	long		itemcount;

	void		AddItem(void* data);
	void		DelItem(RLinkNode* node);
};

/*
struct RBinaryNode
{
	long			dataptr;	
	RBinaryNode*	l_node;
	RBinaryNode*	r_node;
};

class RBinaryTree
{
public:
	RBinaryNode		root;
	RBinaryTree();

	RBinaryNode* AddNode (RBinaryNode* root,bool left,long data);


};

*/

class RString
{
public:
	RString();
	RString(char* asig);
	RString(RString* rst);
	// todo: check for bounds in all functions


	~RString();
	
	char* cstr;
	int slen;

	void		setstring(char* st,...);
	int			instr(int start,RString* ct);
	int			instr(int start,char* ct);
	RString		substr(int strt,int ln);
	RString**	split(char* delim); // todo: do this
	void		ltrim();
	void		rtrim();
	void		trim();
	RString		left(int ln);
	RString		right(int ln);
	void		remove(int st,int ed); // removes chars from string between st and ed
	int			toint();
	float		tofloat();

	void		makelower();
	void		makeupper();
	void		memstatus();

	RString& operator = (char* asig);
	RString& operator + (char* asig);
	RString& operator + (RString* rst);
	RString& operator = (RString* rst);
	RString& operator = (RString& rst);
	RString& operator + (int ik);
	RString& operator + (float flt);
	RString& operator + (double dbl);
	RString& operator + (char ch);	
};

#endif