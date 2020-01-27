
#include "StdAfx.h"
#include "vterrain.h"
#include "../Tools/rtools.h"
#include "ext/GL_EXT_texture3D.h"

TEcosystemData::TEcosystemData()
{
	texcount = 0;
	textures = 0;
	texdensity = 0;
	curtex= 0;
}

void TEcosystemData::PrepareData(int count)
{
	texcount = count;
	textures = new VTexture * [texcount];
	texdensity = new int [texcount];
}

void TEcosystemData::AddTexture(VTexture* tx, float dens)
{
	textures[curtex] = tx;
	if (curtex == 0)
	{
		texdensity[curtex] = (int)(dens * 1000.0f);
	}
	else
	{
		texdensity[curtex] = texdensity[curtex-1] + (int)(dens * 1000.0f);
	}
	if (tx->bytes != 0 && tx->glID == 0)
	{
		tx->gettexid();
	}
	
	curtex++;
}

VTexture* TEcosystemData::GetRandomTexture()
{
	int rd = rand() % 1001;
	int i;
	int curdata;
	VTexture* curtex;
	
	for (i=0;i<texcount;i++)
	{
		curdata = texdensity[i];
		curtex = textures[i];
		if (rd < curdata)
		{
			return curtex;
		}
	}
	
	return curtex;	
}

TEcosystemNode::TEcosystemNode()
{
	parentnode = 0;
	firstnode = 0;
	lastnode = 0;
	nextnode = 0;
	prevnode = 0;
	nodecount = 0;
	altitude = 0.0f;
	data = 0;
}



void TEcosystemNode::AddNode(TEcosystemNode* nd)
{
	if (firstnode == 0)
	{
		firstnode = nd;
		lastnode = nd;
		nd->nextnode = 0;
		nd->prevnode = 0;
		nodecount++;
	}
	else
	{
		lastnode->nextnode = nd;
		nd->nextnode = 0;
		nd->prevnode = lastnode;
		lastnode = nd;
		nodecount++;
	}
}

#define FLT_CMP(a,b,prec) ((fabs(a - b) < prec))

TEcosystemNode* TEcosystem::AddNode(float alt,float slope,float temp)
{
	int i,j,k;
	TEcosystemNode* curnode = root.firstnode;
	int foundlevel=0;
	
	for (i=0;i<root.nodecount;i++)
	{
		if (FLT_CMP(curnode->altitude,alt,0.0001f))
		{
			foundlevel++;
			break;
		}
		curnode = curnode->nextnode;
	}

	if (foundlevel==1)
	{
		TEcosystemNode* curnode2 = curnode->firstnode;
		
		for (i=0;i<curnode->nodecount;i++)
		{
			if (FLT_CMP(curnode2->startslope,slope,0.0001f))
			{
				foundlevel++;
				break;
			}
			curnode2 = curnode2->nextnode;
		}

		if (foundlevel == 2)
		{
			TEcosystemNode* curnode3 = curnode2->firstnode;
			
			for (i=0;i<curnode2->nodecount;i++)
			{
				if (FLT_CMP(curnode3->temperature,temp,0.0001f))
				{
					foundlevel++;
					break;
				}
				curnode3 = curnode3->nextnode;
			}

			if (foundlevel == 3)
			{
				// return this
				return curnode3;
			}
			else
			{
				// add from level 2
				TEcosystemNode* bosnode3;
				bosnode3 = new TEcosystemNode;
				bosnode3->temperature = temp;
				curnode2->AddNode(bosnode3);
				
				bosnode3->data = new TEcosystemData;
				bosnode3->data->owner = bosnode3;

				return bosnode3;
			}
		}
		else
		{
			// add from level 1
			TEcosystemNode* bosnode2;
			bosnode2 = new TEcosystemNode;
			bosnode2->startslope = slope;
			curnode->AddNode(bosnode2);
			
			TEcosystemNode* bosnode3;
			bosnode3 = new TEcosystemNode;
			bosnode3->temperature = temp;
			bosnode2->AddNode(bosnode3);
			
			bosnode3->data = new TEcosystemData;
			bosnode3->data->owner = bosnode3;

			return bosnode3;
		}
	}
	else
	{
		// add from level 0
		TEcosystemNode* bosnode;
		bosnode = new TEcosystemNode;
		bosnode->altitude = alt;
		root.AddNode(bosnode);
		
		TEcosystemNode* bosnode2;
		bosnode2 = new TEcosystemNode;
		bosnode2->startslope = slope;
		bosnode->AddNode(bosnode2);
		
		TEcosystemNode* bosnode3;
		bosnode3 = new TEcosystemNode;
		bosnode3->temperature = temp;
		bosnode2->AddNode(bosnode3);
		
		bosnode3->data = new TEcosystemData;
		bosnode3->data->owner = bosnode3;

		return bosnode3;
	}
}

