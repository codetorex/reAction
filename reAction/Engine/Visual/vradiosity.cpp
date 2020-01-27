#include "stdafx.h"
#include "vmodel.h"
#include "vradiosity.h"


void VREnvironment::SetupEnvironment(RMDModel& eMdl, VSkyBox& eSky, VCamera &eCam)
{
	mdl = &eMdl;
	sky = &eSky;
	cam = &eCam;
	vrad->mdl = mdl;
}

void VREnvironment::LoadHemicube(char* fpath1,char* fpath2)
{
	hemicube[RAD_FRONT].loadbmp(fpath1);
	hemicube[RAD_FRONT].gettexid();

	hemicube[RAD_BOTTOM].loadbmp(fpath2);
	hemicube[RAD_BOTTOM].gettexid();

	empty.createbuffer(DETAIL_SIZE,DETAIL_SIZE,GL_RGB);
	empty.gettexid();
}

void VREnvironment::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	cam->render();	
	
	glDisable(GL_DEPTH_TEST);
	sky->RenderSkyBox();
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	vrad->TestRender();
	glDisable(GL_CULL_FACE);
}

void VREnvironment::Render2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	cam->render();	

	glListBase(0);
	glCallList(rbase);
}

void VREnvironment::Precache()
{
	rbase = glGenLists(1);
	glNewList(rbase,GL_COMPILE);
		glDisable(GL_DEPTH_TEST);
		sky->RenderSkyBox();
		glEnable(GL_DEPTH_TEST);
		
		glEnable(GL_CULL_FACE);
		vrad->TestRender();
		glDisable(GL_CULL_FACE);
	glEndList();
}

void VREnvironment::RenderMultiplier(int direction)
{
	// enter2d
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();									
	glLoadIdentity();									
	glOrtho(0,DETAIL_SIZE,DETAIL_SIZE,0,-1,1);		
	glMatrixMode(GL_MODELVIEW);						
	glPushMatrix();		
	
	glEnable(GL_BLEND);	
	glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
	
	// render
	glLoadIdentity();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,hemicube[direction].glID);
	glBegin(GL_QUADS);

	switch(direction)
	{
		case RAD_FRONT:
			DrawQuad(0,0,(float)DETAIL_SIZE,(float)DETAIL_SIZE,0.0f,0.0f,1.0f,1.0f);
			break;

		case RAD_BOTTOM:
			DrawQuad(0,0,(float)DETAIL_SIZE,(float)(DETAIL_SIZE/2),0.0f,0.0f,1.0f,1.0f);
			break;
	}
	
	glEnd();
	
	// exit2d
	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);					
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);						
	glPopMatrix();									
	glEnable(GL_DEPTH_TEST);
}

void VREnvironment::GatherTexture(bool half,vec3 OUT &clr)
{
	glBindTexture(GL_TEXTURE_2D,empty.glID);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, DETAIL_SIZE, DETAIL_SIZE, 0);
	
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,empty.bytes);
	byte* ar = empty.bytes;
	int r,g,b;
	int r2,g2,b2;
	r = g = b = 0;
	int powdsize = DETAIL_SIZE * DETAIL_SIZE;
	if (half)
	{
		int lim = powdsize / 2;
		for (int i = 0;i<lim;i++)	
		{
			r += ar[0];
			g += ar[1];
			b += ar[2];
			ar+=3;
		}
		r /= lim;
		g /= lim;
		b /= lim;
	}
	else
	{
		int lim = powdsize;
		for (int i = 0;i<lim;i++)	
		{
			r += ar[0];
			g += ar[1];
			b += ar[2];
			ar+=3;
		}
		r /= lim;
		g /= lim;
		b /= lim;
	}
	clr.x = (float)r;
	clr.y = (float)g;
	clr.z = (float)b;
}

void VRFace::SetData(RMDMesh* cmesh, int face)
{
	obj = cmesh;
	faceid = face;

	cmesh->GetTriangle(face,&triangle);
	cmesh->CalculateTangentSpace(face,fnor,ftan,fbin);
}

/*void cperp (vec3 *v1)
{
	if ( (v1->y >= 0.99f || v1->y <= -0.99f) && v1->x <= 0.001 && v1->z <= 0.001 )
	{
		v1->x = 0.0001f;
	}
}*/

char tmpwnd[512];
int d;

void VRFace::Radiate(int px,int py, vec3 &p3d,VRLightmap* lmap)
{
	//sprintf(tmpwnd,"x: %i y: %i p: %i",px,py,d);
	//SetWindowText(mEngine.hWnd,tmpwnd);

	mEngine.DoEvents();
	d = GetTickCount();
	VREnvironment* renv = &lmap->vrad->radenv;

	vec3 offset = (fnor * 2.0f) + p3d; // camera direction
	cperp(&fbin);cperp(&fnor);cperp(&ftan);

	vec3 front,left,right,top,bottom;
	front = (fnor * 5.0f) + offset;
	left = (ftan * 5.0f) + offset;
	right = offset - (ftan * 5.0f);
	top = (fbin * 5.0f) + offset;
	bottom = offset - (fbin * 5.0f);

	renv->cam->SetCamera(offset,front,mv3f(0,-1,0));
	renv->Render();
	renv->RenderMultiplier(RAD_FRONT);
	renv->GatherTexture(false,front);
	//SwapBuffers(mEngine.hDC);

	//// LEFT
	/*renv->cam->SetCamera(offset,left,fnor);
	renv->Render();
	renv->RenderMultiplier(RAD_BOTTOM);
	renv->GatherTexture(true,left);
	
	////// RIGHT
	renv->cam->SetCamera(offset,right,fnor);
	renv->Render();
	renv->RenderMultiplier(RAD_BOTTOM);
	renv->GatherTexture(true,right);
	
	////// TOP
	renv->cam->SetCamera(offset,top,fnor);
	renv->Render();
	renv->RenderMultiplier(RAD_BOTTOM);
	renv->GatherTexture(true,top);
	
	////// BOTTOM
	renv->cam->SetCamera(offset,bottom,fnor);
	renv->Render();
	renv->RenderMultiplier(RAD_BOTTOM);
	renv->GatherTexture(true,bottom);*/

	vec3 res;
	res = (front); //+ ((left + right + top + bottom) * 2.0f);
	//res = res / 3.0f;

	VColor24Bit cpix;
	
	cpix = lmap->sinitial->getpixel(px,py);
	res.x += (float)cpix.r;
	res.y += (float)cpix.g;
	res.z += (float)cpix.b;
	if (res.x > 255.0f) res.x = 255.0f;
	if (res.y > 255.0f) res.y = 255.0f;
	if (res.z > 255.0f) res.z = 255.0f;
	lmap->stexture->setpixel(px,py,RGB((byte)res.x,(byte)res.y,(byte)res.z));
	d = GetTickCount() - d;
}

