#include "stdafx.h" 
#include "vmodel.h"
#include "../tools/rtools.h"
#include "../Tools/rfilesystem.h"
#include "ext/GL_ARB_multitexture.h"


using namespace RTools;

VRMDMemory mModels;

vec3 RMDMesh::GetAverageNormals(IN int faceid)
{
	vec3 n1,n2,n3,r;
	int fix;
	fix = faceid * 9;
	n1.set(&normals[fix]); fix+=3;
	n2.set(&normals[fix]); fix+=3;
	n3.set(&normals[fix]);
	r = (n1 + n2 + n3) * 0.333f;
	//r.normalize();

	return r;
}

void RMDMesh::GetVertices(IN int faceid, OUT vec3* v1, OUT vec3* v2, OUT vec3* v3,OUT int* pindices)
{
	int fix; // face index start
	int f1,f2,f3;
	fix = faceid<<2; // * 4
	f1 = faces[fix++]; // vertex index
	f2 = faces[fix++];
	f3 = faces[fix];
	
	if (pindices)
	{
		pindices[0] = f1;
		pindices[1] = f2;
		pindices[2] = f3;
	}
	
	if ( v1 && v2 && v3 )
	{
		f1 = (f1 << 1) + f1;
		f2 = (f2 << 1) + f2;
		f3 = (f3 << 1) + f3;
		/*f1 = f1 * 3;
		f2 = f2 * 3;
		f3 = f3 * 3;*/
		
		v1->set(&vexs[f1]);
		v2->set(&vexs[f2]);
		v3->set(&vexs[f3]);
	}
}

void RMDMesh::GetUV(IN int faceid,OUT vec2* v1, OUT vec2* v2, OUT vec2* v3)
{
	int fix; // face index start
	int f1,f2,f3;
	fix = (faceid<<1) + faceid; // * 4

	f1 = tfaces[fix++] << 1;
	f2 = tfaces[fix++] << 1;
	f3 = tfaces[fix] << 1;
	
	if ( v1 && v2 && v3 )
	{
		v1->set(&texcords[f1]);
		v2->set(&texcords[f2]);
		v3->set(&texcords[f3]);
	}
}

void RMDMesh::GetUV(IN int faceid,OUT vec3* v1, OUT vec3* v2, OUT vec3* v3)
{
	vec2 y1,y2,y3;
	GetUV(faceid,&y1,&y2,&y3);
	*v1 = y1; *v2 = y2; *v3 = y3; // set x,y
}

// pindices = vertex indices of faces

void RMDMesh::GetTriangle(int faceid,tri3* triangle,int* pindices)
{
	if (triangle)
	{
		GetVertices(faceid,&triangle->v1,&triangle->v2,&triangle->v3,pindices);
	}
	else
	{
		GetVertices(faceid,0,0,0,pindices);
	}
}

void RMDMesh::CalculateTangentSpace(IN int faceid, OUT vec3 &fnor, OUT vec3 &ftan, OUT vec3 &fbin)
{
	vec3 v0,v1,v2; // triangle vertices
	vec3 t0,t1,t2; // texture coordinates

	GetVertices(faceid,&v0,&v1,&v2);
	GetUV(faceid,&t0,&t1,&t2);

	vec3 side0,side1;
	side0 =v1-v0;
	side1 =v2-v0;
	fnor=cross(side0,side1);
	fnor.normalize();

	float deltaT0,deltaT1;
	deltaT0=t1.y-t0.y;
	deltaT1=t2.y-t0.y;
	ftan = side0*deltaT1-side1*deltaT0;
	ftan.normalize();	

	float deltaS0,deltaS1;
	deltaS0=t1.x - t0.x;
	deltaS1=t2.x - t0.x;
	fbin=side0*deltaS1 - side1*deltaS0;
	fbin.normalize();

	vec3 tangcro;
	tangcro=cross(ftan,fbin);
	if(dot(tangcro,fnor)<0.0f)
	{
		ftan=-ftan;
		fbin=-fbin;
	}
}

