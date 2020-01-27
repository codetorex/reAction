#include "stdafx.h"
#include "exporter.h"

RMDLight::RMDLight()
{
	memset(lightname,0,64);
}

void MyExporter::ExportLight(INode * node)
{
	if (!ISEXP(EXPORT_CAMERA)) return;

	TimeValue currtime = m_ip->GetTime();
	ObjectState os = node->EvalWorldState(currtime);
	if (!os.obj)
		return;

	GenLight* light = (GenLight*)os.obj;
	struct LightState ls;
	light->EvalLightState(currtime, FOREVER, &ls);

	if (!light->GetUseLight())
		return; // only export lights that are on for simplicity

	RMDLight * lig = new RMDLight;
	//::fprintf(m_fileStream, "BEGIN LIGHT Name:<%s> Type:<",node->GetName());
	AddDbg("Exporting light: %s",node->GetName());
	// light type
	switch(light->Type()) 
	{
		case OMNI_LIGHT:
			lig->lighttype = OMNI_LIGHT;
			break;
		case TSPOT_LIGHT: 
			lig->lighttype = TSPOT_LIGHT; // target spot
			break;
		case DIR_LIGHT:   
			lig->lighttype = DIR_LIGHT; // directional
			break;
		case FSPOT_LIGHT:
			lig->lighttype = FSPOT_LIGHT; // free spot
			break;
		case TDIR_LIGHT:
			lig->lighttype = TDIR_LIGHT;
			break;
		default:
			lig->lighttype = OMNI_LIGHT;
			break;
	}

	///////////////////////////////////
	// color, intensity info
	Point3 lcolor;
	lcolor = light->GetRGBColor(currtime);
	lig->clr[0] = (lcolor.x*255);
	lig->clr[1] = (lcolor.y*255);
	lig->clr[2] = (lcolor.z*255);
	lig->intensity = ls.intens;

	///////////////////////////////////
	// get and dump the node matrix, mainly for xform and rot info
	// also dump target TM, if the light has a target
	Matrix3 nodepivot = node->GetNodeTM(currtime);
	Point3 cord = Ver2GL(nodepivot.GetTrans());

	lig->lightpos[0] = cord.x;
	lig->lightpos[1] = cord.y;
	lig->lightpos[2] = cord.z;

	//nodepivot = Mat2GL(nodepivot);

	//MxToM(nodepivot, lig->lmatrix);

	INode* target = node->GetTarget();
	if (target) {
		nodepivot = target->GetNodeTM(currtime);
		cord = Ver2GL(nodepivot.GetTrans());
		//nodepivot = Mat2GL(nodepivot);
		//MxToM(nodepivot,lig->tmatrix);
		lig->targetpos[0] = cord.x;
		lig->targetpos[1] = cord.y;
		lig->targetpos[2] = cord.z;
	}

	sprintf_s(lig->lightname,"%s",node->GetName());

	lig->atten[0] = light->GetAtten(currtime,ATTEN1_START); // Near Start
	lig->atten[1] = light->GetAtten(currtime,ATTEN1_END); // Near End
	lig->atten[2] = light->GetAtten(currtime,ATTEN_START); // Far Start
	lig->atten[3] = light->GetAtten(currtime,ATTEN_END); // Far End
	ol.add((void*)lig);
	fileheader.lightcount++;

}
