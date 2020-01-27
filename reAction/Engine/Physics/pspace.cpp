
#include "StdAfx.h"
#include "pspace.h"

#include "../Tools/rtools.h"
using namespace RTools;

inline void setdata(float* ar,int x,int y,int v);
inline float getdata(float* ar,int x,int y);


void PCelestialMass::Calculate()
{

}

/*struct vec2f
{
	float x;
	float y;
};*/

struct lnseg // line segment
{
	int v1;
	int v2;
};

class PLand;

struct plink
{
	PLand* otherland; // otherland
	int myidx; // my index
	int otidx; // other index
};

// vec2f olsaydý güzel olcaktý
class PLand
{
public:
	int centerx;
	int centery;
	double area;
	float radius;

	int	vcount; // vertex count
	float* xs;
	float* ys;

	RArray links;
	RArray bli; // banned lines

	RArray segs;

	float maxx,maxy;
	float minx,miny;

	bool birlesti;

	void findminmax();
};

void PLand::findminmax()
{
	int i;
	maxx = 0.0f;
	maxy = 0.0f;
	minx = 9999999999.0f;
	miny = 9999999999.0f;

	for (i=0;i<vcount;i++)
	{
		if (maxx < xs[i])
			maxx = xs[i];

		if (maxy < ys[i])
			maxy = ys[i];
	
		if (minx > xs[i])
			minx = xs[i];

		if (miny > ys[i])
			miny = ys[i];
	}
}


struct TrackPoint
{
	float x,y;
};