void RMDCamera::SetCamera(VCamera* cm)
{
	cm->SetCamera(camera[0], camera[1], camera[2],target[0], target[1], target[2],0.0f, 1.0f, 0.0f);
}

RMDModel* LoadRMD(char* rmdpath,char* basetd,long loadparams /* = RMD_ALL */)
{
	return mModels.LoadRMD(rmdpath,basetd,loadparams); 
}

RMDModel::RMDModel()
{
	meshcount = 0;
	cameracount = 0;
	lightcount = 0;
	matcount = 0;
	directorys = 0;
	meshes = 0;
	cameras = 0;
	lights = 0;
	mats = 0;
	skeleton = 0;
	skanim = 0;
	rmdname = 0;
	filepath = 0;
	matbasepath = 0;
	rmdfp = 0;
}

RMDModel* VRMDMemory::LoadRMD(char* rmdpath,char* basetd,long loadparams /* = RMD_ALL */)
{
	RFile* rmd = ROpenFile(rmdpath,"rb");
	
	if (rmd == 0)
	{
		return 0;
	}

	long tsig; // temp signature
	rmd->getdata(&tsig,4,1);
	if (tsig != 541347154)
	{
		return 0;
	}

	// Valid RMD now create object
	RMDModel* nrmd = new RMDModel;
	nrmd->fileheader.signature = tsig;
	
	rmd->getdata(&nrmd->fileheader.directorysize,4,1);
	nrmd->rmdfp = rmd;
	// todo: change this
	if (basetd)
	{
		strcpy(nrmd->fileheader.matbasepath,basetd);
	}

	nrmd->LoadDirectorys();
	
	if (loadparams & RMD_MESH)
	{
		nrmd->LoadMeshes();
	}

	if (loadparams & RMD_CAMERA )
	{
		nrmd->LoadCameras();
	}

	if ( loadparams & RMD_LIGHT )
	{
		nrmd->LoadLights();
	}

	if ( loadparams & RMD_TEXTURE )
	{
		nrmd->LoadMaterials();
	}

	if ( loadparams & RMD_PHYSIQUE )
	{
		nrmd->LoadBaseSkeleton();
	}

	if ( loadparams & RMD_SKELETAL )
	{
		nrmd->LoadAnimSkeleton();
	}

	if (nrmd->skeleton)
	{
		nrmd->FixBoneConnections();
	}

	RCloseFile(rmd);

	return nrmd;
}

void RMDModel::LoadDirectorys()
{
	directorys = new RMDDirectory [fileheader.directorysize];
	RMDDirectory* di;
	for (int i=0;i<fileheader.directorysize;i++)
	{
		di = &directorys[i];	
		rmdfp->getdata(&di->chunktype,4,1);
		rmdfp->getdata(&di->count,4,1);
		di->offsets = new long [di->count];
		rmdfp->getdata(di->offsets,4,di->count);
	}
}

