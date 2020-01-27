#include "StdAfx.h"
#include "rcabinet.h"
#include "rtools.h"

using namespace RTools;

void RCabinet::LoadRCB(char* fpath)
{
	FILE* fp;
	fp = fopen(fpath,"rb");
	if ( fp )
	{
		ReadHeader(&cheader,fp);
		if (*(long*)cheader.signature == 'RCB\x0')
		{			
			if (cheader.version == RCB_BUILD)
			{
				int plen = strlen(fpath);
				rcbpath = new char [plen+1];
				strcpy(rcbpath,fpath);

				if (cheader.directorysize != 0)
				{
					dword dirstart = cheader.blocksize * cheader.blockcount;
					dirstart += 32; // forheader
					
					fseek(fp,dirstart,SEEK_SET);
					// Start loading directories and files at this point
					byte *tempmem = new byte [cheader.directorysize];
					fread(tempmem,1,cheader.directorysize,fp); // dump full directory info to memory
					// because it will not fair making billions of read call for 1000 files.
					dirdatastart = tempmem;
					rootdir = LoadDirectoryData(tempmem,0); // parent = 0 because its root					
				}
				else
				{
					// it doesnt have anyfile yet
				}
			}
			else
			{
				// unsupported version
			}
		}
		else
		{
			// not valid rcb
		}
		fclose(fp);
		isopen = false;
	}
	else
	{
		// error file not exist...
	}

}

void RCabinet::CreateRCB(char* fpath,dword blocksize,dword id,dword flags)
{
	FILE* fp;
	fp = fopen(fpath,"wb");

	if (fp)
	{
		cheader.signature[0] = 'R';
		cheader.signature[1] = 'C';
		cheader.signature[2] = 'B';
		cheader.signature[3] = 0;
		cheader.id = id;
		cheader.version = RCB_BUILD; // build number of loader
		cheader.filesize = 32;
		cheader.blocksize = blocksize;
		cheader.blockcount = 0;
		cheader.directorysize = 0;
		cheader.flags = flags;
		WriteHeader(&cheader,fp);

		fclose(fp);
	}
	else
	{
		// error occured...
	}
}

void RCabinet::ReadHeader(RCBHeader* shdr,FILE* fp)
{
	fread(shdr->signature,1,4,fp);
	fread(&shdr->id,1,4,fp);
	fread(&shdr->version,1,4,fp);
	fread(&shdr->filesize,1,4,fp);
	fread(&shdr->blocksize,1,4,fp);
	fread(&shdr->blockcount,1,4,fp);
	fread(&shdr->directorysize,1,4,fp);
	fread(&shdr->flags,1,4,fp);
}

void RCabinet::WriteHeader(RCBHeader* shdr,FILE* fp)
{
	fwrite(shdr->signature,1,4,fp);
	fwrite(&shdr->id,1,4,fp);
	fwrite(&shdr->version,1,4,fp);
	fwrite(&shdr->filesize,1,4,fp);
	fwrite(&shdr->blocksize,1,4,fp);
	fwrite(&shdr->blockcount,1,4,fp);
	fwrite(&shdr->directorysize,1,4,fp);
	fwrite(&shdr->flags,1,4,fp);
}

void RCBDirectory::AddDirectory(RCBDirectory* rd)
{
	if (firstsubdir == 0)
	{
		firstsubdir = rd;
		lastsubdir = rd;
		rd->nextdir = 0;
		rd->prevdir = 0;
		subdircount++;
	}
	else
	{
		lastsubdir->nextdir = rd;
		rd->nextdir = 0;
		rd->prevdir = lastsubdir;
		lastsubdir = rd;
		subdircount++;
	}
	rd->parentd = this;
}

void RCBDirectory::AddFile(RCBFile* rf)
{
	if (firstfile == 0)
	{
		firstfile = rf;
		lastfile = rf;
		rf->nextfile = 0;
		rf->prevfile = 0;
		filecount++;
	}
	else
	{
		lastfile->nextfile = rf;
		rf->nextfile = 0;
		rf->prevfile = lastfile;
		lastfile = rf;
		filecount++;
	}
	rf->parentd = this;
}

