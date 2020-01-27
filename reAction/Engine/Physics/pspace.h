// physical celestial body simulator and generator
// Oha.

#ifndef PSPACE_H
#define PSPACE_H

#define AstronomicalUnit	149597870691.0f				

#include "../Chemistry/celements.h"
#include "../Visual/vcolor.h"
#include "../Visual/vtexture.h"
#include "../Tools/rfilesystem.h"
#include "../Visual/vcamera.h"
#include "../Visual/vengine.h"
#include "../Mathematics/mvector.h"

class PCelestialMass
{
public:
	double		x,y,z; // starting position in universe

	union
	{
		double	aphelion;	// for planets merkeze en yakýn nokta
		double	pergiee;	// for satellites
	};

	union
	{
		double	perihelion;	// for plantes merkeze en yakýn nokta
		double	apogee;		// for satellites
	};

	double		eccentricty;
	double		orbitalperiod;
	double		orbitalspeed;
	double		inclination;
	double		radius;
	double		circumference;	// calculate this!
	double		surfacearea;	// calculate this!
	float		gravity;		// calculate this!
	float		temperature[3]; // min mean max
	float		siderealperiod;
	float		highestpoint;
	float		lowestpoint;
	float		sealevel;
	double		mass;

	bool		discovered;

	color		halocolor;
	color		soilcolor;
	
	CSolution	s_liquid; // belki lav dolu biyer aq
	CSolution	s_land;
	CSolution	s_air; // is it necessary?
	
	bool		plantable;
	bool		habitable;

	float		landwaterratio; // will be obsolete when CSolution implemented

	//virtual void	Randomize();
	void			GenerateTerrain();
	void			Calculate(); // calculates area, circumfence and some other things
	
	
	long			width;
	long			height;
	float*			terrain;	// will be multidimensional array
	float*			normals;	// 3 per node
	float*			tcoords;	// 2 per node
	VTexture*		textures;	// 1 per node
	float*			blend;		// 1 per node ( blend between 2 textures )

	VTexture*		test;

	VCamera*		cam;

	void			Generate();

	int				sx,sy;	// start x
	int				ex,ey;	// endx

	void			EditTerrain(int cx,int cy,int r,int d);

	void			GenerateTriangle(int cx,int cy,int x1,int x2,int y1,int y2,float h); // center x,y segment x1,x2,y1,y2
	void			DrawLine(int x,int y,int x2,int y2,float h);
	void			subimage(VTexture* src,word fx,word fy,word tx,word ty,word w,word h);

	void			Render();

	void			Write(RFile* fp);
	void			Read(RFile* fp);

};

class PPlanet;

class PSatellite: public PCelestialMass
{
public:
	PPlanet*	parent;

	//void		Randomize(); // parameters comes here
};

class PPlanet: public PCelestialMass
{
public:

	long		satellitecount;
	PSatellite*	satellites;

	void		Earth();

	//void		Randomize();
};

class PSun: public PCelestialMass
{
public:
	
};

class PSolarSystem
{
public:
	
	void RenderSkyBox(); // wtf
};

class PGalaxy
{
public:
	
};

/*

class PGroup
{

}

class PSuperCluster
{

}

class PUniverse
{
public:
	
};*/

#endif