#ifndef RFILESYSTEM_H 
#define RFILESYSTEM_H

#include "rcabinet.h"

// todo: ilk bunu bitirelim sora RCB yi yapariz
// todo: full directory cache yaparsak eðer real time dosya aramasý kolay olur
#define ROSFILE_CLASSID		0
#define RCABFILE_CLASSID	1

// mount permissions
#define P_READABLE			1
#define P_WRITABLE			2

enum fs_types
{
	reaction_cabinet,
	os_filesystem
};

class RMount;

class RFile // must can't be used directly
{
public:
	RMount*	parent;

	long filesize;
	long classid;

	virtual long getpos() = 0; // current read position
	virtual int  setpos(long start,int dir= SEEK_SET) = 0; // change position
	virtual int  getdata(void* dataptr,int datasize,int datacount) = 0;
	virtual int  setdata(void* dataptr,int datasize,int datacount) = 0;
	virtual	long getfilesize() = 0; // is end of file
	virtual	int  geteof() = 0; // is end of file
	virtual int  close() = 0;
};

/*class RCABFile: public RFile
{
	RCBFile* rcbf;
	dword curptr;

};*/

class ROSFile: public RFile
{
public:
	ROSFile();
	//~ROSFile();

	FILE* fp;
	char* fpath;
	
	long getpos();
	int  setpos(long start,int dir = SEEK_SET);
	int  setdata(void* dataptr,int datasize,int datacount);
	int  getdata(void* dataptr,int datasize,int datacount);
	long getfilesize();
	int  geteof();
	int  close();

	// todo: implement fputs,fgets fgetline,fprintf,fscanf,rewind, maybe freopen
};

// todo: buraya daha onceden hazirlanmis temporary base directroy stringinin tutacak bir alan ac
// ondan sonra bunun lengthini kaydet.
// böylece bu alan þöyle olur: C:\selammerhaba lenide 15 olsun
// dosya accagin zaman strcpy(tstr + leni, relativedosyapath)
// dosyayi acarsin ondan sora 15. noktaya 0 koyarsýn
// bi cesit optimizasyon iste

class RMount
{
public:
	fs_types	filesystem;
	
	union
	{
		char*		fspath; // used when fs = os_filesystem
		RCabinet*	cabinet; // used when 
	};

	long		lpath; // path len
	long		perm; // permissions

	RMount*		nextmount;
	RMount*		prevmount;

	RFile*		OpenFile(char* fpath,char* izin); // todo: implement remove file,rename file
	bool		IsFileExist(char* fpath);
};

class RFileSystem
{
public:
	RFileSystem();

	RMount* lastusedmount; // used for optimization
	RMount* defsavemount; // used for saving files

	RMount* firstmount;
	RMount* lastmount;
	int		mountcount;

	RMount*	MountCabinet(RCabinet* cab,long permission = P_READABLE);
	RMount*	MountFolder(char* fpath,long permission = P_READABLE);

	void	SetDefaultSaveMount(RMount* mount);

	void	UnMount(RMount* mount);

	RFile*	OpenFile(char* fpath,char* izin);
	void	CloseFile(RFile* fil);

private:
	void	AddMount(RMount* mount);
};


RFile* ROpenFile(char* fpath,char* izin);
int RCloseFile(RFile* fil);

RFileSystem* GetFileSystem();

#endif