void VRFace::TraceLightRay(ray3 &lray, RMDLight& lght, int px,int py ,vec3 & p3d,VRLightmap* lmap)
{
	int o,f; // for iterate through objects and faces
	VRadiosity *vrad = lmap->vrad;
	RMDModel* mdl = vrad->mdl;
	RMDMesh* cmesh;
	VRFace* faces;
	VRFace* cface; // current face
	
	float d = distance(lray.p,p3d) - 1.50f;
	vec3 I; // intersection

	// TODO: optimize this with visibility tests...
	for (o=0;o<mdl->meshcount;o++)
	{
		cmesh = &mdl->meshes[o];
		faces = vrad->faces[o];
		for (f=0;f<cmesh->facecount;f++)
		{
			cface = &(faces[f]);
			/*if (cface == this)
			{
				continue;
			}*/

			if (cface->triangle.IntersectRay(lray,I))
			{
				if ( distance(lray.p,I) < d )
				{
					return; // its shadow not needed to add light
				}
			}
		}
	}
	
	vec3 Vrel = lray.p - p3d;//lray.d;
	Vrel.normalize();
	float cost = dot(Vrel,fnor); // cos theta
	if (cost < 0.0f) // back side
	{
		return;
	}
	float li = (cost) / (d*d); // light intensity
	
	vec3 result;
	result.x = (float)lght.clr.bclr[CRED];
	result.y = (float)lght.clr.bclr[CGRN];
	result.z = (float)lght.clr.bclr[CBLU];
	result = (result * 10000.0f * li );

	if (result.x > 255.0f) result.x = 255.0f;
	if (result.y > 255.0f) result.y = 255.0f;
	if (result.z > 255.0f) result.z = 255.0f;

	if (result.x < 0.0f) result.x = 0;
	if (result.y < 0.0f) result.y = 0;
	if (result.z < 0.0f) result.z = 0;

	color cresult;
	cresult.bclr[CRED] = (byte)result.x;
	cresult.bclr[CGRN] = (byte)result.y;
	cresult.bclr[CBLU] = (byte)result.z;
	
	//lmap->sinitial->setpixel(px,py,MONO(255));//RGB(255,255,255));
	lmap->sinitial->addpixel(px,py,cresult.lclr);
	//lmap->sinitial->setpixel(px,py,cresult.lclr);
}

void VRFace::VisulizeRay(ray3 &lray, RMDLight& lght, int px,int py ,vec3 & p3d,VRLightmap* lmap)
{
	vec3 t;
	glBegin(GL_LINES);
		glColor3f(0,1,0);
		lray.p.render();
		t = lray.p + (lray.d * 100.0f);
		t.render();
		
		glColor3f(1,0,0);
		lray.p.render();
		p3d.render();

		glColor3f(0,0,1);
		p3d.render();
		t = p3d + (fnor * 5.0f);
		t.render();

	glEnd();
}

void VRLinePart::CalculateInitialLight()
{
	int x,l;
	int lwidth = endx - startx; // line width
	vec3 p3d; // point in 3d
	ray3 lray; // light ray
	int y = vsline->y;

	VRLightmap* lmap = vsline->lmap; // light map
	RMDModel* mdl = lmap->vrad->mdl; // lol

	if (lwidth == 0)
	{
		p3d = startv;
		for (l=0;l<mdl->lightcount;l++)
		{
			lray.p = mdl->lights[l].lightpos;
			lray.d = p3d - lray.p;
			
			cface->TraceLightRay(lray,mdl->lights[l],startx,y,p3d,lmap);
		}
	}
	else
	{
		float stp = 1.0f / (float)(lwidth); // step
		float tween = stp / 2;
		for (x=startx;x <= endx;x++)
		{
			p3d = linip(startv,endv,tween);
			for (l=0;l<mdl->lightcount;l++)
			{
				lray.p = mdl->lights[l].lightpos;
				lray.d = p3d - lray.p;
				
				cface->TraceLightRay(lray,mdl->lights[l],x,y,p3d,lmap);
			}
			tween += stp;
		}
	}
}

void VRLinePart::CalculateRadiation()
{
	int x,l;
	int lwidth = endx - startx; // line width
	vec3 p3d; // point in 3d
	int y = vsline->y;
	
	VRLightmap* lmap = vsline->lmap; // light map
	RMDModel* mdl = lmap->vrad->mdl; // lol
	
	glViewport(0,0,DETAIL_SIZE,DETAIL_SIZE); // 128 * 128 MOVED TO HOST FUNCTION

	if (lwidth == 0)
	{
		p3d = startv;
		for (l=0;l<mdl->lightcount;l++)
		{	
			cface->Radiate(x,y,p3d,lmap);
		}
	}
	else
	{
		float stp = 1.0f / (float)(lwidth); // step
		float tween = stp / 2;
		for (x=startx;x <= endx;x++)
		{
			p3d = linip(startv,endv,tween);

			cface->Radiate(x,y,p3d,lmap);

			tween += stp;
		}
	}

	glViewport(0,0,mEngine.rdWidth,mEngine.rdHeight);
}