void RCBDirectory::NewDirectory(char* dname)
{
	RCBDirectory* edir;
	edir = new RCBDirectory;

	edir->dirnamehash = shash(dname);
	strcpy(edir->dirname,dname);
	edir->parentd = this;

	AddDirectory(edir);
}

#define readlng()	*(long*)mptr2; mptr2+=4;
#define readlngp(mpt) *(long*)mpt;mpt+=4;

RCBDirectory* RCabinet::LoadDirectoryData(byte* memptr,RCBDirectory* prnt)
{
	byte* mptr2 = memptr;
	RCBDirectory* tdir; // temp dir
	tdir = new RCBDirectory;
	
	tdir->dirnamehash = readlng();
	tdir->nextdir = (RCBDirectory*)readlng(); // actually its not loaded yet
	memcpy(tdir->dirname,mptr2,32);mptr2+=32;
	tdir->filecount = readlng();
	tdir->flags = readlng();
	tdir->subdircount = readlng();
	tdir->firstfile = (RCBFile*)readlng();
	tdir->firstsubdir = (RCBDirectory*)readlng();
	tdir->parentd = prnt;

	// buraya oyle bir kod gelecekki recursive loadliyacak

	// yahu cok guzel fikirde stack overflow korkum var
	// acaba esp yi degistirererk 10 mb stack acabilirmiyim?
	// bakalim bi deneyelimde olmassa looopla yapariz
	// ki hesaplarima gore her func icin ayri bir func cagirsa bile
	// 1000 klasor ve her klasorde 10 dosya olsa maximum 10000 funccall yapar
	// ki stack zaten 2 mb ise sorun olmaz gibime geliyor

	if (tdir->filecount > 0) // ilk once dosyalari yuklersem bole bi sorun olmaz
	{
		byte* ffile = (byte*)((long)dirdatastart + (long)tdir->firstfile);
		tdir->firstfile = LoadFileData(ffile,tdir);
	}

	if (tdir->subdircount > 0)
	{
		byte* fsubd = (byte*)((long)dirdatastart + (long)tdir->firstsubdir);
		tdir->firstsubdir = LoadDirectoryData(fsubd,tdir);
	}


	if (tdir->nextdir > 0)
	{
		byte* nextd = (byte*)((long)dirdatastart + (long)tdir->nextdir);
		tdir->nextdir = LoadDirectoryData(nextd,tdir);
		tdir->nextdir->prevdir = tdir;
	}
	else
	{
		if (prnt > 0) // if not root then
		{
			prnt->lastsubdir = tdir;
		}
	}

	return tdir;
}

RCBFile* RCabinet::LoadFileData(byte* memptr,RCBDirectory* prnt)
{
	byte* mptr2 = memptr;
	RCBFile* tfil; // temp file
	tfil = new RCBFile;

	tfil->filenamehash = readlng();
	tfil->parentd = prnt;
	tfil->nextfile = (RCBFile*)readlng();
	tfil->prevfile = 0;
	memcpy(tfil->md5hash,mptr2,16);mptr2+=16;
	memcpy(tfil->filename,mptr2,32);mptr2+=32;
	tfil->filesize = readlng();
	tfil->flags = readlng();
	tfil->fragmentcount = readlng();
	tfil->firstfragment = (RCBFileFragment*)readlng();


	if (tfil->filesize > 0 && tfil->firstfragment != 0)
	{
		RCBFileFragment* ffrag;
		RCBFileFragment* pfrag = 0;
		byte* sp = (byte*)((long)dirdatastart + (long)tfil->firstfragment);
		long tmp;
		do 
		{
			ffrag = new RCBFileFragment;
			if (pfrag)
			{
				pfrag->nextfragment = ffrag;
			}
			ffrag->realstart = readlngp(sp);
			ffrag->size = readlngp(sp);
			ffrag->virtualstart = readlngp(sp);
			tmp = readlngp(sp);
			ffrag->nextfragment = 0;
			pfrag = ffrag;
			sp = (byte*)((long)dirdatastart + sp);
		} while (tmp != 0);
	}

	if (tfil->nextfile > 0)
	{
		byte* nextf = (byte*)((long)dirdatastart + (long)tfil->nextfile);
		tfil->nextfile = LoadFileData(nextf,prnt);
		tfil->nextfile->prevfile = tfil;
	}
	else
	{
		prnt->lastfile = tfil;
	}

	return tfil;
}