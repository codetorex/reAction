#include "stdafx.h"
#include "exporter.h"
#include "decomp.h"

RMDBone::RMDBone()
{
	memset(bonename,0,128);
	memset(boneparent,0,128);
}

void MyExporter::ExportSkeletalPose()
{
	AddDbg("Exporting skeletal pose...");
	os.baseskeleton = true;
	os.bonecount = 0;
	//GetBoneCount();
	os.keystart = 0;
	os.keyend = 0;
	os.framerate = 0;

	int numChildren = m_ip->GetRootNode()->NumberOfChildren();
	skeletonexportmode = 0; // export pose
	for (int idx = 0; idx < numChildren; idx++) {
		if (m_ip->GetCancel())break;
		skeletalEnum(m_ip->GetRootNode()->GetChildNode(idx));
	}

	AddDbg("Nodes processed %i / %i",os.bonecount,os.bones.count);
	AddDbg("Fixing node-parent connections...");
	os.FixBoneConnections();
	AddDbg("Node-parent connections fixed.");
}

void MyExporter::ExportSkeletalAnimation()
{
	os.baseskeleton = false;
	os.bonecount = 0;
	//GetBoneCount();
	os.keystart = (m_ip->GetAnimRange().Start())/GetTicksPerFrame();
	os.keyend = (m_ip->GetAnimRange().End())/GetTicksPerFrame();
	os.framerate = GetFrameRate();

	int numChildren = m_ip->GetRootNode()->NumberOfChildren();
	skeletonexportmode = 1; // export animation
	for (int idx = 0; idx < numChildren; idx++) {
		if (m_ip->GetCancel())break;
		skeletalEnum(m_ip->GetRootNode()->GetChildNode(idx));
	}

	AddDbg("Fixing node-parent connections...");
	os.FixBoneConnections();
	AddDbg("Node-parent connections fixed.");
}

void RMDSkeleton::FixBoneConnections()
{
	int i;
	RMDBone* rbone;
	for (i=0;i<bonecount;i++)
	{
		rbone = (RMDBone*)(bones.pArray[i]);
		rbone->parent = GetBoneFromName(rbone->boneparent);
		//AddDbg2("Fixed: %s -> %s",rbone->bonename,rbone->boneparent);
	}
}

RMDBone* RMDSkeleton::GetBoneFromName(char *bonename)
{
	int k;
	RMDBone* rbone;
	for (k=0;k<bonecount;k++)
	{
		rbone = (RMDBone*)(bones.pArray[k]);
		//AddDbg2("Looking for: %i",(int)rbone);
		//AddDbg2("Looking for: %s",rbone->bonename);
		if (strcmp(rbone->bonename,bonename) == 0)
		{
			//AddDbg2("Match!");
			return rbone;
			break;
		}
	}
	//AddDbg2("No matches found for %s",bonename);
	return 0;
}

/*RMDBone* RMDSkeleton::GetBoneFromNameHash(long nhash)
{
	
}*/

bool MyExporter::skeletalEnum(INode* node) 
{
	if (!node) return FALSE;
	if (m_exportSelected && node->Selected() == FALSE)return FALSE;
	if (m_ip->GetCancel())return FALSE;

	ObjectState os = node->EvalWorldState(m_ip->GetTime()); 
	if (os.obj) {
		Control *c = node->GetTMController();
		if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
			(c->ClassID() == BIPBODY_CONTROL_CLASS_ID))
			//(c->ClassID() == FOOTPRINT_CLASS_ID))
		{
			//sprintf_s(dbuffer,"Exporting... %s id=%i class=%i",node->GetName(),GenID(node->GetName()),c->ClassID());AddDbg();
			ExportBone(node,c);
		}
	}

	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!skeletalEnum(node->GetChildNode(c)))
			return FALSE;
	}


	return TRUE;
}

void MyExporter::GetBoneCount()
{
	int numChildren = m_ip->GetRootNode()->NumberOfChildren();
	for (int idx = 0; idx < numChildren; idx++) {
		if (m_ip->GetCancel())break;
		BoneCounter(m_ip->GetRootNode()->GetChildNode(idx));
	}

}

