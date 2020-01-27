
//  reAction Cabinet Processor
//	version 0.1 beta initial release

#ifndef RCABINET_H
#define RCABINET_H

#include "../../StdAfx.h"

enum // RCB Flags
{
	ENCRYPT_FILENAMES = 0x00000001
};

enum // RCB Sub File Flags
{
	FULL_ENCRYPT 	= 0x00000001,
	ZLIB_COMPRESS 	= 0x00000002,
};

#define RCB_BUILD	1
// its maybe first and last build lol
class RCBDirectory;

struct RCBHeader	// 32 bytes
{
	char			signature[4];
	dword			id;
	dword			version;
	dword			filesize;
	dword			blocksize;
	dword			blockcount; // must be multiply of 32
	dword			directorysize;
	dword			flags;
};

class RCBFileFragment
{
public:
	long				realstart; // start of the *.rcb
	long				size; // size of fragment
	long				virtualstart; // real file start
	RCBFileFragment*	nextfragment; // nextfragment
};

class RCBFile
{
public:
	dword				filenamehash;
	RCBDirectory*		parentd; // parent directory its not included in rcb but created in loading
	RCBFile*			nextfile;
	RCBFile*			prevfile;
	char				md5hash[16];
	char				filename[32];
	dword				filesize;
	dword				flags;
	dword				fragmentcount;
	RCBFileFragment*	firstfragment;

	void				MoveFile(RCBDirectory* trgt);
	void				Remove(); // removes from parent dir so it float

	void				Delete(); // totally delete this file
	
	void				AllocateSpace(dword sz);
	void				AllocateDynamicSpace();
	void				DeallocateSpace();
};

class RCBDirectory
{
public:
	dword			dirnamehash;
	RCBDirectory*	parentd; // parent directory
	RCBDirectory*	nextdir;
	RCBDirectory*	prevdir; // this pointer will point when rcb loaded so its not included in rcb
	char			dirname[32];
	dword			filecount;
	dword			flags;
	dword			subdircount;
	RCBFile*		firstfile;
	RCBFile*		lastfile; // same with prevdir
	RCBDirectory*	firstsubdir;
	RCBDirectory*	lastsubdir; // same with prevdir

	void			NewDirectory(char* dname);
	void			InsertFile(char* fpath);
	void			NewFile(char* fname);

	void			AddFile(RCBFile* rf);
	void			AddDirectory(RCBDirectory* rd);

	void			RemoveSelf();

	void			Delete(); // totally delete all subfolders and files

	//void			RemoveFile(RCBFile* rf); // it will only removes pointer, so you can add it to another folder
	//void			RemoveDirectory(RCBDirectory* rd);
};

class RCBBitmap
{
public:
	dword			notemptyuntil;
	dword			emptyblocks;
	dword			*bmap;

	dword			FindEmptyBlock(dword size,dword sstart = 0); // will used for adding files, sstart = start search from
	void			MakeBlocksEmpty(dword sblock,dword eblock); // will used for removing files
	void			RemoveBlocks(dword sblock,dword eblock);
	void			MakeBlocksFilled(dword sblock,dword eblock);
};

class RCabinet
{
public:
	char*			rcbpath; // file path
	FILE*			fileptr; // file pointer
	bool			isopen;	 // true if file is ready to read

	RCBHeader		cheader; // cabinet header
	RCBDirectory*	rootdir;
	RCBBitmap*		filemap;

	void			CreateRCB(char* fpath,dword blocksize,dword id,dword flags = 0);
	void			LoadRCB(char* fpath);

	RCBFile*		GetFile(char* vfpath);
	RCBDirectory*	GetDirectory(char* dirpath);

	void			AddFile(char* fpath,RCBDirectory* dir);
	void			DeleteFile(char* fpath);
	void			MoveFile(char* vfsrc,char* vfdst);
	void			Commit(); // updates directory structure and closes file for write

	RCBFileFragment*	AllocateFile(dword requestsize);
	void				DeallocateFile(RCBFileFragment* addr); // must be first fragment

	void			Defragment();

private:
	void			ReadHeader(RCBHeader* shdr,FILE* fp);
	void			WriteHeader(RCBHeader* shdr,FILE* fp);
	
	byte*			dirdatastart;
	RCBDirectory*	LoadDirectoryData(byte* memptr,RCBDirectory* prnt); // will load all directory structure recursively
	RCBFile*		LoadFileData(byte* memptr,RCBDirectory* prnt);
};

/*class RCabinet
{
private:

public:
	char* currentfile;
	RCBHeader myheader;

	void LoadRCB	(char* filename);		// load rcb file

	long GetFilePos	(char* filename,RCBSubFile* r_fileinfo);	// gets file position to start read
	long GetFile	(char* filename,RCBSubFile* r_fileinfo);	// loads file and returns memory address
	

	void AddFile	(char* filepath,char* rcbfilepath,long flags,unsigned char version = 0);	// adds file to current cabinet
	void DeleteFile	(char* filename);				// delete a file ( virtually deletes )
	void MoveFile	(char* srcname,char* destname); // move a file src name to destname

	void Defragment	();

	// File Functions
	void WriteSubHeader	(RCBSubFile* sfile,FILE* pak);
	void WriteHeader	(RCBHeader* shdr,FILE* pak);
	void ReadSubHeader	(RCBSubFile* sfile,FILE* pak);
	void ReadSubHeaderL	(RCBSubFile* sfile,FILE* pak); // lite version for file search
	void ReadHeader		(RCBHeader* shdr,FILE* pak);

};*/

#endif