void RMDModel::LoadMeshes()
{

	RMDDirectory* objdir = GetDirectory(c_object);
	
	if ( objdir == 0) return;
	
	meshes = new RMDMesh [objdir->count];
	meshcount = objdir->count;
	
	RMDMesh* cmesh = 0;
	for (int i=0;i<objdir->count;i++)
	{
		cmesh = &meshes[i];
		rmdfp->setpos(objdir->offsets[i]);
		rmdfp->getdata(cmesh->objname,1,64);
		
		rmdfp->getdata(&cmesh->matid,4,1);
		rmdfp->getdata(&cmesh->vertexcount,4,1);
		rmdfp->getdata(&cmesh->facecount,4,1);
		rmdfp->getdata(&cmesh->tvcount,4,1);
		rmdfp->getdata(&cmesh->normalcount,4,1);
		rmdfp->getdata(&cmesh->hasphysique,1,1);

		rmdfp->getdata(cmesh->center,4,3);
		rmdfp->getdata(cmesh->boundingbox,4,6);

		cmesh->vexs = new float [cmesh->vertexcount * 3];
		cmesh->faces = new int [cmesh->facecount * 4];
		cmesh->texcords = new float [cmesh->tvcount * 2];
		cmesh->tfaces = new int [cmesh->facecount * 3];
		cmesh->normals = new float [cmesh->facecount * 9];

		cmesh->vexsanim = new float [cmesh->vertexcount * 3]; // Warnning!

		rmdfp->getdata(cmesh->vexs,4, cmesh->vertexcount*3);
		rmdfp->getdata(cmesh->faces,4, cmesh->facecount*4);
		rmdfp->getdata(cmesh->texcords,4, cmesh->tvcount*2);
		rmdfp->getdata(cmesh->tfaces,4, cmesh->facecount*3);
		rmdfp->getdata(cmesh->normals,4, cmesh->facecount*9);

		if (cmesh->hasphysique)
		{
			cmesh->boneinfo = new RMDWeightedVertex [cmesh->vertexcount];
			RMDWeightedVertex* cv; // current vertex
			for (int k=0;k<cmesh->vertexcount;k++)
			{
				cv = &cmesh->boneinfo[k];
				rmdfp->getdata(&cv->ibc,4,1);
				
				cv->boneids = new RMDBone * [cv->ibc];
				cv->weights = new float [cv->ibc];

				rmdfp->getdata(cv->boneids,4,cv->ibc);
				rmdfp->getdata(cv->weights,4,cv->ibc);
			}
		}
	}

}

void RMDModel::LoadCameras()
{
	RMDDirectory* camdir = GetDirectory(c_camera);
	
	if ( camdir == 0) return;

	cameracount = camdir->count;
	cameras = new RMDCamera [camdir->count];

	RMDCamera* ccam; // current camera
	for (int i=0;i<camdir->count;i++)
	{
		ccam = &cameras[i];
		rmdfp->setpos(camdir->offsets[i]);

		rmdfp->getdata(ccam->cameraname,1,64);
		rmdfp->getdata(ccam->camera,4,3);
		rmdfp->getdata(ccam->target,4,3);
	}
}

void RMDModel::LoadLights()
{
	RMDDirectory* ldir = GetDirectory(c_light);
	
	if ( ldir == 0) return;
	
	lightcount = ldir->count;
	lights = new RMDLight [ldir->count];
	
	RMDLight* clig; // current camera
	for (int i=0;i<ldir->count;i++)
	{
		clig = &lights[i];
		rmdfp->setpos(ldir->offsets[i]);
		
		rmdfp->getdata(clig->lightname,1,64);
		rmdfp->getdata(&clig->lighttype,4,1);
		rmdfp->getdata(clig->lightpos,4,3);
		rmdfp->getdata(clig->targetpos,4,3);
		rmdfp->getdata(&clig->intensity,4,1);
		rmdfp->getdata(clig->atten,4,4);
		rmdfp->getdata(clig->clr.bclr,1,3);
	}
}

void RMDModel::LoadMaterials()
{
	RMDDirectory* matdir = GetDirectory(c_texture);
	
	if ( matdir == 0) return;
	
	matcount = matdir->count;
	mats = new RMDMaterial [matdir->count];

	char tmppath[256];
	strcpy(tmppath,fileheader.matbasepath);
	int mpstart = strlen(fileheader.matbasepath);
	char* mpath = tmppath + mpstart;
	
	char tmpname[64];
	long tmpflags;

	RMDMaterial* cmat;
	for (int i = 0;i<matdir->count;i++)
	{
		cmat = &mats[i];
		rmdfp->setpos(matdir->offsets[i]);
		
		rmdfp->getdata(cmat->matname,1,64);
		rmdfp->getdata(&cmat->flags,4,1);

		if (cmat->flags & MAP_MULTI)
		{
			rmdfp->getdata(&cmat->submatcount,4,1);	
			cmat->multimat = new VMaterial * [cmat->submatcount];

			for (int k=0;k<cmat->submatcount;k++)
			{
				rmdfp->getdata(tmpname,1,64);
				rmdfp->getdata(&tmpflags,4,1);

				sprintf(mpath,"%s.rmt",tmpname);
				cmat->multimat[k] = LoadRMT(tmppath);
				cmat->multimat[k]->flags = tmpflags;
			}
			// submaterials loaded
		}
		else
		{
			sprintf(mpath,"%s.rmt",cmat->matname);
			cmat->singlmat = LoadRMT(tmppath);
			cmat->singlmat->flags = cmat->flags;
			// material loaded
		}

	}

}

