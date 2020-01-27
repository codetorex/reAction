#include "StdAfx.h"
#include "rconsole.h"

RConsole mConsole;

RConsole::RConsole()
{
	cache = 0;
	curpos = 0;
	lines = 0;
	curline = 0;
	maxlinecount = 0;
	linecolor = 0;
}

void RConsole::Precache(int sizekb)
{
	cachesize = sizekb * 1024;
	remaining = cachesize;
	cache = new char [cachesize];
	curpos = cache;
	maxlinecount = cachesize / 64;
	lines = new char * [maxlinecount];
	linecolor = new dword [maxlinecount];
	curline = 0;
}

void RConsole::Output(char* txt, long clr)
{
	int lntxt = strlen( txt ); // length of text
	remaining -= lntxt;
	
	lines[curline] = curpos;
	strcpy(curpos,txt);
	curpos[lntxt] = 0;
	
	curpos += lntxt +1;
	linecolor[curline] = clr;
	
	curline++;
}