void TEcosystem::Create3DTexture()
{
	RArray tarray;

	int i,j,k;
	TEcosystemNode *balt;
	TEcosystemNode *bslp;
	
	balt = root.firstnode;
	int totaltexs=0;
	for (i=0;i<root.nodecount;i++)
	{
		bslp = balt->firstnode;
		for (j=0;j<balt->nodecount;j++)
		{
			tarray.add(bslp->firstnode->data);
			totaltexs += bslp->firstnode->data->texcount;
			bslp = bslp->nextnode;
		}

		balt = balt->nextnode;
	}
	
	layers3d = totaltexs;
	oldglids = new dword [layers3d];
	rcoords = new float [layers3d];
	texpixels = new byte [128 * 128 * 3 * layers3d];

	byte* curp=texpixels;
	TEcosystemData* curdata;
	VTexture* curtex;
	int curt=0;
	for (i=0;i<tarray.count;i++)
	{
		curdata = (TEcosystemData*)tarray.pArray[i];
		for (j=0;j<curdata->texcount;j++)
		{
			curtex = curdata->textures[j];
			oldglids[curt] = curtex->glID;
			rcoords[curt] = (float)curt / ((float)layers3d-0.5f);
			memcpy(curp,curtex->bytes,curtex->bytsize);
			curp += curtex->bytsize;
			curt++;
		}
	}
	
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &texture3d);
	glBindTexture(GL_TEXTURE_3D, texture3d);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 128, 128, layers3d, 0, GL_RGB, GL_UNSIGNED_BYTE, texpixels);
	glDisable(GL_TEXTURE_3D);
}	

float TEcosystem::GetRCoord(dword glid)
{
	int i;
	for (i=0;i<layers3d;i++)
	{
		if (oldglids[i] == glid)
		{
			return rcoords[i];
		}
	}

	return 0.0f;
}

TEcosystemNode* TEcosystemNode::GetNode(float dt)
{
	int i;
	TEcosystemNode* curnode = firstnode;
	
	for (i=0;i<nodecount;i++)
	{
		if (dt < curnode->altitude)
		{
			curnode = curnode->prevnode;
			break;
		}
		if (curnode->nextnode == 0)
		{
			return curnode;
		}
		curnode = curnode->nextnode;
	}

	return curnode;

}

// this will find best possible data
TEcosystemData* TEcosystem::GetNode(float alt,float slope,float temp)
{
	int i;
	TEcosystemNode* curnode;
	curnode = root.GetNode(alt);
	curnode = curnode->GetNode(slope);
	curnode = curnode->GetNode(temp);

	return curnode->data;
}


VTerrain::VTerrain()
{
	n = 0;
	width = 0;
	height = 0;
	heightmap = 0;
	normx = 0;
	normy = 0;
	normz = 0;
	txu = 0;
	txv = 0;
	lod = 0;
}

VTerrain::~VTerrain()
{
	if (normx)
		delete [] normx;

	if (normy)
		delete [] normy;

	if (normz)
		delete [] normz;

	if (heightmap)
		delete [] heightmap;

	if (txu)
		delete [] txu;

	if (txv)
		delete [] txv;
}

float VTerrain::GetHeight(int x,int y)
{
	return heightmap[ (y<<n) + x];
}

void VTerrain::SetHeight(int x,int y,float h)
{
	heightmap[ (y<<n) + x] = h;
}

#define setar(ar,x,y,h)	(*(ar+ (y<<n) + x) = h);	
#define getar(ar,x,y) (*(ar+(y<<n) + x)))

/*
Topography

+---+---+---+
| \ | / | \ |
|  \|/  |  \|
+---+---+---+
|  /|\  |  /|
| / | \ | / |
+---+---+---+
| \ | / | \ |
|  \|/  |  \|
+---+---+---+
*/

