#include "stdafx.h"
#include "rfilesystem.h"
#include "rtools.h"
#include "../tools/rtools.h"

using namespace RTools;

RFileSystem mainfs;

// *************************************************
// OS FILE FUNCTIONS
// *************************************************

ROSFile::ROSFile()
{
	filesize = 0;
	parent = 0;
	classid = ROSFILE_CLASSID;	
	fp = 0;
}

/*ROSFile::~ROSFile()
{
	if (fpath != 0)
	{
		delete [] fpath;
	}
}*/

long ROSFile::getpos()
{
	return ftell(fp);
}

int ROSFile::setpos(long start,int dir)
{
	return fseek(fp,start,dir);
}

int ROSFile::setdata(void* dataptr,int datasize,int datacount)
{
	return fwrite(dataptr,datasize,datacount,fp);
}

int ROSFile::getdata(void* dataptr,int datasize,int datacount)
{
	return fread(dataptr,datasize,datacount,fp);
}

long ROSFile::getfilesize()
{
	long t = ftell(fp);
	fseek(fp,0,SEEK_END);
	filesize = ftell(fp);
	fseek(fp,t,SEEK_SET);
	return filesize;
}

int ROSFile::geteof()
{
	return feof(fp);
}

int ROSFile::close()
{
	if (fpath != 0)
	{
		delete [] fpath;
	}
	return fclose(fp);
}

// *************************************************
// MOUNT FUNCTIONS
// *************************************************

RFile* RMount::OpenFile(char* fpath,char* izin)
{
	if (filesystem == os_filesystem)
	{
		int lfpath = lpath + strlen(fpath);
		char* nfilepath = new char[lfpath+1];
		if (!nfilepath)
			QuickInfo("Memory allocation error...");
		strcpy(nfilepath,fspath);
		strcpy(nfilepath+lpath,fpath);
		
		FILE* nfp = fopen(nfilepath,izin);
		if (nfp != NULL)
		{
			// file opened successfuly
			ROSFile* nfile = new ROSFile; // new file
			nfile->parent = this;
			nfile->fp = nfp;
			nfile->fpath = nfilepath;
			return nfile;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		// todo: implement this
	}
	
	return 0;
}

bool RMount::IsFileExist(char* fpath) // someone requesting file
{
	if (filesystem == os_filesystem)
	{
		DWORD fileAttr;
		strcpy(fspath+lpath,fpath);
		fileAttr = GetFileAttributes(fspath);
		fspath[lpath] = 0;

		if (fileAttr == 0xFFFFFFFF)
		{
			return false;
		}
		return true;
	}
	else
	{
		// todo: implement this
		return false;
	}
}

// *************************************************
// FILE SYSTEM FUNCTIONS
// *************************************************

RFileSystem::RFileSystem()
{
	firstmount= 0;
	lastmount = 0;
	mountcount = 0;
	lastusedmount = 0;
	defsavemount = 0;
}

void RFileSystem::CloseFile(RFile* fil)
{
	if ( fil != 0 )
	{
		fil->close();
		delete [] fil;
	}
}

void RFileSystem::SetDefaultSaveMount(RMount* mount)
{
	defsavemount = mount;
}

// scan throught all mounted cabinets and folders if file found use it
RFile* RFileSystem::OpenFile(char* fpath,char* izin)
{
	if ( izin[0] == 'r' )
	{
		if (lastusedmount != 0)
		{
			if (lastusedmount->perm & P_READABLE)
			{
				if (lastusedmount->IsFileExist(fpath))
				{
					return lastusedmount->OpenFile(fpath,izin);
				}
			}
		}
		else
		{
			int i;
			RMount* nmount = firstmount;
			for ( i = 0;i<mountcount;i++ )
			{
				if ( nmount->IsFileExist(fpath))
				{
					lastusedmount = nmount;
					return nmount->OpenFile(fpath,izin);
				}
				nmount = nmount->nextmount;
			}
		}
	}
	if (izin[0] == 'w' || izin[0] == 'a')
	{
		if (defsavemount != 0)
		{
			if (defsavemount->perm & P_WRITABLE)
			{
				return defsavemount->OpenFile(fpath,izin);
			}
			else
			{
				RTools::QuickInfo("Your default save mount doesn't have write permissions.");
			}
		}
		else
		{
			int i;
			RMount* nmount = firstmount;
			for ( i = 0;i<mountcount;i++ )
			{
				if (nmount->perm & P_WRITABLE)
				{
					if (nmount->filesystem == os_filesystem)
					{
						lastusedmount = nmount;
						return nmount->OpenFile(fpath,izin);
					}
				}
				nmount = nmount->nextmount;
			}
		}
	}
	return 0;
}

RMount* RFileSystem::MountCabinet(RCabinet* cab,long permission)
{
	RMount* nmount;
	nmount = new RMount;
	nmount->lpath = 0;
	nmount->filesystem = reaction_cabinet;
	nmount->perm = permission;

	
	nmount->cabinet = cab;

	AddMount(nmount);
	return nmount;
}

RMount* RFileSystem::MountFolder(char* fpath,long permission)
{
	RMount* nmount; // new mount
	nmount = new RMount;
	nmount->filesystem = os_filesystem;
	nmount->perm = permission;
	
	char* cfpath;
	cfpath = new char [256];
	strcpy(cfpath,fpath);
	nmount->fspath = cfpath;
	nmount->lpath = strlen(fpath);
	
	AddMount(nmount);
	return nmount;
}


void RFileSystem::AddMount(RMount* mount)
{
	if (firstmount == 0)
	{
		firstmount = mount;
		lastmount = mount;
		mount->nextmount = 0;
		mount->prevmount = 0;
		mountcount++;
	}
	else
	{
		lastmount->nextmount = mount;
		mount->nextmount = 0;
		mount->prevmount = lastmount;
		lastmount = mount;
		mountcount++;
	}
}

void RFileSystem::UnMount(RMount* mount)
{
	RMount *nx,*px;
	nx = mount->nextmount;
	px = mount->prevmount;
	
	if (px)
	{
		px->nextmount = nx;
	}
	else
	{
		firstmount = nx;
	}
	
	if (nx)
	{
		nx->prevmount = px;
	}
	else
	{
		lastmount = nx;
	}
	
	mount->nextmount = 0;
	mount->prevmount = 0;
	
	mountcount--;
}

RFileSystem* GetFileSystem()
{
	return &mainfs;
}

RFile* ROpenFile(char* fpath,char* izin)
{
	return mainfs.OpenFile(fpath,izin);
}

int RCloseFile(RFile* fil)
{
	return fil->close();
}