//***************************************
void PointOnCurve(TrackPoint &out, float t, TrackPoint p0, TrackPoint p1, TrackPoint p2, TrackPoint p3)
{
	float t2 = t * t;
	float t3 = t2 * t;
	out.x = 0.5f * ( ( 2.0f * p1.x ) +( -p0.x + p2.x ) * t +( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 +( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 );
	out.y = 0.5f * ( ( 2.0f * p1.y ) +( -p0.y + p2.y ) * t +( 2.0f * p0.y - 5.0f * p1.y + 4 * p2.y - p3.y ) * t2 +( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) * t3 );
}


bool CheckBoundCollision(PLand* tochk,int wid,int hei)
{
	if (tochk->centerx - tochk->radius < 0)
	{
		return true;
	}
	
	if (tochk->centerx + tochk->radius > wid)
	{
		return true;
	}
	
	if (tochk->centery - tochk->radius < 0)
	{
		return true;
	}
	
	if (tochk->centery + tochk->radius > hei)
	{
		return true;
	}
	
	return false;
}

int nomvar; // nominal variation
int minvar; // minimum variation percentage
int maxvar; // max variation percentage
int totvar; // total variation or maxvar - minvar

float pointdistance(int x1,int y1,int x2,int y2)
{
	int dx,dy;
	float dist;
	dx = x1 - x2;
	dy = y1 - y2;
	dist = sqrt((dx*dx) + (dy*dy));
	return dist;
}

float landdistance(PLand* l1,PLand* l2)
{
	return pointdistance(l1->centerx,l1->centery,l2->centerx,l2->centery);
}

float absoultedistance(PLand* l1,PLand* l2)
{
	float dist;
	dist = landdistance(l1,l2);
	dist -= (l1->radius + l2->radius);
	return dist;
}

bool CheckCollision(RArray* landary, PLand* tochk)
{
	if (CheckBoundCollision(tochk,2048,2048))
	{
		return true;
	}

	int i;
	PLand* curland;
	int dx;
	int dy;
	float dist;
	for (i=0;i<landary->count;i++)
	{
		curland = (PLand*)landary->pArray[i];

		dist = landdistance(curland,tochk);

		if (dist < (curland->radius + tochk->radius))
		{
			return true;
		}
	}
	return false;
}

PLand* CreateLand(RArray* landary,float area)
{
	PLand* bosland = new PLand;
	bosland->area = area;
	bosland->radius = sqrt(area / PI);

	do 
	{
		bosland->centerx = rand() % 2048;
		bosland->centery = rand() % 2048;
	} while (CheckCollision(landary,bosland));


	bosland->vcount = 8;
	bosland->xs = new float [bosland->vcount];
	bosland->ys = new float [bosland->vcount];

	lnseg* bseg;

	int k;
	float varmult,curdetail;
	float aci = 0.0f;
	for (k=0;k<8;k++)
	{
		bseg = new lnseg;
		varmult = (double)((rand() % totvar) + minvar) / 100.0f ;
		curdetail = bosland->radius * varmult;

		bosland->xs[k] = (cos(DEGTORAD(aci)) * curdetail) + bosland->centerx;
		bosland->ys[k] = (sin(DEGTORAD(aci)) * curdetail) + bosland->centery;
		bseg->v1 = k;
		bseg->v2 = k+1;
		bosland->segs.add(bseg);
		aci += 45.0f;
	}

	bseg->v2 = 0;


	bosland->birlesti = false;
	bosland->findminmax();
	
	return bosland;
}

void TruncateLand(PLand* l1,PLand* l2)
{
	/*PLand* bosland;
	bosland = new PLand;*/

	float curdist;

	int i,k;
	int	curindex=0;

	//bosland->vcount = l1->vcount + l2->vcount;


	curindex = 0;
	float maxdist1 = 999999999.0f;
	float maxdist2 = 999999999.0f;
	
	int last1,last2;
	int last3,last4;

	// ilk en yakin nokta
	for (i=0;i<l1->vcount;i++)
	{
		for (k=0;k<l2->vcount;k++)
		{
			curdist = pointdistance(l1->xs[i],l1->ys[i],l2->xs[k],l2->ys[k]);

			if (curdist < maxdist1)
			{
				last1 = i;
				last2 = k;
				maxdist1 = curdist;
			}
		}
	}


	// 2. en yakin nokta
	for (i=0;i<l1->vcount;i++)
	{
		if (i == last1)
			continue;

		for (k=0;k<l2->vcount;k++)
		{
			if (k == last2)
				continue;

			curdist = pointdistance(l1->xs[i],l1->ys[i],l2->xs[k],l2->ys[k]);
			
			if (curdist < maxdist2)
			{
				last3 = i;
				last4 = k;
				maxdist2 = curdist;
			}
		}
	}

	float ortax1,ortay1,ortax2,ortay2;



	ortax1 = (l1->xs[last1] + l2->xs[last2]) / 2.0f;
	ortay1 = (l1->ys[last1] + l2->ys[last2]) / 2.0f;
	ortax2 = (l1->xs[last3] + l2->xs[last4]) / 2.0f;
	ortay2 = (l1->ys[last3] + l2->ys[last4]) / 2.0f;
	
	l1->xs[last1] = ortax1;
	l1->ys[last1] = ortay1;

	l2->xs[last2] = ortax1;
	l2->ys[last2] = ortay1;

	l1->xs[last3] = ortax2;
	l1->ys[last3] = ortay2;
	
	l2->xs[last4] = ortax2;
	l2->ys[last4] = ortay2;


	// checked and fixed old links
	plink* tlink;
	for (i=0;i<l1->links.count;i++)
	{
		tlink = (plink*)l1->links.pArray[i];
		if (tlink->myidx == last1)
		{
			tlink->otherland->xs[tlink->otidx] = ortax1;
			tlink->otherland->ys[tlink->otidx] = ortay1;
		}
		if (tlink->myidx == last3)
		{
			tlink->otherland->xs[tlink->otidx] = ortax2;
			tlink->otherland->ys[tlink->otidx] = ortay2;
		}
	}

	for (i=0;i<l2->links.count;i++)
	{
		tlink = (plink*)l2->links.pArray[i];
		if (tlink->myidx == last2)
		{
			tlink->otherland->xs[tlink->otidx] = ortax1;
			tlink->otherland->ys[tlink->otidx] = ortay1;
		}
		if (tlink->myidx == last4)
		{
			tlink->otherland->xs[tlink->otidx] = ortax2;
			tlink->otherland->ys[tlink->otidx] = ortay2;
		}
	}


	// creating new links
	plink * blink = new plink;
	blink->otherland = l2;
	blink->myidx = last1;
	blink->otidx = last2;
	l1->links.add(blink);
	
	blink = new plink;
	blink->otherland = l2;
	blink->myidx = last3;
	blink->otidx = last4;
	l1->links.add(blink);

	// link for land2
	blink = new plink;
	blink->otherland = l1;
	blink->myidx = last2;
	blink->otidx = last1;
	l2->links.add(blink);

	blink = new plink;
	blink->otherland = l1;
	blink->myidx = last4;
	blink->otidx = last3;
	l2->links.add(blink);

	lnseg* cseg;

	// fixes tekil links
	for (i=0;i<l2->segs.count;i++)
	{
		cseg = (lnseg*)l2->segs.pArray[i];
		if ( (cseg->v1 == last2 && cseg->v2 == last4) ||  (cseg->v1 == last4 && cseg->v2 == last2) )
		{
			l2->segs.remove(i);
			break;
		}
	}

	for (i=0;i<l1->segs.count;i++)
	{
		cseg = (lnseg*)l1->segs.pArray[i];
		if ( (cseg->v1 == last1 && cseg->v2 == last3) || (cseg->v1 == last3 && cseg->v2 == last1) )
		{
			l1->segs.remove(i);
			break;
		}
	}

	/*// banning links
	int b1 = min(last1,last3);
	int b2 = min(last2,last4);
	// maxes
	int b3 = max(last1,last3);
	int b4 = max(last2,last4);

	if (b3 == 7)
	{
		//l1->bli.add((void*)(b3));
	}
	else
	{
		l1->bli.add((void*)(b1));
	}

	if (b4 == 7)
	{
		//l2->bli.add((void*)(b4));
	}
	else
	{
		l2->bli.add((void*)(b2));
	}*/



}

RArray* TruncateLands(RArray* landary)
{
	int i,k;
	PLand* curland;
	PLand* curland2;
	float absdist;
	
	RArray* newlandary = new RArray;
	
	bool actiontaken; // true when some lands combined
	
	for (i=0;i<landary->count;i++)
	{
		curland = (PLand*)landary->pArray[i];
		/*if (curland->birlesti)
			continue;*/
		
		for (k=0;k<landary->count;k++)
		{
			if (k == i)
				continue;
			
			curland2 = (PLand*)landary->pArray[k];
			
			/*if (curland2->birlesti)
				continue;*/
			
			absdist = absoultedistance(curland,curland2);
			
			if (absdist< 80.0f) // 50 pixelden güççük oley
			{
				TruncateLand(curland,curland2);
				//newlandary->add(TruncateLand(curland,curland2));
				curland->birlesti = true;
				curland2->birlesti = true;
				actiontaken = true;
			}
		}
	}
	
	return landary;
}



// bi  yontemde bi cember cizip sagindan solundan kaydirmak oalbilir

void PCelestialMass::Generate()
{
	test = new VTexture;
	test->createbuffer(2048,2048,GL_RGB);
	test->draw_wrap = true;

	srand(GetTickCount());

	int bigareas;
	int mediumareas;
	int smallareas;

	bigareas = rand() % 4 +1;
	mediumareas = rand() % 3 +1;
	smallareas = rand() % 12 +1; // like japan mapan

	/*
	// good setting for creating dungeons
	 bigareas = 0;
	mediumareas = 0;
	smallareas = 60;*/

	double totalarea = 2048.0f * 2048.0f;

	// totalareanýnda %60 ý büyük alanlar % 30 u orta %10 uda küçük alanlar
	double landarea = totalarea * landwaterratio;

	double bigarea = landarea * 0.6f;
	double mediumarea = landarea * 0.3f;
	double smallarea = landarea * 0.1f;

	double variation =  0.30f; // %30 + - olabilir boyutlarda

	double* bigareasizes = new double [bigareas];
	double* mediumareasizes = new double [mediumareas];
	double* smallareasizes = new double [smallareas];

	int i,k;
	
	double averagebigarea;
	double averagemedarea;
	double averagesmallarea;

	if (bigareas)
		 averagebigarea = bigarea / (double)bigareas;

	if (mediumareas)
		averagemedarea = mediumarea / (double)mediumareas;

	if (smallareas)
		averagesmallarea = smallarea / (double)smallareas;

	nomvar = floor(variation * 100.0f); // nominal variation
	minvar = 100 - nomvar; // minimum variation percentage
	maxvar = 100 + nomvar; // max variation percentage
	totvar = nomvar << 1; // total variation or maxvar - minvar

	double varmult; // variation multiplier
	double curarea; // current area
	double remarea; // remaining area

	// array for land masses
	RArray lands;
	PLand* bosland;

	// calculate area sizes for big sized lands
	remarea = bigarea;
	for (i=0;i<bigareas;i++)
	{
		varmult = (double)((rand() % totvar) + minvar) / 100.0f ;
		curarea = averagebigarea * varmult;

		if (remarea <= curarea)
		{
			if ( (i+1) == bigareas)
			{
				curarea = remarea;
			}
			else
			{
				QuickInfo("eerrror 1");
			}
		}

		bigareasizes[i] = curarea;
		remarea -= curarea;
		lands.add(CreateLand(&lands,curarea));
	}


	// calculate area sizes for medium sized lands
	remarea = mediumarea;
	for (i=0;i<mediumareas;i++)
	{
		varmult = (double)((rand() % totvar) + minvar) / 100.0f ;
		curarea = averagemedarea * varmult;
		
		
		if (remarea <= curarea)
		{
			if ( (i+1) == mediumareas)
			{
				curarea = remarea;
			}
			else
			{
				QuickInfo("eerrror 2");
			}
		}
		
		mediumareasizes[i] = curarea;
		remarea -= curarea;
		lands.add(CreateLand(&lands,curarea));
	}

	
	// calculate area sizes for small sized lands
	remarea = smallarea;
	for (i=0;i<smallareas;i++)
	{
		varmult = (double)((rand() % totvar) + minvar) / 100.0f ;
		curarea = averagesmallarea * varmult;
				
		if (remarea <= curarea)
		{
			if ( (i+1) == smallareas)
			{
				curarea = remarea;
			}
			else
			{
				QuickInfo("eerrror 3");
			}
		}
		
		smallareasizes[i] = curarea;
		remarea -= curarea;
		lands.add(CreateLand(&lands,curarea));
	}


	color ydkclr = soilcolor;
	soilcolor.lclr = RGB(255,255,255);
	color grn,rd;
	grn.lclr = RGB(0,255,0);
	rd.lclr = RGB(255,0,0);

	for (i=0;i<lands.count;i++)
	{
		bosland = (PLand*)lands.pArray[i];

		//test->drawcircle(bosland->centerx,bosland->centery, (word)(floor(bosland->radius)) ,grn.lclr);
		for (k=0;k<bosland->vcount-1;k++)
		{
			test->drawline(bosland->xs[k],bosland->ys[k],bosland->xs[k+1],bosland->ys[k+1],soilcolor.lclr);
		}
		k=bosland->vcount-1;
		test->drawline(bosland->xs[k],bosland->ys[k],bosland->xs[0],bosland->ys[0],soilcolor.lclr);

		test->drawline(bosland->minx,bosland->miny,bosland->maxx,bosland->miny,rd.lclr);
		test->drawline(bosland->maxx,bosland->miny,bosland->maxx,bosland->maxy,rd.lclr);
		test->drawline(bosland->maxx,bosland->maxy,bosland->minx,bosland->maxy,rd.lclr);
		test->drawline(bosland->minx,bosland->maxy,bosland->minx,bosland->miny,rd.lclr);
		
		

		test->drawcircle(bosland->centerx,bosland->centery,10,grn.lclr);
		test->drawcircle(bosland->centerx,bosland->centery,(word)(floor(bosland->radius)) ,RGB(0,0,255));
	}

	VTexture* test2 = new VTexture;
	test2->createbuffer(2048,2048,GL_RGB);
	test2->draw_wrap = true;


	RArray* tlands = TruncateLands(&lands); // truncated lands
	int j;
	int g; // current link
	bool band=false;

	/*for (i=0;i<tlands->count;i++)
	{
		bosland = (PLand*)tlands->pArray[i];
		
		//test->drawcircle(bosland->centerx,bosland->centery, (word)(floor(bosland->radius)) ,grn.lclr);
		lnseg *cseg;
		for (k=0;k<bosland->segs.count;k++)
		{
			cseg = (lnseg*)bosland->segs.pArray[k];
			
			test2->drawline(bosland->xs[cseg->v1],bosland->ys[cseg->v1],bosland->xs[cseg->v2],bosland->ys[cseg->v2],soilcolor.lclr);
		}
		
		test2->drawcircle(bosland->centerx,bosland->centery,10,grn.lclr);
		test2->drawcircle(bosland->centerx,bosland->centery,(word)(floor(bosland->radius)) ,RGB(0,0,255));
	}*/

	
	//test2->writebmp("testarea2.bmp");
	/*VTexture* test3 = new VTexture;
	VTexture* test4 = new VTexture;
	test3->createbuffer(2048,2048,GL_RGB);
	test4->createbuffer(2048,2048,GL_RGB);


	test->perlinnoise(0,256.0f);
	test2->perlinnoise(0,128.0f);
	test3->perlinnoise(0,64.0f);
	test4->perlinnoise(0,512.0f);*/

	terrain = new float [2048 * 2048];

	for (i=0;i<4194304;i++)
	{
		terrain[i] = -64;
	}


	test->gettexid();

	VTexture* ttmp = new VTexture;
	ttmp->createbuffer(512,512,GL_RGB);
	ttmp->gettexid();

	// set viewport
	glViewport(0,0,512,512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// enter 2d
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();									
	glLoadIdentity();									
	glOrtho(0,512,512,0,-1,1);		
	glMatrixMode(GL_MODELVIEW);						
	glPushMatrix();
	
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	



	int x,y;
	int sira;
	for (y=0;y<2048;y+=512)
	{
		for (x=0;x<2048;x+=512)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);
			glTranslated(-x,-y,0);

			for (i=0;i<tlands->count;i++)
			{
				bosland = (PLand*)tlands->pArray[i];
				
				glBegin(GL_TRIANGLE_FAN);
				glColor4f(0.6f,0.6f,0.6f,1.0f);
				glVertex2i(bosland->centerx,bosland->centery);
				glColor3f(0.0f,0.0f,0.0f);
				
				
				for (k=0;k<bosland->vcount;k++)
				{
					glVertex2f(bosland->xs[k],bosland->ys[k]);
				}
				glVertex2f(bosland->xs[0],bosland->ys[0]);
				
				glEnd();
			}
			
			glBindTexture(GL_TEXTURE_2D,ttmp->glID);
			glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,512,512,0);
			glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,ttmp->bytes);


			subimage(ttmp,0,0,x,y,512,512);
			/*byte* start;
			byte* othr;
			for (sira=0;sira<512;sira++) // y
			{
				start = ttmp->bytes + (sira * 512);
				othr = test->bytes + (((512-sira)+y)*2048) + x;
				memcpy(othr,start,3*512);
			}*/
			
		}
	}




	// exit 2d
	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);					
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);						
	glPopMatrix();									
	glEnable(GL_DEPTH_TEST);

	glViewport(0,0,mEngine.rdWidth,mEngine.rdHeight);


	int h;
	color cts;
	color lclr;
	float dt;
	
	for (y=0;y<2048;y++)
	{
		for (x=0;x<2048;x++)
		{
			cts.lclr = test->getpixel(x,y) ;

			dt = ((float)cts.bclr[CRED]) - 64.0f;
			
			setdata(terrain,x,y,dt);
		}
	}

	
	/*float d1,d2,d3,d4;
	float fin;
	for (y=1;y<2047;y++)
	{
		for (x=1;x<2047;x++)
		{
			d1 = getdata(terrain,x-1,y-1);
			d2 = getdata(terrain,x+1,y-1);
			d3 = getdata(terrain,x-1,y+1);
			d4 = getdata(terrain,x+1,y+1);
			fin = (d1+d2+d3+d4) / 4.0f;

			setdata(terrain,x,y,fin);
		}
	}*/



	for (i=0;i<tlands->count;i++)
	{
		bosland = (PLand*)tlands->pArray[i];
		
		//test->drawcircle(bosland->centerx,bosland->centery, (word)(floor(bosland->radius)) ,grn.lclr);
		lnseg *cseg;
		for (k=0;k<bosland->segs.count;k++)
		{
			cseg = (lnseg*)bosland->segs.pArray[k];
			
			test->drawline(bosland->xs[cseg->v1],bosland->ys[cseg->v1],bosland->xs[cseg->v2],bosland->ys[cseg->v2],soilcolor.lclr);
		}
		
		test->drawcircle(bosland->centerx,bosland->centery,10,grn.lclr);
		test->drawcircle(bosland->centerx,bosland->centery,(word)(floor(bosland->radius)) ,RGB(0,0,255));
	}



	/*int x,y,h;
	color cts,cts2,cts3,cts4;
	color lclr;
	int dt;

	for (y=0;y<2048;y++)
	{
		for (x=0;x<2048;x++)
		{
			cts.lclr = test->getpixel(x,y) ;
			cts2.lclr = test2->getpixel(x,y);
			cts3.lclr = test3->getpixel(x,y);
			cts4.lclr = test4->getpixel(x,y);
			
			dt = cts.bclr[CRED] - cts2.bclr[CRED] + (cts3.bclr[CRED]/2) - (cts4.bclr[CRED]/4);
			if (dt<0) dt = 0;

			lclr.lclr = MONO(dt);

			test->setpixel(x,y,lclr.lclr);
			setdata(terrain,x,y,lclr.bclr[CRED]-128);
		}
	}*/

	


	QuickInfo("bigs: %i meds: %i smalls: %i",bigareas,mediumareas,smallareas);

	test->writebmp("testarea.bmp");

}