matrix4x4f CreateMatrix(float x,float y,float z, float rotx,float roty,float rotz)
{
	matrix4x4f res;

	quat tq = qfromeuler(rotx,roty,rotz);
	tq.tomatrix(res.m);
	res.transposerotation();

	res.m[12] = x;
	res.m[13] = y;
	res.m[14] = z;

	return res;
}

void RMDModel::ReadBoneKeyframe(RMDBoneKeyFrame* bkf)
{
	float tempM[12];
	rmdfp->getdata(&bkf->pos,4,3);
	rmdfp->getdata(&bkf->rot,4,3);
	rmdfp->getdata(tempM,4,12);

	//bkf->mat = CreateMatrix(bkf->pos[0],bkf->pos[1],bkf->pos[2],RADTODEG(bkf->rot[0]),RADTODEG(bkf->rot[1]),RADTODEG(bkf->rot[2]));



	bkf->mat.m[12] = tempM[9];
	bkf->mat.m[13] = tempM[10];
	bkf->mat.m[14] = tempM[11];
	// and rotation;
	// currently not know is it transposed or not
	bkf->mat.m[0]  = tempM[0];
	bkf->mat.m[1]  = tempM[1];
	bkf->mat.m[2]  = tempM[2];
	
	bkf->mat.m[4]  = tempM[3];
	bkf->mat.m[5]  = tempM[4];
	bkf->mat.m[6]  = tempM[5];
	
	bkf->mat.m[8]  = tempM[6];
	bkf->mat.m[9]  = tempM[7];
	bkf->mat.m[10] = tempM[8];

//	bkf->mat.transposerotation();

	rmdfp->getdata(&bkf->time,4,1);
}

void RMDModel::LoadBaseSkeleton()
{
	RMDDirectory* skdir = GetDirectory(c_bonebase);
	
	if ( skdir == 0) return;

	skeleton = new RMDSkeleton;
	
	rmdfp->setpos(skdir->offsets[0]);
	rmdfp->getdata(&skeleton->bonecount,4,1); // get bonecount
	skeleton->bones = new RMDBone * [skeleton->bonecount];
	float tempM[12]; // temp matrix

	RMDBone* curbone;
	for (int i=0;i<skeleton->bonecount;i++)
	{
		skeleton->bones[i] = new RMDBone;
		curbone = skeleton->bones[i];
		rmdfp->getdata(&curbone->boneid,4,1);
		rmdfp->getdata(curbone->bonename,1,128);
		curbone->bonenamehash = shash(curbone->bonename);
		curbone->sklt = skeleton;

		rmdfp->getdata(&curbone->parent,4,1);
		//ReadBoneKeyframe(&curbone->m_init);
		//rmdfp->getdata(curbone->m_init.m,4,12);
		rmdfp->getdata(tempM,4,12);
		curbone->m_init.identity();
		// translate
		curbone->m_init.m[12] = tempM[9];
		curbone->m_init.m[13] = tempM[10];
		curbone->m_init.m[14] = tempM[11];
		// and rotation;
		// currently not know is it transposed or not
		curbone->m_init.m[0]  = tempM[0];
		curbone->m_init.m[1]  = tempM[1];
		curbone->m_init.m[2]  = tempM[2];

		curbone->m_init.m[4]  = tempM[3];
		curbone->m_init.m[5]  = tempM[4];
		curbone->m_init.m[6]  = tempM[5];

		curbone->m_init.m[8]  = tempM[6];
		curbone->m_init.m[9]  = tempM[7];
		curbone->m_init.m[10] = tempM[8];

		curbone->m_init.transposerotation();
	}

	skeleton->baseskeleton = true;
	skeleton->keystart = 0;
	skeleton->keyend = 0;
	skeleton->framerate = 0;
}