void VRLinePart::VisualizeRay(int x)
{
	x = x - startx;
	int l;
	int lwidth = endx - startx; // line width
	vec3 p3d; // point in 3d
	ray3 lray; // light ray
	int y = vsline->y;
	
	VRLightmap* lmap = vsline->lmap; // light map
	RMDModel* mdl = lmap->vrad->mdl; // lol

	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(0,1,0);
	
	if (lwidth == 0)
	{
		p3d = startv;
		for (l=0;l<mdl->lightcount;l++)
		{
			lray.p = mdl->lights[l].lightpos;
			lray.d = p3d - lray.p;
			
			cface->VisulizeRay(lray,mdl->lights[l],startx,y,p3d,lmap);
			
		}
	}
	else
	{
		float stp = 1.0f / (float)(lwidth); // step
		float tween = (stp / 2) * (float)x;

		p3d = linip(startv,endv,tween);
		for (l=0;l<mdl->lightcount;l++)
		{
			lray.p = mdl->lights[l].lightpos;
			lray.d = p3d - lray.p;
			
			cface->VisulizeRay(lray,mdl->lights[l],x,y,p3d,lmap);
		}
	}
		
}

VRLinePart* VRScanline::CreatePart()
{
	VRLinePart* npart = new VRLinePart;
	if (first==0)
	{
		first = npart;
		last = npart;
	}
	else
	{
		last->nextpart = npart;
		last = npart;
	}
	partcount++;
	npart->nextpart= 0;
	npart->vsline = this;
	return npart;
}

VRScanline::VRScanline()
{
	first = 0;
	last = 0;
	partcount = 0;
	y = 0;
}

void VRLightmap::InitializeLightmap(int width,int height)
{
	stexture = new VTexture;
	sinitial = new VTexture;
	stexture->createbuffer(width,height,GL_RGB);
	sinitial->createbuffer(width,height,GL_RGB);
	slines = new VRScanline [ height ];
	int i;
	for (i=0;i<height;i++)
	{
		slines[i].y = i;
		slines[i].lmap = this;
	}
	remainingarea = width * height;

	mainquad.x = 0;
	mainquad.y = 0;
	mainquad.width = width;
	mainquad.height = height;

	sinitial->gettexid(); // TODO: maybe some bad thing happens here
}

VRLinePart* VRLightmap::GetLinePart(int x,int y)
{
	VRScanline* cscan = &slines[y];
	VRLinePart* clpart;

	clpart = cscan->first;
	while(clpart != 0)
	{
		if (clpart->startx >= x && clpart->endx <= x)
		{
			return clpart;
		}
		clpart = clpart->nextpart;
	}

	return 0;
}

VRLightmap* VRLightmap::CreateNewLightmap(VRadiosity* rad)
{
	VRLightmap* csurface;
	csurface = new VRLightmap;
	csurface->InitializeLightmap(MAX_LIGHTMAPSIZE,MAX_LIGHTMAPSIZE);
	csurface->vrad = rad;
	csurface->is_ilc = false;
	return csurface;
}

VPackNode* VRLightmap::AllocateQuad(int width,int height)
{
	VPackNode* newquad;
	newquad = mainquad.InsertQuad(width,height);
	if (newquad)
	{
		sinitial->drawline(newquad->x,newquad->y,newquad->x+newquad->width,newquad->y,RGB(0,255,0));
		sinitial->drawline(newquad->x+newquad->width,newquad->y,newquad->x+newquad->width,newquad->y+newquad->height,RGB(0,255,0));
		sinitial->drawline(newquad->x+newquad->width,newquad->y+newquad->height,newquad->x,newquad->y+newquad->height,RGB(0,255,0));
		sinitial->drawline(newquad->x,newquad->y+newquad->height,newquad->x,newquad->y,RGB(0,255,0));



	}
	else
	{
		QuickInfo("oha");
	}
	return newquad;
}

// tool function. swaps two float
void SwapVec2(vec2 &a,vec2 &b)
{
	vec2 tmp;
	tmp = a;
	a = b;
	b = tmp;
}

// tool function for sort triangle vertices by y value
void SortTri2(tri2* v)
{
	if (v->v2.y <= v->v1.y) SwapVec2(v->v2,v->v1);
	if (v->v3.y <= v->v2.y) SwapVec2(v->v3,v->v2);
	if (v->v2.y <= v->v1.y) SwapVec2(v->v2,v->v1);
	
	if ( (v->v1.y > v->v2.y) || (v->v2.y > v->v3.y) || v->v1.y > v->v3.y)
	{
		QuickInfo("siralama error");
	}	
}

void VRLightmap::FillQuad(VRFace* fc,tri2* tSpc,VPackNode* pnode)
{
	int y;
	int sx,ex;
	sx = pnode->x;
	ex = sx + pnode->width;
	for (y=pnode->y;y<pnode->y+pnode->height;y++)
	{
		AddLinePart(y,sx,ex,fc,tSpc);
	}
}

/*void VRLightmap::FillTriangle(VRFace* fc,tri2* tSpc)
{
	int Sx,Sy,Ex,Ey;
	tri2 sorted;
	sorted = *tSpc;
	
	SortTri2(&sorted);

	int dx1,dx2,dx3;
	if (sorted.v2.y-sorted.v1.y > 0) dx1=(sorted.v2.x-sorted.v1.x)/(sorted.v2.y-sorted.v1.y); else dx1=sorted.v2.x - sorted.v1.x;
	if (sorted.v3.y-sorted.v1.y > 0) dx2=(sorted.v3.x-sorted.v1.x)/(sorted.v3.y-sorted.v1.y); else dx2=0;
	if (sorted.v3.y-sorted.v2.y > 0) dx3=(sorted.v3.x-sorted.v2.x)/(sorted.v3.y-sorted.v2.y); else dx3=0;
	
	Sx = Ex = sorted.v1.x;
	Sy = Ey = sorted.v1.y;
	
	if(dx1 > dx2) 
	{
		for(;Sy<=sorted.v2.y;Sy++,Ey++,Sx+=dx2,Ex+=dx1)
		{
			AddLinePart(Sy,Sx,Ex,fc,&sorted);
		}

		Ex=sorted.v2.x;
		Ey=sorted.v2.y;
		for(;Sy<=sorted.v3.y;Sy++,Ey++,Sx+=dx2,Ex+=dx3)
		{
			AddLinePart(Sy,Sx,Ex,fc,&sorted);
		}
	} 
	else 
	{
		for(;Sy<=sorted.v2.y;Sy++,Ey++,Sx+=dx1,Ex+=dx2)
		{
			AddLinePart(Sy,Sx,Ex,fc,&sorted);
		}
		
		Sx=sorted.v2.x;
		Sy=sorted.v2.y;
		for(;Sy<=sorted.v3.y;Sy++,Ey++,Sx+=dx3,Ex+=dx2)
		{
			AddLinePart(Sy,Sx,Ex,fc,&sorted);
		}
	}
}*/


