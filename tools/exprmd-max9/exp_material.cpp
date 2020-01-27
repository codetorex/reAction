#include "StdAfx.h"
#include "exporter.h"

RMDMaterial::RMDMaterial()
{
	memset(matname,0,64);
}

void MyExporter::ColorToByteArray(Color cp, byte* bp)
{
	bp[0] = cp.r;
	bp[1] = cp.g;
	bp[2] = cp.b;
}


void MyExporter::LoadMaterialData(RMDMaterial* rmdm,Mtl* maxm)
{
	// load material info
	sprintf_s(rmdm->matname,"%s\0",maxm->GetName());
/*	ColorToByteArray(maxm->GetAmbient(),rmdm->ambclr);
	ColorToByteArray(maxm->GetDiffuse(),rmdm->difclr);
	ColorToByteArray(maxm->GetSpecular(),rmdm->speclr);
	rmdm->selfilum = maxm->GetSelfIllum();
	ColorToByteArray(maxm->GetSelfIllumColor(),rmdm->silclr);
	rmdm->xparency = maxm->GetXParency();*/
	
	/*if (maxm->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		Texmap *tmap = maxm->GetSubTexmap(ID_DI);
		if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt;
			bmt = (BitmapTex*) tmap;
			sprintf_s(rmdm->diffusemap,"%s",bmt->GetMapName());
			rmdm->flags = MAP_DIFFUSE;
		}
	}*/



	// load texture maps
	/*Texmap *tmap;
	BitmapTex *bmt;

	tmap = maxm->GetSubTexmap(ID_DI);
	if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		bmt = (BitmapTex*) tmap;
		sprintf_s(rmdm->diffusemap,"%s",bmt->GetMapName());
		rmdm->flags |= MAP_DIFFUSE;
	}

	tmap = maxm->GetSubTexmap(ID_OP);
	if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		bmt = (BitmapTex*) tmap;
		sprintf_s(rmdm->opacitymap,"%s",bmt->GetMapName());
		rmdm->flags |= MAP_OPACITY;
	}

	tmap = maxm->GetSubTexmap(ID_BU);
	if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		bmt = (BitmapTex*) tmap;
		sprintf_s(rmdm->bumpmap,"%s",bmt->GetMapName());
		rmdm->flags |= MAP_BUMP;
	}

	tmap = maxm->GetSubTexmap(ID_RL);
	if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		bmt = (BitmapTex*) tmap;
		sprintf_s(rmdm->reflection,"%s",bmt->GetMapName());
		rmdm->flags |= MAP_REFLECTION;
	}
	CStr temp;
	tmap->GetClassName(temp);
	if (strcmp(temp.data(),"Raytrace") == 0)
	{
		rmdm->flags |= MAP_REFLECTION | MAP_RAYTRACE;
	}*/

}

long MyExporter::ExportMaterial(Mtl* matnode)
{
	int i;
	RMDMaterial* rp;
	for (i=0;i<ot.count;i++)
	{
		rp = (RMDMaterial*)ot.pArray[i];
		if (strcmp(matnode->GetName().data(),rp->matname) == 0 )
		{
			return i;
		}
	}

	

	BOOL mlmat;
	mlmat = matnode->IsMultiMtl();

	if (mlmat)
	{
		AddDbg("Exporting multi-material: %s",matnode->GetName());
		long smatc;
		smatc = matnode->NumSubMtls();
		//AddDbg("Exporting material: %s",matnode->GetName());
		RMDMaterial* mmat = new RMDMaterial; // main material
		ot.add((void*)mmat);
		sprintf_s(mmat->matname,"%s",matnode->GetName());
		mmat->flags = MAP_MULTI;
		for (i =0 ;i<smatc ; i++)
		{
			RMDMaterial* bos = new RMDMaterial;
			Mtl* maxm = matnode->GetSubMtl(i);
			AddDbg("Exporting sub-material: %s",maxm->GetName());
			LoadMaterialData(bos,maxm);
			mmat->submtls.add((void*)bos);
		}
		
	}
	else
	{
		AddDbg("Exporting material: %s",matnode->GetName());
		RMDMaterial* bos = new RMDMaterial;
		LoadMaterialData(bos,matnode);
		bos->flags = 0;
		ot.add((void*)bos);
	}
	fileheader.texturecount++;
	return ot.count-1;
}