void VTerrain::CalculateNormals()
{
	int i;
	int asize = width * height; // array size
	normx = new float [asize]; // vertex normals
	normy = new float [asize];
	normz = new float [asize];

	for (i=0;i<asize;i++)
	{
		normz[i] = normy[i] = normx[i] = 0.0f;
	}

	/*float *fnx,*fny,*fnz; // face normals
	asize = (width-1) * (height-1);
	fnx = new float [asize*2];
	fny = new float [asize*2];
	fnz = new float [asize*2];*/

	int x,y;
	dword memp;

	vec3 v1,v2,v3,v4;
	vec3 t1,t2;
	vec3 nrm;
	
	float h1,h2,h3,h4;

	// generate face normals
	int dsp=0;
	dword nv1,nv2,nv3,nv4;

	for (y=0;y<height-1;y++)
	{
		for (x=0;x<width-1;x++)
		{
			nv1 = (y<<n) + x;
			nv2 = (y<<n) + x+1;
			nv3 = ((y+1)<<n) + x+1;
			nv4 = ((y+1)<<n) + x;

			v1.set(x,heightmap[nv1],y);
			v2.set(x+1,heightmap[nv2],y);
			v3.set(x+1,heightmap[nv3],y+1);
			v4.set(x,heightmap[nv4],y+1);
			

			t1 = v2-v1;
			t2 = v2-v4;
			nrm = cross(t1,t2);
			nrm.normalize();
			
			normx[nv1] += nrm.x;
			normy[nv1] += nrm.y;
			normz[nv1] += nrm.z;
			normx[nv2] += nrm.x;
			normy[nv2] += nrm.y;
			normz[nv2] += nrm.z;
			normx[nv4] += nrm.x;
			normy[nv4] += nrm.y;
			normz[nv4] += nrm.z;


			t1 = v2-v3;
			t2 = v2-v4;
			nrm = cross(t2,t1);
			nrm.normalize();

			normx[nv2] += nrm.x;
			normy[nv2] += nrm.y;
			normz[nv2] += nrm.z;
			normx[nv3] += nrm.x;
			normy[nv3] += nrm.y;
			normz[nv3] += nrm.z;
			normx[nv4] += nrm.x;
			normy[nv4] += nrm.y;
			normz[nv4] += nrm.z;
		}
	}

	for (y=1;y<height-1;y++)
	{
		for (x=1;x<width-1;x++)
		{
			nv1 = (y<<n) + x;

			normx[nv1] /= 6.0f;
			normy[nv1] /= 6.0f;
			normz[nv1] /= 6.0f;
		}
	}

	for (x=1;x<width-1;x++)
	{
		nv1 = x;
		nv2 = ( ((height-1)<<n) +x );

		normx[nv1] /= 3.0f;
		normy[nv1] /= 3.0f;
		normz[nv1] /= 3.0f;

		normx[nv2] /= 3.0f;
		normy[nv2] /= 3.0f;
		normz[nv2] /= 3.0f;
		
	}

	for (y=1;y<height-1;y++)
	{
		nv1 = (y<<n);
		nv2 = (y<<n) + (width-1);
		
		normx[nv1] /= 3.0f;
		normy[nv1] /= 3.0f;
		normz[nv1] /= 3.0f;
		
		normx[nv2] /= 3.0f;
		normy[nv2] /= 3.0f;
		normz[nv2] /= 3.0f;
	}

	// kosegen fix
	nv1 = width-1;
	nv2 = ((height-1)<<n);
	normx[nv1] /= 2.0f;
	normy[nv1] /= 2.0f;
	normz[nv1] /= 2.0f;

	normx[nv2] /= 2.0f;
	normy[nv2] /= 2.0f;
	normz[nv2] /= 2.0f;

	vec3 n;
	for (i=0;i<asize;i++)
	{
		n.x = normx[i];
		n.y = normy[i];
		n.z = normz[i];
		n.normalize();
		normx[i] = n.x;
		normy[i] = n.y;
		normz[i] = n.z;
	}

}

void VTerrain::LoadBaseTexture(char* fpath)
{
	synths = new VTexture;
	synths->loadbmp("testterra.bmp");
	synths->gettexid();

	int x,y,nv1,tn,pixelpertile;
	tn = synths->twidth;
	pixelpertile = tn / width;
	
	txu = new float [width * height];
	txv = new float [width * height];
	for (y=0;y<height;y++)
	{
		for (x=0;x<width;x++)
		{
			nv1 = (y<<n) + x;
			txu[nv1] = (float)(x * pixelpertile) / (float)tn;
			txv[nv1] = (float)(y * pixelpertile) / (float)tn;
		}
	}
}