void MyExporter::MatrixToBoneKeyframe(INode* node,RMDBoneKeyFrame* kfm,TimeValue t)
{
	INode* parentnode;
	parentnode = node->GetParentNode();

	Matrix3 ma,pma,ltma; // MA object matrix PMA parent's matrix LTMA local matrix
	Matrix3 lm;

	ma = node->GetNodeTM(t);
	pma = parentnode->GetNodeTM(t);
	ma.NoScale();
	pma.NoScale();
	lm = ma * Inverse(pma); // local matrix
	lm = Mat2GL(lm);

	Point3 pos3d = lm.GetTrans();

	AffineParts affparts;
	float rgflXYZRotations[3];

	decomp_affine(lm,&affparts);
	QuatToEuler(affparts.q,rgflXYZRotations);

	// _p[0],_p[2],- _p[1]       3DSmax to OpenGL conversion
	// x , z , -y
	kfm->pos[0] = pos3d.x;
	kfm->pos[1] = pos3d.y;
	kfm->pos[2] = pos3d.z;

	kfm->rot[0] = ReduceRotation(rgflXYZRotations[0]);
	kfm->rot[1] = ReduceRotation(rgflXYZRotations[1]);
	kfm->rot[2] = ReduceRotation(rgflXYZRotations[2]);

	kfm->time = t / GetTicksPerFrame();

}

void MyExporter::MatrixToFloat(INode* node,RMDBoneKeyFrame* kfm,TimeValue t)
{
	INode* parentnode;
	parentnode = node->GetParentNode();

	Matrix3 ma,pma,ltma; // MA object matrix PMA parent's matrix LTMA local matrix
	Matrix3 lm;

	ma = node->GetNodeTM(t);
	pma = parentnode->GetNodeTM(t);

	ma.NoScale();
	pma.NoScale();
	lm = ma * Inverse(pma); // local matrix

	lm = Mat2GL(lm);
	MxToM(lm,kfm->mat);
}

void MyExporter::ExportBone(INode* node,Control* c)
{
	AddDbg("Exporting bone: %s",node->GetName());
	Matrix3 ma,pma,ltma; // MA object matrix PMA parent's matrix LTMA local matrix
	Matrix3 lm;
	lm.Zero();
	RMDBone* bosbone;

	if (skeletonexportmode == 0) // just export initial local matrix
	{
		int kfc=0;

		// burada biseyler yapilacakti galiba unuttum aq
		INode* parentnode;
		parentnode = node->GetParentNode();

		bosbone = new RMDBone;
		bosbone->framec = 0;
		bosbone->sklt = &os;
		sprintf_s(bosbone->bonename,"%s\0",node->GetName());
		sprintf_s(bosbone->boneparent,"%s\0",parentnode->GetName());

		bosbone->boneid = os.bonecount;
		bosbone->bonenamehash = shash(bosbone->bonename);
		bosbone->parent = (RMDBone*)shash(node->GetParentNode()->GetName());
		// Warning! bu islem tamamlaninca parentlere, isimlere bakilip boneler birbirine linklenmeli

		//MatrixToBoneKeyframe(node,&bosbone->m_init,m_ip->GetAnimRange().Start());
		ma = node->GetNodeTM(m_ip->GetAnimRange().Start());
		ma = Mat2GL(ma);
		MxToM(ma,bosbone->m_init);



		//AddDbg("Bone added to list: %s  %i",bosbone->bonename,bosbone);
		os.bones.add((void*)bosbone);
		os.bonecount++;
		
	}
	else // export animation
 	{
		int kfc=0;

		INode* parentnode;
		parentnode = node->GetParentNode();

		bosbone = new RMDBone;
		bosbone->framec = os.keyend - os.keystart;
		bosbone->sklt = &os;
		sprintf_s(bosbone->bonename,"%s\0",node->GetName());
		sprintf_s(bosbone->boneparent,"%s\0",parentnode->GetName());

		bosbone->boneid = os.bonecount;
		bosbone->bonenamehash = shash(bosbone->bonename);
		bosbone->parent = (RMDBone*)shash(node->GetParentNode()->GetName());
		// Warning! bu islem tamamlaninca parentlere, isimlere bakilip boneler birbirine linklenmeli
		bosbone->kframes = new RMDBoneKeyFrame [bosbone->framec];

		TimeValue t;
		TimeValue ts,te;
		int tpf;
		tpf = GetTicksPerFrame();
		ts = m_ip->GetAnimRange().Start();
		te = m_ip->GetAnimRange().End();
		kfc = 0;

		for (t=ts; t < te ;t+=tpf)
		{
			MatrixToBoneKeyframe(node,&bosbone->kframes[kfc],t);
			MatrixToFloat(node,&bosbone->kframes[kfc],t);
			kfc++;
		}

		os.bones.add((void*)bosbone);
		os.bonecount++;

	}
		/*for (i=os.keystart;i<os.keyend;i++)
		{
			ma = node->GetNodeTM(i*GetTicksPerFrame());
			if (ma == lm)
			continue;
			lm = ma;
			kfc++;
		}*/
/*	Matrix3 ma,pma,ltma;
	// MA object matrix PMA parent's matrix LTMA local matrix
	Matrix3 lm;
	//int delta = bh.ticksperframe;
	int kfc=0;
	lm.Zero();
	//	lm.Identity();
	int i;	
	for (i=bh.keystart;i<bh.keyend;i++)
	{
		ma = node->GetNodeTM(i*GetTicksPerFrame());
		if (ma == lm)
			continue;
		lm = ma;
		//sprintf_s(dbuffer,"Bone %s changed in %i.",node->GetName(),i);AddDbg();
		kfc++;
	}
	bcon[curb].framec = kfc;
	bcon[curb].frames = new IMBKeyframe [kfc];
	//sprintf_s(dbuffer,"Bone %s id=%i has %i frames.",node->GetName(),GenID(node->GetName()),kfc);AddDbg();

	kfc = 0;

	for (i=bh.keystart;i<bh.keyend;i++)
	{
		ma = node->GetNodeTM(i*GetTicksPerFrame());
		//ma.NoScale();
		//ma = Mat2GL(ma);


		if (ma == lm)
			continue;


		bcon[curb].frames[kfc].frameno = i;
		//		INode *npnode = node->GetParentNode();
		ma = node->GetNodeTM(i*GetTicksPerFrame());
		//		pma = npnode->GetNodeTM(i*GetTicksPerFrame());
		lm = ma;
		ma.NoScale();		// scale sicirtir
		//		pma.NoScale();		// scale sicirtir

		//		ltma	= ma * Inverse(pma);
		//		Point3 trans = ltma.GetTrans();
		Point3 trans = ma.GetTrans();

		AffineParts affparts;
		float rgflXYZRotations[3];

		//		decomp_affine(ltma, &affparts);
		decomp_affine(ma, &affparts);
		QuatToEuler(affparts.q, rgflXYZRotations);

		float xRo = rgflXYZRotations[0];		// in radians
		float yRo = rgflXYZRotations[1];		// in radians
		float zRo = rgflXYZRotations[2];		// in radians

		xRo = ReduceRotation(xRo);
		yRo = ReduceRotation(yRo);
		zRo = ReduceRotation(zRo);
		bcon[curb].frames[kfc].xPos = trans.x;
		bcon[curb].frames[kfc].yPos = trans.y;
		bcon[curb].frames[kfc].zPos = trans.z;

		bcon[curb].frames[kfc].xRot = xRo;
		bcon[curb].frames[kfc].yRot = yRo;
		bcon[curb].frames[kfc].zRot = zRo;
		//MxToM(ma,bcon[curb].frames[kfc].m);

		kfc++;
	}
	bcon[curb].boneid = GenID(node->GetName());
	//	bcon[curb].parentid = 0;
	//	bcon[curb].childid = 0;

	//sprintf_s(dbuffer,"Bone %s id=%i exported.",node->GetName(),GenID(node->GetName()) );AddDbg();

	curb++;*/

}