void PCelestialMass::subimage(VTexture* src,word fx,word fy,word tx,word ty,word w,word h)
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
			test->setpixel(dx,dy,clrs);

		}
	}
}

inline void setdata(float* ar,int x,int y,int v)
{
	ar[ (y<<11) + x] = v;
}

inline float getdata(float* ar,int x,int y)
{
	return ar[ (y<<11) + x];
}

inline float lip(float mn,float mx,float tween) // linear interpolation
{
	return (tween * mx) + ((1.0f-tween) * mn);
}

void PCelestialMass::EditTerrain(int cx,int cy,int r,int d) // edit terrain, ar = float array, x ,y coordinates ,r radius, d = diffrence
{
	sx = floor(cx) - r;
	ex = floor(cx) + r;
	
	sy = floor(cy) - r;
	ey = floor(cy) + r;
	
	if ( sx < 0)
		sx = 0;
	
	if ( ex > 2047)
		ex = 2047;
	
	if ( sy < 0)
		sy = 0;
	
	if ( ey > 2047)
		ey = 2047;
	
	float cd; // current diffrence
	float dist; // current distance
	float ch; // current height
	float pdd =  (float)r / (float)r;

	int x,y;
	for (y=sy;y<ey;y++)
	{
		for (x=sx;x<ex;x++)
		{
			dist = pointdistance(x,y,cx,cy);
			if (dist < 1.0f)
				dist = 1.0f;
			
			cd = lip((float)d,0.0f,dist/(float)r);

			ch = (getdata(terrain,x,y) + cd) * 0.5f;
			setdata(terrain,x,y,ch);

			test->setpixel(x,y,MONO((byte)(ch+64)));
		}
	}

}



void PCelestialMass::Render()
{

}

void PPlanet::Earth()
{
	aphelion = 1.0167103335L;
	perihelion = 0.9832898912L;
	eccentricty = 0.016710219L;
	orbitalperiod = 365.256366L; // days
	orbitalspeed = 29783.0L; // km/sec
	inclination = 7.25L; // degrees
	satellitecount = 1; // WRONG way to do this
	satellites = 0;
	radius = 6378.1f;
	circumference = 40075.02L; // km
	surfacearea = 510072000.0L; // km^2
	// siderealperiod = 23h 56 04.09054 secs
	highestpoint = 10000.0f;
	lowestpoint = -10000.0f;
	sealevel = 0.0f;
	gravity = 9.780327f; // m/s^2
	temperature[0] = 184; // min Kelvin
	temperature[1] = 287; // mean kelvin
	temperature[2] = 331; // max kelvin

	discovered = true;
	halocolor.lclr = RGBA(179,209,255,128);
	soilcolor.lclr = RGB(109,67,20);

	plantable = true;
	habitable = true;
	landwaterratio = 29.2f / 70.8f; // percentages of water land
}