void VTerrain::CalculateTextures()
{
	int x,y;
	texids = new int [(width) * (height)];

	vec3 v1,v2,v3,v4;
	
	float nx,ny,nz;
	dword nv1,nv2,nv3,nv4;
	
	float mid;
	float ang;
	TEcosystemData* tp; // TP for your bunghole

	vec3 n1,n2,n3,n4,nf;
	vec3 p;
	p.set(0,1,0);

	int* a = texids;
	VTexture* rndtex;

	for (y=0;y<height-1;y++)
	{
		for (x=0;x<width-1;x++)
		{
			
			nv1 = (y<<n) + x;
			nv2 = (y<<n) + x+1;
			nv3 = ((y+1)<<n) + x+1;
			nv4 = ((y+1)<<n) + x;

			v1.set(x,heightmap[nv1],y);
			v2.set(x+1,heightmap[nv2],y);
			v3.set(x+1,heightmap[nv3],y+1);
			v4.set(x,heightmap[nv4],y+1);
			
			n1.set(normx[nv1],normy[nv1],normz[nv1]);
			n2.set(normx[nv2],normy[nv2],normz[nv2]);
			n3.set(normx[nv3],normy[nv3],normz[nv3]);
			n4.set(normx[nv4],normy[nv4],normz[nv4]);
			nf = (n1+n2+n3+n4) * 0.25f ;
			ang = acos(dot(nf,p));
			ang = RADTODEG(ang);
			mid = (v1.y + v2.y+ v3.y + v4.y) / 4.0f;
			
			tp = ecosystem->GetNode(mid,ang,0.0f);
			if (tp)
			{
				rndtex = tp->GetRandomTexture();
				texids[nv1] = rndtex->glID;
				//*a = rndtex->glID;
			}
			else
			{
				texids[nv1] = 0;
				*a  = 0;
			}

			a++;
		}
	}
}

void subimage(VTexture* src,word fx,word fy,word tx,word ty,word w,word h,VTexture* dst)
{
	word sx,sy;
	word dx,dy;
	long clrs;
	VColor32Bit t32;
	VColor24Bit t24;
	
	for (sy=fy+h,dy=ty;sy>=fy,dy<ty+h;sy--,dy++)
	{
		for (sx=fx,dx=tx;sx<fx+w,dx<tx+w;sx++,dx++)
		{
			clrs = src->getpixel(sx,sy);
			dst->setpixel(dx,dy,clrs);
		}
	}
}

void VTerrain::SynthesizeBaseTexture()
{
	// laterly we will organize it by sectors
	int tn;
	tn = 2048;

	VTexture* synth = new VTexture;
	VTexture* subsynth = new VTexture;
	synth->createbuffer(2048,2048,GL_RGB);
	subsynth->createbuffer(512,512,GL_RGB);
	subsynth->gettexid();
	


	glViewport(0,0,512,512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// enter 2d
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();									
	glLoadIdentity();									
	glOrtho(0,512,512,0,-1,1);		
	glMatrixMode(GL_MODELVIEW);						
	glPushMatrix();
	glDisable(GL_LIGHTING);


	dword nv1,nv2,nv3,nv4;

	int x,y;
	int xs;
	int ys;

	int xs2,xe2;
	int ys2,ye2;
	int scrx,scry;



	float uv1[] = { 0.0f, 0.0f };
	float uv2[] = { 1.0f, 0.0f };
	float uv3[] = { 1.0f, 1.0f };
	float uv4[] = { 0.0f, 1.0f };

	int pixelpertile = tn / width; 
	int tilepersector = 512 / pixelpertile;
	int sectors = tn / 512; // 4

	for ( ys=0;ys<sectors;ys++ )
	{
		ys2 = ys * tilepersector;
		ye2 = (ys+1)  * tilepersector;
		for (xs=0;xs<sectors;xs++)
		{
			xs2 = xs * tilepersector;
			xe2 = (xs+1) * tilepersector;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);
			scrx = 0;
			scry = 0;
			for (y = ys2;y<ye2;y++)
			{
				scrx = 0;
				for (x = xs2;x<xe2;x++)
				{
					nv1 = (y<<n) + x;


					glBindTexture(GL_TEXTURE_2D,texids[nv1]);

					glBegin(GL_QUADS);
						glTexCoord2fv(uv1);
						glVertex2f(scrx*pixelpertile,scry*pixelpertile);
						
						glTexCoord2fv(uv2);
						glVertex2f((scrx+1)*pixelpertile,scry*pixelpertile);

						glTexCoord2fv(uv3);
						glVertex2f((scrx+1)*pixelpertile,(scry+1)*pixelpertile);
						
						glTexCoord2fv(uv4);
						glVertex2f(scrx*pixelpertile,(scry+1)*pixelpertile);
					glEnd();
					
					scrx++;
				}
				scry++;
			}

			

			glBindTexture(GL_TEXTURE_2D,subsynth->glID);
			glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,512,512,0);
			glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,subsynth->bytes);
			subsynth->writebmp("testterra1.bmp");
			

			subimage(subsynth,0,0,xs*512,ys*512,512,512,synth);
		}
	}

	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);					
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);						
	glPopMatrix();									
	glEnable(GL_DEPTH_TEST);

	glViewport(0,0,mEngine.rdWidth,mEngine.rdHeight);

	synth->writebmp("testterra.bmp");
	//synth->loadbmp("testterra.bmp");
	synths = synth;
	synths->gettexid();

	txu = new float [width * height];
	txv = new float [width * height];
	for (y=0;y<height;y++)
	{
		for (x=0;x<width;x++)
		{
			nv1 = (y<<n) + x;
			txu[nv1] = (float)(x * pixelpertile) / (float)tn;
			txv[nv1] = (float)(y * pixelpertile) / (float)tn;
		}
	}
}