int iround2(float x)
{
	int t;
	
	__asm
	{
		fld  x
		fistp t
	}
	
	return t;
}

void VRLightmap::FillTriangle(VRFace* fc,tri2* tSpc)
{
	INT64 X1 = iround2(16.0f * tSpc->v1.x);
	INT64 Y1 = iround2(16.0f * tSpc->v1.y);
	
	INT64 X2 = iround2(16.0f * tSpc->v2.x);
	INT64 Y2 = iround2(16.0f * tSpc->v2.y);
	
	INT64 X3 = iround2(16.0f * tSpc->v3.x);
	INT64 Y3 = iround2(16.0f * tSpc->v3.y);
	
    INT64 DX12 = X1 - X2;
    INT64 DX23 = X2 - X3;
    INT64 DX31 = X3 - X1;
	
    INT64 DY12 = Y1 - Y2;
    INT64 DY23 = Y2 - Y3;
    INT64 DY31 = Y3 - Y1;
	
    INT64 FDX12 = DX12 << 4;
    INT64 FDX23 = DX23 << 4;
    INT64 FDX31 = DX31 << 4;
	
    INT64 FDY12 = DY12 << 4;
    INT64 FDY23 = DY23 << 4;
    INT64 FDY31 = DY31 << 4;
	
    INT64 minx = (min(X1, min(X2, X3)) + 0xF) >> 4;
    INT64 maxx = (max(X1, max(X2, X3)) + 0xF) >> 4;
    INT64 miny = (min(Y1, min(Y2, Y3)) + 0xF) >> 4;
    INT64 maxy = (max(Y1, max(Y2, Y3)) + 0xF) >> 4;
	
    INT64 C1 = DY12 * X1 - DX12 * Y1;
    INT64 C2 = DY23 * X2 - DX23 * Y2;
    INT64 C3 = DY31 * X3 - DX31 * Y3;
	
    if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
    if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
    if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;
	
    INT64 CY1 = C1 + DX12 * (miny << 4) - DY12 * (minx << 4);
    INT64 CY2 = C2 + DX23 * (miny << 4) - DY23 * (minx << 4);
    INT64 CY3 = C3 + DX31 * (miny << 4) - DY31 * (minx << 4);
	
    for(int y = miny; y < maxy; y++)
    {
        INT64 CX1 = CY1;
        INT64 CX2 = CY2;
        INT64 CX3 = CY3;
		
        for(int x = minx; x < maxx; x++)
        {
            if(CX1 > 0 && CX2 > 0 && CX3 > 0)
            {
				//setpixel(x,y,pClr);
				//sinitial->setpixel(x,y,0xffffffff);
				AddLinePart(y,x,x,fc,tSpc);
			}
			
            CX1 -= FDY12;
            CX2 -= FDY23;
            CX3 -= FDY31;
        }
		
        CY1 += FDX12;
        CY2 += FDX23;
        CY3 += FDX31;
		
		
	}
}

void VRLightmap::AddLinePart(int y,int x1,int x2,VRFace* fc,tri2* tSpc)
{
	vec2 st;
	VRLinePart* npart = slines[y].CreatePart();
	npart->startx = x1;
	npart->endx = x2;
	npart->cface = fc;
	st.set(x1,y);
	npart->startv = tSpc->ProjectTri3(fc->triangle,st);
	if (x1 == x2)
	{
		npart->endv = npart->startv;
	}
	else
	{
		st.x = x2;
		npart->endv = tSpc->ProjectTri3(fc->triangle,st);
	}
	// todo: find startn endn
}

void VRLightmap::CalculateLightmap()
{
	int cy;
	VRLinePart* clpart; // current line party
	float prc = 512.0f / (float)(sinitial->theight);
	float sy;
	glEnable(GL_TEXTURE_2D);

	for (cy=0;cy<stexture->theight;cy++)
	{
		dword dt = GetTickCount(); // delta time
		clpart = slines[cy].first;
		if (is_ilc)
		{
			while(clpart)
			{
				clpart->CalculateRadiation();
				clpart = clpart->nextpart;
			}
			stexture->updatetex(true);
			glBindTexture(GL_TEXTURE_2D,stexture->glID);

		}
		else
		{
			while(clpart)
			{
				clpart->CalculateInitialLight();
				clpart = clpart->nextpart;
			}
			sinitial->updatetex(true);
			glBindTexture(GL_TEXTURE_2D,sinitial->glID);
		}
		
		/*if ((cy & 31) != 0)
		{
			continue;
		}*/
		// Update Screen
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		mEngine.Enter2D();
		glColor4f(1,1,1,1);
		
		// draw ligtmap
		
		glBegin(GL_QUADS);
			DrawQuad(0,0,512,512,0,0,1.0f,1.0f);
		glEnd();
		
		// draw scanline
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(1,1,1);
		sy = ((float)cy * prc) + 1.0f;
		glBegin(GL_LINES);
			glVertex3f(0.0f,sy,0.0f);
			glVertex3f(512.0f,sy,0.0f);
		glEnd();

		dword onescan = GetTickCount() - dt;
		dword remscan = (1024-cy) * onescan;
		
		int sec,hs,ms,ss;
		sec = remscan / 1000;
		ss = sec % 60; sec -= ss; sec = sec / 60;
		ms = sec % 60; sec -= ms; sec = sec / 60;
		hs = sec;

		// write status
		DefaultSkin->basefont->PrintText(10,530,"Processing %i/%i...",cy,sinitial->theight);	
		DefaultSkin->basefont->PrintText(10,542,"Scanline time %i...",onescan);
		DefaultSkin->basefont->PrintText(10,554,"Remaining %ih %im %is",hs,ms,ss);

		mEngine.Exit2D();
		SwapBuffers(mEngine.hDC);
		mEngine.DoEvents();
	}
}