void RMDModel::LoadAnimSkeleton()
{
	RMDDirectory* skdir = GetDirectory(c_boneanim);
	
	if ( skdir == 0) return;
	
	skeleton = new RMDSkeleton;

	rmdfp->setpos(skdir->offsets[0]);
	rmdfp->getdata(&skeleton->bonecount,4,1); // get bonecount
	rmdfp->getdata(&skeleton->keystart,4,1); 
	rmdfp->getdata(&skeleton->keyend,4,1);
	rmdfp->getdata(&skeleton->framerate,4,1);
	skeleton->bones = new RMDBone * [skeleton->bonecount];


	RMDBone* curbone;
	for (int i=0;i<skeleton->bonecount;i++)
	{
		skeleton->bones[i] = new RMDBone;
		curbone = skeleton->bones[i];
		rmdfp->getdata(&curbone->boneid,4,1);
		rmdfp->getdata(curbone->bonename,1,128);
		curbone->bonenamehash = shash(curbone->bonename);
		curbone->sklt = skeleton;

		rmdfp->getdata(&curbone->parent,4,1);
		rmdfp->getdata(&curbone->framec,4,1);
		curbone->kframes = new RMDBoneKeyFrame [curbone->framec];
		for (int k=0;k<curbone->framec;k++)
		{
			ReadBoneKeyframe(&curbone->kframes[k]);
		}
	}
	skeleton->baseskeleton = false;
}

RMDDirectory* RMDModel::GetDirectory(RMDChunk chnktyp)
{
	for (int i=0;i<fileheader.directorysize;i++)
	{
		if ( directorys[i].chunktype == chnktyp)
		{
			return &directorys[i];
		}
	}
	return 0;
}

RMDWeightedVertex::RMDWeightedVertex()
{
	ibc = 0;
	boneids = 0;
	weights = 0;
}

RMDBone* RMDSkeleton::GetBoneById(int bid)
{
	int i;
	RMDBone* cbone;
	for ( i=0;i<bonecount;i++)
	{
		cbone = bones[i];
		if (cbone->boneid == bid)
		{
			return cbone; 
		}
	}
	return 0;
}

void RMDModel::FixBoneConnections()
{
	int i,k,j;
	RMDMesh* cmesh;
	RMDWeightedVertex* cvex;
	long bid;
	for (i=0;i<meshcount;i++)
	{
		cmesh = &meshes[i];
		if (cmesh->hasphysique)
		{
			for (k=0;k<cmesh->vertexcount;k++)
			{
				cvex = &cmesh->boneinfo[k];
				for (j=0;j<cvex->ibc;j++)
				{
					bid = (long)cvex->boneids[j];
					cvex->boneids[j] = skeleton->GetBoneById(bid);
				}
			}
		}
	}

	RMDBone* cbone;
	for (i=0;i<skeleton->bonecount;i++)
	{
		cbone = skeleton->bones[i];
		if ((int)cbone->parent != -1)
		{
			cbone->parent = skeleton->GetBoneById((int)cbone->parent);
			cbone->parent->childs.add((void*)cbone);
		}
		else
		{
			cbone->parent = 0;
		}
	}
}