// min1 deðer aralaginida girilen bi degeri min2 deger araligina cevirir
float clamps(float min1,float max1,float min2,float max2,float t)
{
	float t1,t2,adj;
	float r;
	t1 = max1-min1;
	t2 = max2-min2;
	adj = t - min1;
	r = (t2 * adj) / t1;
	r += min2;
	/*if (r > max2)
	{
		r = max2;
	}
	if (r< min2)
	{
		r = min2;
	}*/
	return r;
}

void VTerrain::Calculate3DTexture()
{
	int x,y;	
	vec3 v1,v2,v3,v4;
	
	float nx,ny,nz;
	dword nv1,nv2,nv3,nv4;
	
	float mid;
	float ang;
	TEcosystemData* tp; // TP for your bunghole
	
	vec3 n1;
	vec3 p;
	p.set(0,1,0);
	
	txs = new float [height * width];

	float* a = txs;
	VTexture* rndtex;

	
	for (y=0;y<height-1;y++)
	{
		for (x=0;x<width-1;x++)
		{			
			nv1 = (y<<n) + x;
			
			v1.set(x,heightmap[nv1],y);
			
			n1.set(normx[nv1],normy[nv1],normz[nv1]);

			ang = acos(dot(n1,p));
			ang = RADTODEG(ang);
			ang = fabs(ang);
			mid = v1.y;
			
			/*tp = ecosystem->GetNode(mid,ang,0.0f);
			if (tp)
			{
				rndtex = tp->GetRandomTexture();
				//*a =  clamps(miny,maxy,0.0f,1.0f,mid);
				//*a = ecosystem->GetRCoord(rndtex->glID) / 6.0f;
				txs[nv1] = ecosystem->GetRCoord(rndtex->glID);

			}
			else
			{
				*a  = 0.0f;
			}*/
			if (mid < 0)
			{
				txs[nv1] = 0.0f;
			}
			else
			{
				txs[nv1] = clamps(miny,maxy,0.001f,0.999f,mid);
				if (txs[nv1] < 0.22f)
				{
					txs[nv1] = 0.23f;
				}	
			}

			//*a =  mid / maxy;//clamps(miny,maxy,0.0f,6.0f,mid);*/
			//a++;
		}
	}	
}

void VTerrain::GenerateFromHeightMap(VTexture* hmap)
{
	width = hmap->twidth;
	height = hmap->theight;

	n = PowerOfTwo((dword)width);



	heightmap = new float [width * height];

	miny = 999999999.0f;
	maxy = -999999999.0f;

	color c;
	int x,y;
	float bd;
	dword memp;
	for (y=0;y<height;y++)
	{
		for (x=0;x<width;x++)
		{
			c.lclr = hmap->getpixel(x,y);
			bd = (float)(c.bclr[CRED]);
			bd = (bd-32.0f) / 16.0f;
			if (bd < miny)
			{
				miny = bd;
			}
			if (bd > maxy)
			{
				maxy = bd;
			}
			SetHeight(x,y,bd);
			//memp = y<<n + x;
			//heightmap[memp] = (bd-75.0f) / 32.0f;	
		}
	}
	CalculateNormals();
	CalculateTextures();
	Calculate3DTexture();


}