//****************************************
// PLANAR MEMBERS
// Used for finding faces which shares
// same plane. So it finds planes.
// Also used for transform it too...
//****************************************


VRPlanar::VRPlanar()
{
	objid = 0;
	facecount = 0;
	/*highcorner.set(-9999999.0f,-9999999.0f,-9999999.0f);
	lowcorner.set(9999999.0f,9999999.0f,9999999.0f);*/
}

void VRPlanar::AddFace(int fid, tri3 *tri,bool* pFaces)
{
	pFaces[fid] = true; // its processed
	tri3 *bostri = new tri3;
	bostri->v1 = tri->v1; // copy
	bostri->v2 = tri->v2; // copy
	bostri->v3 = tri->v3; // copy

	faceids.add((void*)fid);
	triangles.add((void*)bostri);
	facecount++;
	center.x += (tri->v1.x + tri->v2.x + tri->v3.x) / 3.0f;
	center.y += (tri->v1.y + tri->v2.y + tri->v3.y) / 3.0f;
	center.z += (tri->v1.z + tri->v2.z + tri->v3.z) / 3.0f;

	//Tri3Test(bostri);
	// centeri bulurken, center vectorunu (facecount * 3) e bölmek gerek
}


void VRPlanar::FindQuads(RMDMesh* cmesh,int fid,int* fvi,bool* pFaces)
{
	if (FindQuad(cmesh,fid,fvi[0],fvi[1],pFaces,fvi[2])) return;
	if (FindQuad(cmesh,fid,fvi[1],fvi[2],pFaces,fvi[0])) return;
	if (FindQuad(cmesh,fid,fvi[2],fvi[0],pFaces,fvi[1])) return;
}

int VRPlanar::GetCorner(int* fvi,int sv1,int sv2)
{
	bool a,b,c;
	int corner;
	a = fvi[0] == sv1 || fvi[0] == sv2;
	b = fvi[1] == sv1 || fvi[1] == sv2;
	c = fvi[2] == sv1 || fvi[2] == sv2;
	if (a & b)
		return 2;
	else
		if (b & c)
			return 0;
		else
			return 1;
}



bool VRPlanar::FindQuad(RMDMesh* cmesh,int fid,int sv1,int sv2, bool* pfaces,int sv3)
{
	int i;
	int fvi[3]; // face vertex indice
	tri3 tri;
	vec3 cfn; // current face normal
	float ang;
	
	for (i=0;i<cmesh->facecount;i++)
	{
		if ( i==fid ) continue;
		
		cmesh->GetTriangle(i,&tri,fvi);
		
		if (fvi[0] == sv1 || fvi[1] == sv1 || fvi[2] == sv1 )
		{
			if (fvi[0] == sv2 || fvi[1] == sv2 || fvi[2] == sv2 )
			{
				if (pfaces[i])
				{
					return false;
				}
				
				cfn = tri.CalculateNormal();
				ang = angle(fn,cfn);
				
				if (ang > MAX_ANGLE)
				{
					return false;
				}

				int corneri1 = GetCorner(fvi,sv1,sv2); // corner indice 1
				// sv3 = corneri2
				tri3 mtri2;
				cmesh->GetTriangle(fid,&mtri2);
				
				vec3 corner1 = tri.GetVertex(corneri1);
				vec3 corner2 = mtri2.GetVertex(sv3);

				vec3 edge1 = mtri2.GetVertex(sv1);
				vec3 edge2 = mtri2.GetVertex(sv2);
				
				float m1 = distance(corner1,corner2);
				float m2 = distance(edge1,edge2);

				// tolerance = 0.1f;
				if (fabs(m1-m2) < 1.0f)
				{
					// its not a quad
					return false;
				}
				
				AddFace(i,&tri,pfaces);
				
				// and everybody lived happy forever...
			}
		}
		
	}
}

void VRPlanar::FindNeighbours(RMDMesh* cmesh,int fid,int* fvi,bool* pFaces)
{
	FindEdgeNeighbours(cmesh,fid,fvi[0],fvi[1],pFaces);
	FindEdgeNeighbours(cmesh,fid,fvi[1],fvi[2],pFaces);
	FindEdgeNeighbours(cmesh,fid,fvi[2],fvi[0],pFaces);
}

void VRPlanar::FindEdgeNeighbours(RMDMesh* cmesh,int fid,int sv1,int sv2, bool* pfaces)
{
	int i;
	int fvi[3]; // face vertex indice
	tri3 tri;
	vec3 cfn; // current face normal
	float ang;
	
	for (i=0;i<cmesh->facecount;i++)
	{
		if ( i==fid ) continue;
		
		cmesh->GetTriangle(i,&tri,fvi);

		if (fvi[0] == sv1 || fvi[1] == sv1 || fvi[2] == sv1 )
		{
			if (fvi[0] == sv2 || fvi[1] == sv2 || fvi[2] == sv2 )
			{
				if (pfaces[i])
				{
					return;
				}

				cfn = tri.CalculateNormal();
				ang = angle(fn,cfn);

				if (ang > MAX_ANGLE)
				{
					return;
				}

				AddFace(i,&tri,pfaces);

				FindEdgeNeighbours(cmesh,i,fvi[0],fvi[1],pfaces);
				FindEdgeNeighbours(cmesh,i,fvi[1],fvi[2],pfaces);
				FindEdgeNeighbours(cmesh,i,fvi[2],fvi[0],pfaces);
				// and everybody lived happy forever...
			}
		}
		
	}
}

void VRPlanar::GetMidPoint()
{
	center = center / (float)(facecount);
}

