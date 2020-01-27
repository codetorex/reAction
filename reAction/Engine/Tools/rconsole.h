
#ifndef RCONSOLE_H
#define RCONSOLE_H


// todo: add log.txt output

// console shit
class RConsole
{
public:
	RConsole();
	
	void Precache(int sizekb);

	char* cache;					// cache
	char* curpos;					// current position in cache
	int cachesize;
	int remaining;
	
	dword* linecolor;				// line colors
	char** lines;					// line array
	int curline;					// line count;
	int maxlinecount;

	void Output(char* txt,long clr=0xFFFFFF);			// outputs one line of console text
	void Execute(char* cmdline);	// executes one line of console code
};

extern RConsole mConsole;

#endif