void VTerrain::RenderWithBaseTexture()
{
	int x,y;
	
	sx = floor(cam->xPos) - 128;
	ex = floor(cam->xPos) + 128;
	
	sy = floor(cam->zPos) - 128;
	ey = floor(cam->zPos) + 128;
	
	if ( sx < 0)
		sx = 0;
	
	if ( ex > width-1)
		ex = width-1;
	
	if ( sy < 0)
		sy = 0;
	
	if ( ey > height-1)
		ey = height-1;
	
	vec3 v1,v2,v3,v4;
	vec3 t1,t2;
	vec3 nrm;
	
	
	float nx,ny,nz;
	dword nv1,nv2,nv3,nv4;
	
	bool duz =true;
	
	float mid;
	float ang;
	TEcosystem* tp; // TP for your bunghole
	float uv1[] = { 0.0f, 0.0f };
	float uv2[] = { 1.0f, 0.0f };
	float uv3[] = { 1.0f, 1.0f };
	float uv4[] = { 0.0f, 1.0f };
	
	vec3 n1,n2,n3,n4,nf;
	vec3 p;
	p.set(0,1,0);

	glBindTexture(GL_TEXTURE_2D,synths->glID);
	glBegin(GL_TRIANGLES);

	for (y=sy;y<ey;y++)
	{
		for (x=sx;x<ex;x++)
		{
			glColor3f(1.0f,1.0f,1.0f);
			
			nv1 = (y<<n) + x;
			nv2 = (y<<n) + x+1;
			nv3 = ((y+1)<<n) + x+1;
			nv4 = ((y+1)<<n) + x;
			
			v1.set(x,heightmap[nv1],y);
			v2.set(x+1,heightmap[nv2],y);
			v3.set(x+1,heightmap[nv3],y+1);
			v4.set(x,heightmap[nv4],y+1);

			
			

			

			
			
			glNormal3f(normx[nv1],normy[nv1],normz[nv1]);
			glTexCoord2f(txu[nv1],txv[nv1]);
			v1.render();
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord2f(txu[nv2],txv[nv2]);
			v2.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord2f(txu[nv4],txv[nv4]);
			v4.render();
			
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord2f(txu[nv2],txv[nv2]);
			v2.render();
			glNormal3f(normx[nv3],normy[nv3],normz[nv3]);
			glTexCoord2f(txu[nv3],txv[nv3]);
			v3.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord2f(txu[nv4],txv[nv4]);
			v4.render();
			
			
		}
	}
	
	glEnd();

	RenderFakeWater();
	
}

void VTerrain::Render()
{
	int x,y;

	sx = floor(cam->xPos) - 128;
	ex = floor(cam->xPos) + 128;
	
	sy = floor(cam->zPos) - 128;
	ey = floor(cam->zPos) + 128;

	if ( sx < 0)
		sx = 0;
	
	if ( ex > width-1)
		ex = width-1;

	if ( sy < 0)
		sy = 0;
	
	if ( ey > height-1)
		ey = height-1;

	vec3 v1,v2,v3,v4;
	vec3 t1,t2;
	vec3 nrm;


	float nx,ny,nz;
	dword nv1,nv2,nv3,nv4;
	
	bool duz =true;

	float mid;
	float ang;
	TEcosystem* tp; // TP for your bunghole
	float uv1[] = { 0.0f, 0.0f };
	float uv2[] = { 1.0f, 0.0f };
	float uv3[] = { 1.0f, 1.0f };
	float uv4[] = { 0.0f, 1.0f };

	vec3 n1,n2,n3,n4,nf;
	vec3 p;
	p.set(0,1,0);

	for (y=sy;y<ey;y++)
	{
		for (x=sx;x<ex;x++)
		{
			glColor3f(1.0f,1.0f,1.0f);

			nv1 = (y<<n) + x;
			nv2 = (y<<n) + x+1;
			nv3 = ((y+1)<<n) + x+1;
			nv4 = ((y+1)<<n) + x;
			
			v1.set(x,heightmap[nv1],y);
			v2.set(x+1,heightmap[nv2],y);
			v3.set(x+1,heightmap[nv3],y+1);
			v4.set(x,heightmap[nv4],y+1);


			glBindTexture(GL_TEXTURE_2D,texids[nv1]);

			glBegin(GL_TRIANGLES);
			
			
			glNormal3f(normx[nv1],normy[nv1],normz[nv1]);
			glTexCoord2fv(uv1);
			v1.render();
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord2fv(uv2);
			v2.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord2fv(uv4);
			v4.render();
			
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord2fv(uv2);
			v2.render();
			glNormal3f(normx[nv3],normy[nv3],normz[nv3]);
			glTexCoord2fv(uv3);
			v3.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord2fv(uv4);
			v4.render();

			glEnd();
		
		}
	}

	RenderFakeWater();

}