float MyExporter::ReduceRotation(float fl)
{
	while (fl >= TWOPI)
		fl -= TWOPI;
	while (fl <= -TWOPI)
		fl += TWOPI;
	return fl;
}

void MyExporter::BoneCounter(INode* node)
{
	if (node)
	{

		ObjectState osp = node->EvalWorldState(m_ip->GetTime()); 
		if (osp.obj) {
			Control *c = node->GetTMController();
			if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
				(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ) // ||
				//(c->ClassID() == FOOTPRINT_CLASS_ID))
			{
				os.bonecount++;
			}
		}

		for (int c = 0; c < node->NumberOfChildren(); c++) {
			BoneCounter(node->GetChildNode(c));
		}
	}
}

// weighted vertex class functions

RMDWeightedVertex::RMDWeightedVertex()
{
	ibc = 0;
	weights = 0;
	boneids = 0;
	curbone = 0;
}

void RMDWeightedVertex::PrepareMemory(int ibonecount)
{
	curbone = 0;
	ibc = ibonecount;
	boneids = new int [ibc];
	weights = new float [ibc];
}

void RMDWeightedVertex::AddBone(int boneid,float weight)
{
	boneids[curbone] = boneid;
	weights[curbone] = weight;
	curbone++;
}

void RMDWeightedVertex::Normalize()
{
	float totalweight = 0.0f;
	int i;
	for (i=0;i<ibc;i++)
	{
		totalweight += weights[i];
	}

	for (i=0;i<ibc;i++)
	{
		weights[i] /= totalweight;
	}

}