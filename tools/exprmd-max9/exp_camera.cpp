#include "stdafx.h"
#include "exporter.h"

RMDCamera::RMDCamera()
{
	memset(cameraname,0,64);
}

void MyExporter::ExportCamera(INode * node)
{
	if (!ISEXP(EXPORT_CAMERA)) return;
	AddDbg("Exporting camera: %s",node->GetName());	
	TimeValue currtime = m_ip->GetTime();
	ObjectState os = node->EvalWorldState(currtime);
	if (!os.obj)
		return;
	RMDCamera *cam = new RMDCamera;
	/*	GenCamera* camera = (GenCamera*)os.obj;
	struct CameraState cs;
	camera->EvalLightState(currtime, FOREVER, &cs);
	*/
	sprintf_s(cam->cameraname,"%s",node->GetName());

	Matrix3 nodepivot = node->GetNodeTM(currtime);
	Point3 row;
	row = nodepivot.GetTrans();
	row = Ver2GL(row);
	cam->camera[0] = row.x;
	cam->camera[1] = row.y;
	cam->camera[2] = row.z;

	INode* target = node->GetTarget();
	nodepivot = target->GetNodeTM(currtime);
	row = nodepivot.GetTrans();
	row = Ver2GL(row);
	cam->target[0] = row.x;
	cam->target[1] = row.y;
	cam->target[2] = row.z;

	fileheader.cameracount++;
	oc.add(cam);
}