void VTerrain::RenderWith3DTexture()
{
	int x,y;

	/*sx = floor(cam->xPos) - 128;
	ex = floor(cam->xPos) + 128;
	
	sy = floor(cam->zPos) - 128;
	ey = floor(cam->zPos) + 128;*/

	sx = -500;
	ex = 550;
	sy = -500;
	ey = 500;

	if ( sx < 0)
		sx = 0;
	
	if ( ex > width-1)
		ex = width-1;

	if ( sy < 0)
		sy = 0;
	
	if ( ey > height-1)
		ey = height-1;

	vec3 v1,v2,v3,v4;
	vec3 t1,t2;
	vec3 nrm;


	float nx,ny,nz;
	dword nv1,nv2,nv3,nv4;
	
	bool duz =true;

	float mid;
	float ang;
	TEcosystem* tp; // TP for your bunghole
	float uv1[] = { 0.0f, 0.0f ,0.0f };
	float uv2[] = { 1.0f, 0.0f ,0.0f };
	float uv3[] = { 1.0f, 1.0f ,0.0f };
	float uv4[] = { 0.0f, 1.0f ,0.0f };

	vec3 n1,n2,n3,n4,nf;
	vec3 p;
	p.set(0,1,0);

	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, ecosystem->texture3d);
	glBegin(GL_TRIANGLES);

	for (y=sy;y<ey;y++)
	{
		for (x=sx;x<ex;x++)
		{

			nv1 = (y<<n) + x;
			nv2 = (y<<n) + x+1;
			nv3 = ((y+1)<<n) + x+1;
			nv4 = ((y+1)<<n) + x;
			
			v1.set(x,heightmap[nv1],y);
			v2.set(x+1,heightmap[nv2],y);
			v3.set(x+1,heightmap[nv3],y+1);
			v4.set(x,heightmap[nv4],y+1);

			uv1[2] = txs[nv1];
			uv2[2] = txs[nv2];
			uv3[2] = txs[nv3];
			uv4[2] = txs[nv4];

			/*uv1[2] = clamps(miny,maxy,0.0f,1.0f,v1.y);
			uv2[2] = clamps(miny,maxy,0.0f,1.0f,v2.y);
			uv3[2] = clamps(miny,maxy,0.0f,1.0f,v3.y);
			uv4[2] = clamps(miny,maxy,0.0f,1.0f,v4.y);*/
			
			glNormal3f(normx[nv1],normy[nv1],normz[nv1]);
			glTexCoord3fv(uv1);
			v1.render();
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord3fv(uv2);
			v2.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord3fv(uv4);
			v4.render();
			
			glNormal3f(normx[nv2],normy[nv2],normz[nv2]);
			glTexCoord3fv(uv2);
			v2.render();
			glNormal3f(normx[nv3],normy[nv3],normz[nv3]);
			glTexCoord3fv(uv3);
			v3.render();
			glNormal3f(normx[nv4],normy[nv4],normz[nv4]);
			glTexCoord3fv(uv4);
			v4.render();

			
		
		}
	}

	glEnd();
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_LIGHTING);

	RenderFakeWater();
}

void VTerrain::RenderFakeWater()
{
	/// draaw sea
	glBindTexture(GL_TEXTURE_2D,0);
	
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);		
	
	glColor4f(0.4f,0.4f,0.7f,0.5f);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f(sx,0.0f,sy);
	glVertex3f(ex,0.0f,sy);
	glVertex3f(ex,0.0f,ey);
	glVertex3f(sx,0.0f,ey);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}