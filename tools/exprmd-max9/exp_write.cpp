#include "stdafx.h"
#include "exporter.h"

void MyExporter::CreateDirectoryStructure()
{
	RMDDirectory *bosdir;
	fileheader.directorysize = 0;

	if (ISEXP(EXPORT_MESH))
	{
		if (om.count > 0)
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_object;
			bosdir->count = om.count;
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
	}

	if (ISEXP(EXPORT_CAMERA))
	{
		if (oc.count > 0)
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_camera;
			bosdir->count = oc.count;
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
	}

	if (ISEXP(EXPORT_LIGHT))
	{
		if (ol.count > 0)
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_light;
			bosdir->count = ol.count;
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
	}

	if (ISEXP(EXPORT_MATERIALS))
	{
		if (ot.count > 0)
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_texture;
			bosdir->count = ot.count;
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
	}

	if (os.bonecount > 0) // check for bone count
	{
		if (ISEXP(EXPORT_PHYSIQUE))
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_bonebase;
			bosdir->count = 1; // because exporting one skeleton
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
		else if (ISEXP(EXPORT_SKELETAL))
		{
			bosdir = new RMDDirectory;
			bosdir->chunktype = c_boneanim;
			bosdir->count = 1; // because exporting one skeleton
			bosdir->offsets = new long [bosdir->count];
			fileheader.directorysize++;
			od.add((void*)bosdir);
		}
	}
}

void MyExporter::WriteToFile()
{
	int i;
	int directorystart;
	AddDbg("Creating directory structure...");
	CreateDirectoryStructure();
	AddDbg("Writing directory structure...");

	AddDbg("Writing header...");
	WriteHeader(m_fileStream,&fileheader); // write header

	directorystart = ftell(m_fileStream);
	for (i=0;i<od.count;i++)
	{
		WriteDirectory(m_fileStream,(RMDDirectory*)od.pArray[i]);
	}

	RMDDirectory* rdir;
	if (om.count > 0)
	{
		if (ISEXP(EXPORT_MESH))
		{
			AddDbg("Writing meshes...");
			rdir = GetDirectoryByType(c_object);
			for (i=0;i<om.count;i++)
			{
				if (rdir != 0)
				{
					rdir->offsets[i] = ftell(m_fileStream);
				}
				else
				{
					AddDbg("Error: Directory not set");
				}
				WriteMesh(m_fileStream,(RMDMesh*)om.pArray[i]);
			}
		}
	}

	if (oc.count > 0)
	{
		if (ISEXP(EXPORT_CAMERA))
		{
			AddDbg("Writing cameras...");
			rdir = GetDirectoryByType(c_camera);
			for (i=0;i<oc.count;i++)
			{
				rdir->offsets[i] = ftell(m_fileStream);
				WriteCamera(m_fileStream,(RMDCamera*)oc.pArray[i]);
			}
		}

	}

	if (ol.count > 0)
	{
		if (ISEXP(EXPORT_LIGHT))
		{
			AddDbg("Writing lights...");
			rdir = GetDirectoryByType(c_light);
			for (i=0;i<ol.count;i++)
			{
				rdir->offsets[i] = ftell(m_fileStream);
				WriteLight(m_fileStream,(RMDLight*)ol.pArray[i]);
			}
		}
	}

	if (ot.count > 0)
	{
		if(ISEXP(EXPORT_MATERIALS))
		{
			AddDbg("Writing material info...");
			rdir = GetDirectoryByType(c_texture);
			for(i=0;i<ot.count;i++)
			{
				rdir->offsets[i] = ftell(m_fileStream);
				WriteMaterial(m_fileStream,(RMDMaterial*)ot.pArray[i]);
			}
		}
	}

	if (os.bonecount > 0)
	{
		// dont export them at same time
		if (ISEXP(EXPORT_PHYSIQUE))
		{
			AddDbg("Writing base skeleton info...");
			rdir = GetDirectoryByType(c_bonebase);
			rdir->offsets[0] = ftell(m_fileStream);
			WriteBaseSkeleton(m_fileStream,&os);
		}
		else if (ISEXP(EXPORT_SKELETAL))
		{
			AddDbg("Writing animated skeleton info...");
			rdir = GetDirectoryByType(c_boneanim);
			rdir->offsets[0] = ftell(m_fileStream);
			WriteAnimSkeleton(m_fileStream,&os);		
		}
	}

	fseek(m_fileStream,directorystart,SEEK_SET);
	AddDbg("Updating directory structure...");
	for (i=0;i<od.count;i++)
	{
		WriteDirectory(m_fileStream,(RMDDirectory*)od.pArray[i]);
	}


	AddDbg("Closing file...");
	fclose(m_fileStream);
}

