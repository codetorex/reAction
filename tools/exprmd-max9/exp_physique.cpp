#include "stdafx.h"
#include "exporter.h"


Modifier* MyExporter::FindPhysiqueModifier (INode* nodePtr)
{
	Object* ObjectPtr = nodePtr->GetObjectRef();
	if (!ObjectPtr) return NULL;
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);

		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				return ModifierPtr;
			}
			ModStackIndex++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	return NULL;
}


void MyExporter::ExportPhysiqueData(TimeValue t, ObjectState *ost, INode *node,RMDMesh* msh)
{
	if (!ISEXP(EXPORT_PHYSIQUE)) return;
	Modifier *phyMod = FindPhysiqueModifier(node);
	if (!phyMod) return;

	IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYINTERFACE);
	if (phyExport)
	{
		IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

		if (mcExport)
		{
			msh->boneinfo = new RMDWeightedVertex [msh->vertexcount];
			msh->hasphysique = true;
			AddDbg("Exporting physique info of %s",node->GetName());
			mcExport->ConvertToRigid(TRUE);
			mcExport->AllowBlending(true);

			IPhyBlendedRigidVertex *rb_vtx;
			IPhyRigidVertex *r_vtx;
//			IPhyFloatingVertex *f_vtx; currently not implemented?
			int x;

			for (int i = 0; i < ost->obj->NumPoints();  i++)
			{
				int boneid;
				RMDBone* rbone;
				float weight = 0.0f;
				IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(i);

				if (vtxExport)
				{
					int type = vtxExport->GetVertexType();
					switch (type)
					{
						// multi link assigned vertex
						case RIGID_BLENDED_TYPE:
							rb_vtx = (IPhyBlendedRigidVertex*)vtxExport;
							msh->boneinfo[i].PrepareMemory(rb_vtx->GetNumberNodes());
							for ( x = 0;x < rb_vtx->GetNumberNodes();x++ )
							{
								rbone = os.GetBoneFromName(rb_vtx->GetNode(x)->GetName()); // we will convert these to ids when saving
								boneid = rbone->boneid;
								weight = rb_vtx->GetWeight(x);
								msh->boneinfo[i].AddBone(boneid,weight);
							}
						break;

						// one link assigned vertex
						case RIGID_TYPE:
							msh->boneinfo[i].PrepareMemory(1);
							r_vtx = (IPhyRigidVertex*)vtxExport;
							rbone = os.GetBoneFromName(r_vtx->GetNode()->GetName());
							boneid = rbone->boneid;
							msh->boneinfo[i].AddBone(boneid,1.0f);
						break;
					}
					msh->boneinfo[i].Normalize();
					mcExport->ReleaseVertexInterface(vtxExport);
					vtxExport = NULL;
				}
			}
			phyExport->ReleaseContextInterface(mcExport);
		}
		phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	}
}