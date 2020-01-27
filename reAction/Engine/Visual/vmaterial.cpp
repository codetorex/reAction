#include "stdafx.h"
#include "vmaterial.h"
#include "../tools/rtools.h"
#include "../tools/rfilesystem.h"

using namespace RTools;

VMaterialMemory mMaterials;

VMaterialMemory::VMaterialMemory()
{
	firstmat = 0;
	lastmat = 0;
	matcount = 0;
}

void VMaterialMemory::AddMaterial(VMaterial* mt)
{
	if (firstmat == 0)
	{
		firstmat = mt;
		lastmat = mt;
		mt->nextmat = 0;
		mt->prevmat = 0;
		matcount++;
	}
	else
	{
		lastmat->nextmat = mt;
		mt->nextmat = 0;
		mt->prevmat = lastmat;
		lastmat = mt;
		matcount++;
	}
}

void VMaterialMemory::RemoveMaterial(VMaterial* mt)
{
	VMaterial *nx,*px;
	nx = mt->nextmat;
	px = mt->prevmat;
	
	if (px)
	{
		px->nextmat = nx;
	}
	else
	{
		firstmat = nx;
	}
	
	if (nx)
	{
		nx->prevmat = px;
	}
	else
	{
		lastmat = nx;
	}
	
	mt->nextmat = 0;
	mt->prevmat = 0;
	
	matcount--;
}

VMaterial* VMaterialMemory::GetMaterialByPath(char* mpath,VMaterial* start /* = 0 */)
{
	VMaterial* nx = firstmat;
	long hs = shash(mpath);
	if (start)
	{
		nx = start;
	}
	
	while (nx)
	{
		if (nx->matpathhash == hs)
		{
			if (strcmp(nx->matpath,mpath) == 0)
			{
				return nx;
			}
		}
		nx = nx->nextmat;
	}
	return 0;
}

VMaterial* LoadRMT(char* rmtpath)
{
	return mMaterials.LoadRMT(rmtpath);
}


void VMaterial::AttachMaterial()
{
	if (mp_shadertype == mp_generictexture)
	{
		glBindTexture(GL_TEXTURE_2D,mp_diffusemap->glID);
	}
}


void VMaterial::SetParameter(char* paramname,char* paramdata)
{
	RString pnm;
	RString pdt;
	pnm = paramname;
	pdt = paramdata;
	pnm.trim();
	pdt.trim();


	if ( strcmp(pnm.cstr,"shader") == 0)
	{
		if (strcmp(pdt.cstr,"generictexture") == 0)
		{
			mp_shadertype = mp_generictexture;
		}
		return;
		// todo: implement others
	}

	if (strcmp(pnm.cstr,"diffusemap") == 0)
	{
		RString filetype;
		RString filepath;
		filetype = pdt.left(3);
		int x,y;
		x = pdt.instr(0,"\"")+1;
		y = pdt.instr(x,"\"");
		filepath = pdt.substr(x,y-x);
		//QuickInfo(filepath.cstr);

		VTexture* t;
		t = mTextures.GetTextureByPath(filepath.cstr);
		if (t != 0 )
		{
			while ( t->changed )
			{
				if (t->nexttex != 0)
				{
					t = mTextures.GetTextureByPath(filepath.cstr,t->nexttex);
					if (t == 0)
					{
						break;
					}
				}
			}
		}

		if (t == 0)
		{
			// load that texture
			mp_diffusemap = new VTexture;
			if (strcmp(filetype.cstr,"BMP") == 0)
			{
				mp_diffusemap->loadbmp(filepath.cstr,true);
			}
			if (strcmp(filetype.cstr,"TEX") == 0)
			{
				mp_diffusemap->loadtex(filepath.cstr,true);
			}
		}
		else
		{
			mp_diffusemap = t;
		}
		return;
	}

	//QuickInfo("-%s- = -%s-",paramname,paramdata);
}

VMaterial* VMaterialMemory::LoadRMT(char* rmtpath)
{
	char* rmtdata;
	RFile* rmt;
	rmt = ROpenFile(rmtpath,"rt");
	if (rmt == 0)
	{
		return 0;
	}
	int flen = rmt->getfilesize();
	rmtdata = new char [flen];
	rmt->getdata(rmtdata,1,flen);
	RCloseFile(rmt);

	VMaterial* nmat = new VMaterial;

	// todo: optimize here
	// possible bug : dosya sonu / ile biterse
	int i;
	int k = 0;
	char dt;
	char paramname[64];
	char paramdata[128];
	bool getname = true;
	bool satirbasi = true;

	for (i=0; i<flen;i++)
	{
		dt = rmtdata[i];
		if (dt == '/')
		{
			if ( rmtdata[i+1] == '/')
			{
				while ( (dt != 0x0D || dt != 0x0A) && i < flen)
				{
					i++;
					dt = rmtdata[i];
				}
			}
			else
			{
				if (rmtdata[i+1] == '*')
				{
					i+=2;
					while ( i<flen )
					{
						dt = rmtdata[i];
						if (dt == '*')
						{
							if (rmtdata[i+1] == '/')
							{
								break;
							}
						}
						i++;
					}
				}
				else
				{
					if (getname)
					{
						paramname[k] = dt;
						k++;
					}
					else
					{
						paramdata[k] = dt;
						k++;
					}
				}
			}
		}
		else
		{
			if (dt == '=' )
			{
				getname = false;
				paramname[k] = 0;
				k = 0;
			}
			else
			{
				if (dt == 0x0D || dt == 0x0A || i == (flen-1))
				{
					if (getname)
					{
						if (k == 0)
						{
							// blank line
						}
						else
						{
							// premature end error
						}
					}
					else
					{
						paramdata[k] = 0;
						nmat->SetParameter(paramname,paramdata);
//						prmnms.add(paramname);
//						prmdts.add(paramdata);
						// set variable
					}
					getname = true;
					k = 0;
				}
				else
				{
					if (getname)
					{
						paramname[k] = dt;
						k++;
					}
					else
					{
						paramdata[k] = dt;
						k++;
					}
				}
			}
		}
	}

	delete [] rmtdata;
	AddMaterial(nmat);
	return nmat;
}