#define writelong(a) fwrite(&a,4,1,fp)
#define writestr(a,ln) fwrite(a,1,ln,fp);
#define writebool(a) fwrite(&a,1,1,fp);

void MyExporter::WriteBoneKeyframe(FILE* fp,RMDBoneKeyFrame* kf)
{
	fwrite(kf->pos,4,3,fp);
	fwrite(kf->rot,4,3,fp);
	fwrite(kf->mat,4,12,fp);
	writelong(kf->time);
}

void MyExporter::WriteBaseBone(FILE* fp,RMDBone* bn)
{
	int t = -1;
	writelong(bn->boneid);
	writestr(bn->bonename,128);
	if (bn->parent != 0)
	{
		writelong(bn->parent->boneid);
	}
	else
	{
		writelong(t);
	}
	fwrite(bn->m_init,4,12,fp);
	//WriteBoneKeyframe(fp,&bn->m_init);
}

void MyExporter::WriteBaseSkeleton(FILE* fp,RMDSkeleton* sk)
{
	writelong(sk->bonecount);

	int i;
	for (i=0;i<sk->bonecount;i++)
	{
		WriteBaseBone(fp,(RMDBone*)sk->bones.pArray[i]);
	}
}

void MyExporter::WriteAnimBone(FILE* fp,RMDBone* bn)
{
	int t = -1;
	writelong(bn->boneid);
	writestr(bn->bonename,128);
	if (bn->parent != 0)
	{
		writelong(bn->parent->boneid);
	}
	else
	{
		writelong(t);
	}

	writelong(bn->framec);
	int i;
	for (i=0;i<bn->framec;i++)
	{
		WriteBoneKeyframe(fp,&bn->kframes[i]);
	}
}

void MyExporter::WriteAnimSkeleton(FILE* fp,RMDSkeleton* sk)
{
	writelong(sk->bonecount);
	writelong(sk->keystart);
	writelong(sk->keyend);
	writelong(sk->framerate);

	int i;
	for (i=0;i<sk->bonecount;i++)
	{
		WriteAnimBone(fp,(RMDBone*)sk->bones.pArray[i]);
	}

}

void MyExporter::WriteHeader(FILE* fp,RMDHeader* hdr)
{
	writelong(hdr->signature);
	writelong(hdr->directorysize);
	//fwrite(&hdr->signature,4,1,fp);
//	fwrite(&hdr->directorysize,4,1,fp);
/*	fwrite(&hdr->objectcount,4,1,fp);
	fwrite(&hdr->lightcount,4,1,fp);
	fwrite(&hdr->cameracount,4,1,fp);
	fwrite(&hdr->texturecount,4,1,fp);
	fwrite(&hdr->entitycount,4,1,fp);
	fwrite(&hdr->ambientlight,4,1,fp);
	fwrite(&hdr->flags,4,1,fp);*/
}

void MyExporter::WriteDirectory(FILE* fp,RMDDirectory* rdr)
{
	writelong(rdr->chunktype);
	writelong(rdr->count);
	fwrite(rdr->offsets,4,rdr->count,fp);
}