void VRPlanar::Vec3Test(vec3*v1 ,vec3* v2,bool dir)
{
	if (dir) // true then look for greater
	{
		if (v1->x > v2->x) v2->x = v1->x;
		if (v1->y > v2->y) v2->y = v1->y;
		if (v1->z > v2->z) v2->z = v1->z;
	}
	else
	{
		if (v1->x < v2->x) v2->x = v1->x;
		if (v1->y < v2->y) v2->y = v1->y;
		if (v1->z < v2->z) v2->z = v1->z;
	}
}

void VRPlanar::Tri3Test(tri3* tri)
{
	Vec3Test(&tri->v1,&highcorner,true);
	Vec3Test(&tri->v2,&highcorner,true);
	Vec3Test(&tri->v3,&highcorner,true);

	Vec3Test(&tri->v1,&lowcorner,false);
	Vec3Test(&tri->v2,&lowcorner,false);
	Vec3Test(&tri->v3,&lowcorner,false);
}

void VRPlanar::RotateToNewNormal(vec3 nn)
{
	quat q;
	vec3 qpart;
	float ang;
	int i;
	matrix4x4f rmat; // final rotation matrix

	// find rotation axis with cross product
	qpart = cross(fn,nn);
	qpart.normalize();
	// find angle between two normals
	ang = angle(fn,nn);
	if (ang < MAX_ANGLE || (180 - ang) < MAX_ANGLE)
	{
		tri3 *ctri; // current triangle
		for (i=0;i<facecount;i++)
		{
			ctri = (tri3*)triangles.pArray[i];
			ctri->v1 -= center;
			ctri->v2 -= center;
			ctri->v3 -= center;
			Tri3Test(ctri);
		}
		return;
	}
	// create quat from axis and angle
	q.setaxis(qpart.x,qpart.y,qpart.z,ang);
	// create matrix from quaternion
	q.tomatrix(rmat.m);
	rmat.transposerotation(); // TODO: fix quaternion to mat

	// translate all faces
	tri3 *ctri; // current triangle
	for (i=0;i<facecount;i++)
	{
		ctri = (tri3*)triangles.pArray[i];
		ctri->v1 -= center;
		ctri->v2 -= center;
		ctri->v3 -= center;
		
		rmat.transformVector(&ctri->v1); // will not translate it
		rmat.transformVector(&ctri->v2); // will not translate it
		rmat.transformVector(&ctri->v3); // will not translate it
		
		// find min max points of new plane
		Tri3Test(ctri);
	}

	fn = nn; // :)
}

void VRPlanar::MoveToZero()
{
	int i;
	tri3 *ctri; // current triangle
	for (i=0;i<facecount;i++)
	{
		ctri = (tri3*)triangles.pArray[i];
		ctri->v1 -= lowcorner;
		ctri->v2 -= lowcorner;
		ctri->v3 -= lowcorner;
	}
	highcorner -= lowcorner;
	lowcorner.set(0,0,0);
}

void VRPlanar::Project2D()
{
	vec3 v2d;
	v2d.z = -1.0f;

	RotateToNewNormal(v2d);
	MoveToZero();
}

void VRPlanar::Scale(float sz)
{
	int i;
	tri3 *ctri; // current triangle
	for (i=0;i<facecount;i++)
	{
		ctri = (tri3*)triangles.pArray[i];
		ctri->v1 = ctri->v1 * sz;
		ctri->v2 = ctri->v2 * sz;
		ctri->v3 = ctri->v3 * sz;
	}
	highcorner = highcorner * sz;
}

void VRPlanar::Render()
{
	int i;
	tri3 *ctri; // current triangle

	glBindTexture(GL_TEXTURE_2D,0);

	glBegin(GL_TRIANGLES);
	for (i=0;i<facecount;i++)
	{
		ctri = (tri3*)triangles.pArray[i];
		ctri->v1.render();
		ctri->v2.render();
		ctri->v3.render();
	}
	glEnd();
}

void VRPlanar::AllocateLightmap(RArray* lightmaps)
{
	int i; // i for loop, r for result
	int rw,rh; // required width, required height
	
	rw = (int)(ceil(highcorner.x)) + MAP_TOTALPADDING; // 2 = for padding
	rh = (int)(ceil(highcorner.y)) + MAP_TOTALPADDING;
	
	
	// if its first plane create first lightmap
	if (lightmaps->count == 0)
	{
		lightmaps->add( VRLightmap::CreateNewLightmap(vrad) );
	}
	
	// Look for a lightmap that can able to handle this plane
	VRLightmap* csurf;
	for (i=0;i<lightmaps->count;i++)
	{
		csurf = (VRLightmap*)(lightmaps->pArray[i]);
		alpnode = csurf->AllocateQuad(rw,rh);
		if (alpnode == 0)
		{
			if (i == (lightmaps->count -1) ) // if it was last item
			{
				csurf = VRLightmap::CreateNewLightmap(vrad);
				lightmaps->add(csurf);
				alpnode = csurf->AllocateQuad(rw,rh);
				QuickInfo("erür");
				if (alpnode == 0)
				{
					QuickInfo("space allocation failed");
				}
			}
			continue;
		}
		else
		{
			break;
		}
	}
	
	// Fill the information
	alightmap = csurf;
}