/*void RMDModel::PrepareForSkeletalAnimation()
{
	int i,j,k;
	RMDMesh* cmesh;
	RMDWeightedVertex* cvex;
	matrix4x4f tmat; // temp matrix
	vec3 cvec;
	vec3 tvec; // temp vector
	if (skeleton->baseskeleton)
	{
		for (i=0;i<meshcount;i++)
		{
			cmesh = &meshes[i];
			if (cmesh->hasphysique)
			{
				cmesh->vexsbase = new float [cmesh->vertexcount*3];
				long cvexmem = 0;
				for (j = 0;j<cmesh->vertexcount;j++)
				{
					cvex = &cmesh->boneinfo[j];
					cvec.x = cmesh->vexs[cvexmem++];
					cvec.y = cmesh->vexs[cvexmem++];
					cvec.z = cmesh->vexs[cvexmem++];


					for (k=0;k<cvex->ibc;k++)
					{
						tmat = cvex->boneids[k]->m_init;

						//tmat.transposerotation();
						
						
					}

				}
			}
		}
	}
	
}*/

RMDBone* RMDSkeleton::GetBoneByNameHash(int hsh)
{
	for (int i=0;i<bonecount;i++)
	{
		if (bones[i]->bonenamehash == hsh)
		{
			return bones[i];
		}
	}
	return 0;
}

void RMDBoneKeyFrame::mul_matrix(matrix4x4f* mt)
{
/*	matrix4x4f tmat;


	// create matrix from position and rotation
	matrix4x4f tmat; // temp matrix
	quat tq = qfromeuler(rot[0],rot[1],rot[2]); // temp quat

	tq.tomatrix(tmat.m);
	//tmat.transposerotation();

	tmat.m[12] = pos[0];
	tmat.m[13] = pos[1];
	tmat.m[14] = pos[2];

	*mt = (*mt) * tmat; */
	
}

byte ac1[] = {128,0,0};
byte ac2[] = {0,128,0};
byte ac3[] = {0,0,128};
byte ac4[] = {255,0,0};
byte ac5[] = {0,255,0};
byte ac6[] = {0,0,255};
byte ac7[] = {255,255,0};
byte ac8[] = {0,255,255};
byte ac9[] = {255,0,255};

byte* clrs[] = { ac1,ac2,ac3,ac4,ac5,ac6,ac7,ac8,ac9};

void RMDBone::drawchilds(int dz)
{
	int i;
	if (parent != 0)
	{
		glColor3ubv(clrs[dz%3]);
		glBegin(GL_LINES);
			glVertex3fv(&parent->m_final.m[12]);
			glVertex3fv(&m_final.m[12]);
		glEnd();
	}

	RMDBone* cc;
	for (i=0;i<childs.count;i++)
	{
		cc = (RMDBone*)(childs.pArray[i]);
		cc->drawchilds(dz+1);
	}

	glColor3ub(255,255,255);
	glBegin(GL_POINTS);
	glVertex3fv(&m_final.m[12]);
	glEnd();
}

void RMDBone::drawbase(int dz)
{
	int i;
	
	if (parent != 0)
	{
		//glColor3ubv(clrs[dz%9]);
		glColor3ubv(clrs[dz%3]);
		glBegin(GL_LINES);
			glVertex3fv(&parent->m_init.m[12]);
			glVertex3fv(&m_init.m[12]);
		glEnd();
	}


	RMDBone* cc;
	for (i=0;i<childs.count;i++)
	{
		cc = (RMDBone*)(childs.pArray[i]);
		cc->drawbase(dz+1);
	}

	glColor3ub(255,255,255);
	glBegin(GL_POINTS);
	glVertex3fv(&m_init.m[12]);
	glEnd();

}

void RMDBone::setchilds(int frameid)
{
	int i;
	if (parent != 0)
	{
		m_final = parent->m_final;
		m_final = m_final * kframes[frameid].mat;
	}
	else
	{
		m_final.identity();
		m_final = m_final * kframes[frameid].mat;
	}
	
	//kframes[frameid].mul_matrix(&m_final);
	
	RMDBone* cc;
	for (i=0;i<childs.count;i++)
	{
		cc= (RMDBone*)(childs.pArray[i]);
		cc->setchilds(frameid);
	}
}