void MyExporter::WriteVertexLink(FILE* fp,RMDWeightedVertex* vtx)
{
	writelong(vtx->ibc);
	
	if (vtx->ibc != vtx->curbone)
	{
		AddDbg("Error: Some vertexes has missing link data. %i / %i",vtx->curbone,vtx->ibc);
	}

	if (vtx->ibc > 0)
	{
		fwrite(vtx->boneids,4,vtx->ibc,fp);
		fwrite(vtx->weights,4,vtx->ibc,fp);
	}
	else
	{
		AddDbg("Error: Some vertexes has no link!");
	}
}

void MyExporter::WriteMesh(FILE* fp,RMDMesh* msh)
{
	AddDbg("Writing mesh data: %s",msh->objname);
	int firstp;
	firstp = ftell(fp);
	fwrite(msh->objname,1,64,fp);

	fwrite(&msh->matid,4,1,fp);
	fwrite(&msh->vertexcount,4,1,fp);
	fwrite(&msh->facecount,4,1,fp);
	fwrite(&msh->tvcount,4,1,fp);
	fwrite(&msh->normalcount,4,1,fp);
	fwrite(&msh->hasphysique,1,1,fp);

	fwrite(msh->center,4,3,fp);
	fwrite(msh->boundingbox,4,6,fp);
	
	fwrite(msh->vexs,4,msh->vertexcount*3,fp);
	fwrite(msh->faces,4,msh->facecount<<2,fp);
	fwrite(msh->texcords,4,msh->tvcount<<1,fp);
	fwrite(msh->tfaces,4,msh->facecount*3,fp);
	fwrite(msh->normals,4,msh->facecount*9,fp);

	
	if (msh->hasphysique)
	{
		AddDbg("Writing link data: %s",msh->objname);
		int i;
		for (i=0;i<msh->vertexcount;i++)
		{
			WriteVertexLink(fp,&msh->boneinfo[i]);
		}
		AddDbg("Mesh and link data written: %s (%i bytes)",msh->objname,ftell(fp) - firstp);
//		fwrite(msh->boneid,4,msh->vertexcount,fp);
	}
	else
	{
		AddDbg("Mesh data written: %s (%i bytes)",msh->objname,ftell(fp) - firstp);
	}
}

void MyExporter::WriteCamera(FILE* fp,RMDCamera* cam)
{
	AddDbg("Writing camera data: %s",cam->cameraname);
	fwrite(cam->cameraname,1,64,fp);
	fwrite(cam->camera,4,3,fp);
	fwrite(cam->target,4,3,fp);
}

void MyExporter::WriteLight(FILE* fp, RMDLight* lig)
{
	AddDbg("Writing light data: %s",lig->lightname);
	fwrite(lig->lightname,1,64,fp);
	fwrite(&lig->lighttype,4,1,fp);
	fwrite(lig->lightpos,4,3,fp);
	fwrite(lig->targetpos,4,3,fp);
	fwrite(&lig->intensity,4,1,fp);
	fwrite(lig->atten,4,4,fp);
	fwrite(lig->clr,1,3,fp);
}

void MyExporter::WriteMaterial(FILE* fp,RMDMaterial* mt)
{
	AddDbg("Writing material data: %s",mt->matname);
	fwrite(mt->matname,1,64,fp);
	fwrite(&mt->flags,4,1,fp);
/*	fwrite(mt->ambclr,1,3,fp);
	fwrite(mt->difclr,1,3,fp);
	fwrite(mt->speclr,1,3,fp);
	fwrite(&mt->selfilum,4,1,fp);
	fwrite(mt->silclr,1,3,fp);
	fwrite(&mt->xparency,4,1,fp);

	fwrite(mt->diffusemap,1,128,fp);
	fwrite(mt->opacitymap,1,128,fp);
	fwrite(mt->bumpmap,1,128,fp);
	fwrite(mt->reflection,1,128,fp);*/

	if (mt->flags & MAP_MULTI)
	{
		fwrite(&mt->submtls.count,4,1,fp);// write sub mtl count
		int i;
		RMDMaterial* mp;
		for (i=0;i<mt->submtls.count;i++)
		{
			mp = (RMDMaterial*)mt->submtls.pArray[i];
			WriteMaterial(fp,mp);
		}
	}
}