void VRPlanar::ConvertToTextureCoords(tri2* tSpc)
{
	int fstart;
	int i,k;
	tri3 *ctri; // current triangle
	int cfid; // current faceid
	float sx,sy;
	sx = (float)(alpnode->x + MAP_HALFPADDING);
	sy = (float)(alpnode->y + MAP_HALFPADDING);

	tri2* ct2d; // current triangle 2D
	VRFace* faces = vrad->faces[objid];

	float nxy[6];

	bool idone = false;

	for (i=0;i<facecount;i++)
	{
		ctri = (tri3*)(triangles.pArray[i]);
		cfid = (int)(faceids.pArray[i]); // face id
		ct2d = &tSpc[cfid];

		ct2d->v1.x = ctri->v1.x + sx;
		ct2d->v2.x = ctri->v2.x + sx;
		ct2d->v3.x = ctri->v3.x + sx;
		
		ct2d->v1.y = ctri->v1.y + sy;
		ct2d->v2.y = ctri->v2.y + sy;
		ct2d->v3.y = ctri->v3.y + sy;

		// this code for allocate as quad
		if (!idone)
		{
			alightmap->FillQuad(&faces[cfid],ct2d,alpnode);
			idone = true;
		}

		//alightmap->FillTriangle(&faces[cfid],ct2d);
		
		/*nxy[0] = ct2d->v1.x;
		nxy[1] = ct2d->v1.y;
		nxy[2] = ct2d->v2.x;
		nxy[3] = ct2d->v2.y;
		nxy[4] = ct2d->v3.x;
		nxy[5] = ct2d->v3.y;*/


		//alightmap->sinitial->filltriangle2(nxy,RGB(255,255,255));
		ct2d->v1.x /= (float)MAX_LIGHTMAPSIZE;
		ct2d->v1.y /= (float)MAX_LIGHTMAPSIZE;
		ct2d->v2.x /= (float)MAX_LIGHTMAPSIZE;
		ct2d->v2.y /= (float)MAX_LIGHTMAPSIZE;
		ct2d->v3.x /= (float)MAX_LIGHTMAPSIZE;
		ct2d->v3.y /= (float)MAX_LIGHTMAPSIZE;


		/*for (k=0;k<6;k++)
		{
			

			//dl[k] = (int)nxy;
			//df[k] = (float)dl[k] / (float)MAX_LIGHTMAPSIZE;
		}*/
	}
}


//****************************************
// RADIOSITY MEMBERS
//****************************************

VRadiosity::VRadiosity()
{
	radenv.vrad = this;
}

void VRadiosity::PrepareGeometry(float ptsr)
{
	int meshid,face;

	VRFace* cfaces;
	RMDMesh* cmesh;
	tri3 tri;
	vec3 fn;

	int i,j;
	int fvi[3]; // face vertex indice

	//****************************************
	// MARK THE NOT PROCESSED FACES
	//****************************************
	bool** pFaces; // processed faces
	pFaces = new bool * [mdl->meshcount];
	tSpc = new tri2 * [mdl->meshcount];
	faces = new VRFace * [mdl->meshcount];

	bool* pMesh; // processed mesh

	for (i=0;i<mdl->meshcount;i++)
	{
		pMesh = new bool [mdl->meshes[i].facecount];
		pFaces[i] = pMesh;
		for (j = 0;j<mdl->meshes[i].facecount;j++)
		{
			pMesh[j] = false;

		}
	}

	//****************************************
	// PROCESS THE FACES
	//****************************************
	for (meshid=0;meshid<mdl->meshcount;meshid++)
	{
		cmesh = &mdl->meshes[meshid];
		pMesh = pFaces[meshid];
		faces[meshid] = new VRFace [cmesh->facecount];
		tSpc[meshid] = new tri2 [cmesh->facecount]; // allocate area for texture space
		cfaces = faces[meshid];

		for (face=0;face<cmesh->facecount;face++)
		{
			cfaces[face].SetData(cmesh,face);
			// if processed continue...
			if (pMesh[face])
			{
				continue;
			}

			// if not? process it...
			cmesh->GetTriangle(face,&tri,fvi);
			
			VRPlanar* newplane = new VRPlanar;
			fplanes.add(newplane);
			newplane->objid = meshid;
			newplane->vrad = this;
			newplane->fn = tri.CalculateNormal();
			newplane->AddFace(face,&tri,pMesh);
			newplane->FindNeighbours(cmesh, face,fvi,pMesh);// find neighbours of this face
			newplane->GetMidPoint();
			newplane->Project2D();
			newplane->Scale(ptsr);
			newplane->AllocateLightmap(&lightmaps);
		}
	}

	VRPlanar* cplane;
	for (i=0;i<fplanes.count;i++)
	{
		cplane = (VRPlanar*)fplanes.pArray[i];
		cplane->ConvertToTextureCoords(tSpc[cplane->objid]);
	}
}

void VRadiosity::CalculateRadiosity()
{
	int i;
	VRLightmap *clmap;
	for (i=0;i< lightmaps.count;i++ )
	{
		clmap = (VRLightmap*)lightmaps.pArray[i];
		clmap->CalculateLightmap();
	}	
}

void VRadiosity::TestRenderParts()
{
	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(0,1,0);

	int i,j;
	VRPlanar* cplane;
	tri3* ctri;
	for (i=0;i<fplanes.count;i++)
	{
		cplane = (VRPlanar*)fplanes.pArray[i];
		for (j=0;j<cplane->facecount;j++)
		{
			ctri = (tri3*)cplane->triangles.pArray[j];
			glBegin(GL_LINES);
				ctri->v1.render();
				ctri->v2.render();

				ctri->v2.render();
				ctri->v3.render();

				ctri->v3.render();
				ctri->v1.render();
			glEnd();
		}
	}
}

void VRadiosity::TestRenderNormals()
{
	int i,j;
	vec3 c,n,t,b; // center, normal tangent binormal

	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(0,1,0);

	VRFace* cfaces;
	for (i=0;i<mdl->meshcount;i++)
	{
		cfaces = faces[i];
		for (j=0;j<mdl->meshes[i].facecount;j++)
		{
			c = cfaces[j].triangle.CalculateCenter();
			n = cfaces[j].fnor;
			t = cfaces[j].ftan;
			b = cfaces[j].fbin;
			
			n = c + (n * 5.0f);
			t = c + (t * 5.0f);
			b = c + (b * 5.0f);

			glBegin(GL_LINES);
				glColor3f(0,1,0);
				cfaces[j].triangle.v1.render();
				cfaces[j].triangle.v2.render();

				cfaces[j].triangle.v2.render();
				cfaces[j].triangle.v3.render();

				cfaces[j].triangle.v3.render();
				cfaces[j].triangle.v1.render();

				c.render();
				n.render();
				
				glColor3f(1,0,0);
				c.render();
				t.render();
				
				glColor3f(0,0,1);
				c.render();
				b.render();
			glEnd();
		}
	}
}