void RMDModel::BoneMul(RMDBone* bn,float weig,vec3* vx)
{
	// inverse translate
	vx->x -= bn->m_init.m[12];
	vx->y -= bn->m_init.m[13];
	vx->z -= bn->m_init.m[14];
	//bn->m_init.transposerotation();
	bn->m_init.transformVector(vx); // only applies rotation
	
	vec3 lpos = *vx;
	
	bn->m_final.transformPoint(&lpos); // use final matrix
	//bn->m_final.m[12]
	vx->x = (lpos.x) * weig;
	vx->y = (lpos.y) * weig;
	vx->z = (lpos.z) * weig;

	/*vx->x = (vx->x + bn->m_final.m[12]) * weig;
	vx->y = (vx->y + bn->m_final.m[13]) * weig;
	vx->z = (vx->z + bn->m_final.m[14]) * weig;*/
}


void RMDModel::RenderSkeleton(int frameid)
{
	int i,j, k, l,m;
	int v1,v2,v3,mt;
	int t1,t2,t3;
	vec3 x0,x1,x2,n1;
	//glBindTexture(GL_TEXTURE_2D,0);

	// firstly prepare skeleton
	RMDBoneKeyFrame* kf;

	for (i=0;i<skanim->bonecount;i++)
	{
		if (skanim->bones[i]->parent == 0)
		{
			skanim->bones[i]->m_final.identity();
			skanim->bones[i]->setchilds(frameid);
		}
	}

	RMDBone* cb;
	for (i=0;i<skeleton->bonecount;i++)
	{
		cb = skanim->GetBoneByNameHash(skeleton->bones[i]->bonenamehash);
		if (cb == 0)
		{
			QuickInfo("Oha! %s",skeleton->bones[i]->bonename);
			skeleton->bones[i]->m_final.identity();
		}
		else
		{
			skeleton->bones[i]->m_final = cb->m_final;
		}
	}
	

	bool mm;
	RMDMaterial* cmat;
	RMDMesh* cmesh;
	for (i = 0 ; i < meshcount ;i++)
	{	

		cmat = &mats[meshes[i].matid];
		if (cmat->flags & MAP_MULTI)
		{
			mm=true;
		}
		else
		{
			mm = false;
			cmat->singlmat->AttachMaterial();
		}
		cmesh = &meshes[i];

		
		
		if (cmesh->hasphysique)
		{
			// skeletal animated object
			RMDWeightedVertex* cvinfo; // current vertex info
			vec3 cv; // current vector
			vec3 tvec; // temp vector
			vec3 fvec; // final vector
			m = l = 0;
			RMDBone* cbn; // current bone
			float cweig;  // current weight
			for (j=0; j<cmesh->vertexcount;j++)
			{
				cvinfo = &cmesh->boneinfo[j];
				cv.x = cmesh->vexs[l++];
				cv.y = cmesh->vexs[l++];
				cv.z = cmesh->vexs[l++];

				fvec.x = 0;
				fvec.y = 0;
				fvec.z = 0;

				for (k=0; k<cvinfo->ibc;k++)
				{
					cbn = cvinfo->boneids[k];
					cweig = cvinfo->weights[k];
					
					tvec = cv;

					BoneMul(cbn,cweig,&tvec);
					fvec += tvec;
				}

				cmesh->vexsanim[m++] = fvec.x;
				cmesh->vexsanim[m++] = fvec.y;
				cmesh->vexsanim[m++] = fvec.z;


			}
			
		}

		m=l=k=0;
		for (j=0; j < meshes[i].facecount ; j++)
		{
			v1 = meshes[i].faces[k++] *3;
			v2 = meshes[i].faces[k++] *3;
			v3 = meshes[i].faces[k++] *3;
			mt = meshes[i].faces[k++]; // WTF?

			if (mm)
			{
				cmat->multimat[mt]->AttachMaterial();
			}
			/*if (mm)
			{
				glBindTexture(GL_TEXTURE_2D,ot[om[i].matid].submtls[mt]->dmap.glID);
			}*/
			
			t1 = meshes[i].tfaces[m++] << 1;
			t2 = meshes[i].tfaces[m++] << 1;
			t3 = meshes[i].tfaces[m++] << 1;
			
			glColor3ub(255,255,255);
			glBegin(GL_TRIANGLES);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t1]);		
				glVertex3fv(&meshes[i].vexsanim[v1]);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t2]);
				glVertex3fv(&meshes[i].vexsanim[v2]);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t3]);
				glVertex3fv(&meshes[i].vexsanim[v3]);
			glEnd();

			/*glBegin(GL_LINES);
				glVertex3fv(&meshes[i].vexs[v1]);
			glEnd();*/

		}
	}
}

