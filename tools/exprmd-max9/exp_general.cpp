#include "StdAfx.h"
#include "exporter.h"


RMDDirectory* MyExporter::GetDirectoryByType(RMDChunk chktype)
{
	int i;
	for (i=0;i<od.count;i++)
	{
		RMDDirectory* k = (RMDDirectory*)(od.pArray[i]);
		if (k->chunktype == chktype)
		{
			return k;
		}
	}
	return 0;
}

void MyExporter::StartExporting()
{
	PrepareHeader();

	// export bone data first
	// dont export both same time
	try
	{
		if (ISEXP(EXPORT_PHYSIQUE))
		{
			ExportSkeletalPose();
		}
		else if (ISEXP(EXPORT_SKELETAL))
		{
			ExportSkeletalAnimation();
		}
	}
	catch (...)
	{
		AddDbg("Error: Location: Exporting skeleton info.");
		return;
	}

	try
	{
		EnumerateNodes();
	}
	catch (...)
	{
		AddDbg("Error: Location: Enumurating nodes.");
		return;
	}
	
	
	try
	{
		WriteToFile();
	}
	catch (...)
	{
		AddDbg("Error: Location: Writing to file.");
		return;
	}
	
}

void MyExporter::PrepareHeader()
{
	fileheader.signature = 541347154;
	fileheader.objectcount = 0;
	fileheader.lightcount = 0;
	fileheader.cameracount = 0;
	fileheader.texturecount = 0;
	fileheader.entitycount = 0;
//	fileheader.flags = exportmode;

	SYSTEMTIME systim;
	GetLocalTime(&systim);
	elaptimer = GetTickCount();
	GetString(IDS_LONGDESC);

	AddDbg("-----------------------------------------------");
	AddDbg("%s %i/%i/%i %i:%i",GetString(IDS_LONGDESC),systim.wDay,systim.wMonth,systim.wYear,systim.wHour,systim.wMinute);
	AddDbg("-----------------------------------------------");

	Color ambLight = m_ip->GetAmbient(m_ip->GetTime(), FOREVER);
	fileheader.ambientlight = ambLight.toRGB();
}

void MyExporter::EnumerateNodes()
{
	INode* node;
	int numChildren = m_ip->GetRootNode()->NumberOfChildren();
	for (int idx = 0; idx < numChildren; idx++) 
	{
		if (m_ip->GetCancel())break;
		node = m_ip->GetRootNode()->GetChildNode(idx);
		ObjectState os = node->EvalWorldState(m_ip->GetTime()); 
		if (os.obj) 
		{
			Control *c = node->GetTMController();
			switch(os.obj->SuperClassID()) 
			{
				case GEOMOBJECT_CLASS_ID:
					if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0)) break;
					if (os.obj->ClassID() == SKELOBJ_CLASS_ID) break;
					ExportGeomObject(node);
					break;

				case LIGHT_CLASS_ID:
					ExportLight(node);
					break;

				case CAMERA_CLASS_ID:
					ExportCamera(node);
					break;

				default:
					break;
			}
		}
	}
}


/*bool MyExporter::nodeEnum(INode* node) 
{
	//char tak[100];

	if (!node) return FALSE;
	if (m_exportSelected && node->Selected() == FALSE)return FALSE;
	if (m_ip->GetCancel())return FALSE;

	ObjectState os = node->EvalWorldState(m_ip->GetTime()); 
	if (os.obj) {
		switch(os.obj->SuperClassID()) {
		case GEOMOBJECT_CLASS_ID: 
			//sprintf_s(tak,"%s",node->GetName());
			if (tak[0] == 0x65)
			{
				ExportEntity(node);
			}else{
				ExportGeomObject(node); 
			//}
			break;

		case LIGHT_CLASS_ID:
			//ExportLightObject(node);
			break;

		case CAMERA_CLASS_ID:
			//ExportCamera(node);
			break;

		default:
			break;
		}
	}

	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnum(node->GetChildNode(c)))
			return FALSE;
	}


	return TRUE;
}*/