void VRadiosity::TestRenderLighting()
{
	int i,j, k, l,m;
	int v1,v2,v3,mt;
	int t1,t2,t3;
	vec3 x0,x1,x2,n1;
	bool mm;
	RMDMaterial* cmat;
	RMDMesh* cmesh;
	VRLightmap* lmap;
	tri2 * ctspc; // 2d triangles found on texture space
	
	lmap = (VRLightmap*)lightmaps.pArray[0];
	glBindTexture(GL_TEXTURE_2D,lmap->stexture->glID);
	
	for (i = 0 ; i < mdl->meshcount ;i++)
	{	
		m=l=k=0;

		cmesh = &mdl->meshes[i];
		ctspc = tSpc[i];
		
		for (j=0; j < cmesh->facecount ; j++)
		{
			v1 = cmesh->faces[k++] *3;
			v2 = cmesh->faces[k++] *3;
			v3 = cmesh->faces[k++] *3;
			mt = cmesh->faces[k++]; // WTF?
			
			t1 = cmesh->tfaces[m++] << 1;
			t2 = cmesh->tfaces[m++] << 1;
			t3 = cmesh->tfaces[m++] << 1;
			
			glColor3ub(255,255,255);
			glBegin(GL_TRIANGLES);
			
			glNormal3fv(&cmesh->normals[l]);l+=3;
			glTexCoord2f(ctspc[j].v1.x,ctspc[j].v1.y);
			glVertex3fv(&cmesh->vexs[v1]);
			
			glNormal3fv(&cmesh->normals[l]);l+=3;
			glTexCoord2f(ctspc[j].v2.x,ctspc[j].v2.y);			
			glVertex3fv(&cmesh->vexs[v2]);
			
			glNormal3fv(&cmesh->normals[l]);l+=3;
			glTexCoord2f(ctspc[j].v3.x,ctspc[j].v3.y);
			glVertex3fv(&cmesh->vexs[v3]);
			
			glEnd();
			
			
		}
	}
	
	glActiveTexture( GL_TEXTURE1_ARB );
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture( GL_TEXTURE0_ARB );
	
}


void VRadiosity::TestRender()
{
	int i,j, k, l,m;
	int v1,v2,v3,mt;
	int t1,t2,t3;
	vec3 x0,x1,x2,n1;
	bool mm;
	RMDMaterial* cmat;
	RMDMesh* cmesh;
	VRLightmap* lmap;
	tri2 * ctspc; // 2d triangles found on texture space

	lmap = (VRLightmap*)lightmaps.pArray[0];
	glActiveTexture( GL_TEXTURE1_ARB );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,lmap->stexture->glID);
	
	glActiveTexture( GL_TEXTURE0_ARB );

	for (i = 0 ; i < mdl->meshcount ;i++)
	{	
		m=l=k=0;
		cmat = &mdl->mats[mdl->meshes[i].matid];
		if (cmat->flags & MAP_MULTI)
		{
			mm=true;
		}
		else
		{
			mm = false;
			cmat->singlmat->AttachMaterial();
		}

		cmesh = &mdl->meshes[i];
		ctspc = tSpc[i];

		for (j=0; j < cmesh->facecount ; j++)
		{
			v1 = cmesh->faces[k++] *3;
			v2 = cmesh->faces[k++] *3;
			v3 = cmesh->faces[k++] *3;
			mt = cmesh->faces[k++]; // WTF?

			if (mm)
			{
				cmat->multimat[mt]->AttachMaterial();
			}

			
			
			t1 = cmesh->tfaces[m++] << 1;
			t2 = cmesh->tfaces[m++] << 1;
			t3 = cmesh->tfaces[m++] << 1;
			
			glColor3ub(255,255,255);
			glBegin(GL_TRIANGLES);

				glNormal3fv(&cmesh->normals[l]);l+=3;
				glMultiTexCoord2fv(GL_TEXTURE0_ARB,&cmesh->texcords[t1]);		
				glMultiTexCoord2f(GL_TEXTURE1_ARB,ctspc[j].v1.x,ctspc[j].v1.y);
				glVertex3fv(&cmesh->vexs[v1]);

				glNormal3fv(&cmesh->normals[l]);l+=3;
				glMultiTexCoord2fv(GL_TEXTURE0_ARB,&cmesh->texcords[t2]);
				glMultiTexCoord2f(GL_TEXTURE1_ARB,ctspc[j].v2.x,ctspc[j].v2.y);				
				glVertex3fv(&cmesh->vexs[v2]);

				glNormal3fv(&cmesh->normals[l]);l+=3;
				glMultiTexCoord2fv(GL_TEXTURE0_ARB,&cmesh->texcords[t3]);
				glMultiTexCoord2f(GL_TEXTURE1_ARB,ctspc[j].v3.x,ctspc[j].v3.y);
				glVertex3fv(&cmesh->vexs[v3]);
		
			glEnd();


		}
	}

	glActiveTexture( GL_TEXTURE1_ARB );
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture( GL_TEXTURE0_ARB );

}

VRFace* VRadiosity::TestProjectRay(ray3 &r,vec3 &p,tri2& tx)
{
	int msh,fc;
	VRFace* cfaces;
	VRFace* cface;
	float dist;
	float ldist = 999999.0f;
	vec3 lp;
	VRFace* lface = 0;
	int lmid;
	int lfid;
	for (msh=0;msh<mdl->meshcount;msh++)
	{
		cfaces = faces[msh];
		for (fc=0;fc<mdl->meshes[msh].facecount;fc++)
		{
			cface = &cfaces[fc];
			if (cface->triangle.IntersectRay(r,p))
			{
				dist= distance(r.p,p);
				if (dist < ldist )
				{
					ldist = dist;
					lface = cface;
					lp = p;
					lmid = msh;
					lfid = fc;
				}
			}
		}
	}
	
	if (lface)
	{
		tri2* spcs = tSpc[lmid];
		tri2* ohs = &spcs[lfid];
		tx = *ohs;
	}

	p = lp;
	return lface;
}