void RMDModel::Render()
{
	int i,j, k, l,m;
	int v1,v2,v3,mt;
	int t1,t2,t3;
	vec3 x0,x1,x2,n1;
	//glBindTexture(GL_TEXTURE_2D,0);

	bool mm;
	RMDMaterial* cmat;
	for (i = 0 ; i < meshcount ;i++)
	{	
		m=l=k=0;
		cmat = &mats[meshes[i].matid];
		if (cmat->flags & MAP_MULTI)
		{
			mm=true;
		}
		else
		{
			mm = false;
			cmat->singlmat->AttachMaterial();
		}

		for (j=0; j < meshes[i].facecount ; j++)
		{
			v1 = meshes[i].faces[k++] *3;
			v2 = meshes[i].faces[k++] *3;
			v3 = meshes[i].faces[k++] *3;
			mt = meshes[i].faces[k++]; // WTF?

			if (mm)
			{
				cmat->multimat[mt]->AttachMaterial();
			}
			/*if (mm)
			{
				glBindTexture(GL_TEXTURE_2D,ot[om[i].matid].submtls[mt]->dmap.glID);
			}*/
			
			t1 = meshes[i].tfaces[m++] << 1;
			t2 = meshes[i].tfaces[m++] << 1;
			t3 = meshes[i].tfaces[m++] << 1;
			
			glColor3ub(255,255,255);
			glBegin(GL_TRIANGLES);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t1]);		
				glVertex3fv(&meshes[i].vexs[v1]);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t2]);
				glVertex3fv(&meshes[i].vexs[v2]);

				glNormal3fv(&meshes[i].normals[l]);l+=3;
				glTexCoord2fv(&meshes[i].texcords[t3]);
				glVertex3fv(&meshes[i].vexs[v3]);
			glEnd();

			/*glBegin(GL_LINES);
				glVertex3fv(&meshes[i].vexs[v1]);
			glEnd();*/

		}
	}


}

void RMDModel::RenderBaseSkeleton()
{
	int i;
	glBindTexture(GL_TEXTURE_2D,0);
	glLineWidth(3.0f);
	glPointSize(5.0f);
	/*glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	for (i=0;i<skeleton->bonecount;i++)
	{
		if (skeleton->bones[i]->parent == 0)
		{
			skeleton->bones[i]->drawbase(0);
		}
	}

	//glDisable(GL_BLEND);
}

void RMDModel::RenderAnimSkeleton()
{
	int i;
	glBindTexture(GL_TEXTURE_2D,0);
	glLineWidth(3.0f);
	glPointSize(5.0f);
	/*glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	for (i=0;i<skeleton->bonecount;i++)
	{
		if (skeleton->bones[i]->parent == 0)
		{
			skeleton->bones[i]->drawchilds(0);
		}
	}

	//glDisable(GL_BLEND);
}