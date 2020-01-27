#include "StdAfx.h"
#include "exporter.h"

RMDMesh::RMDMesh()
{
	memset(objname,0,64);
}

void MyExporter::ExportGeomObject(INode * node)
{
	if (!ISEXP(EXPORT_MESH)) return;
//	fileheader.objectcount++;
	int i,k; // for loops
	Point3 vert;

	TimeValue currtime = m_ip->GetTime();
	ObjectState os = node->EvalWorldState(currtime);



	Matrix3 tm = node->GetObjTMAfterWSM(currtime);
	bool delMesh = false;
	Object *obj = os.obj;
	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{ 
		TriObject * tri = NULL;
		tri = (TriObject *) obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) 
			delMesh = true; // we own the copy
		if (tri) 
		{
			Mesh * mesh = &(tri->GetMesh());
			assert(mesh);
			mesh->buildNormals();
			AddDbg("Exporting mesh: %s",node->GetName());

			RMDMesh	*emesh = new RMDMesh;

			emesh->vertexcount = mesh->getNumVerts();
			emesh->facecount = mesh->getNumFaces();

			emesh->vexs = new float [emesh->vertexcount * 3];
			emesh->faces = new int [emesh->facecount << 2 ]; // * 4

			float centerx,centery,centerz;
			centerx = centery = centerz = 0;

			// Export the vertices
			for (i = 0,k=0; i < emesh->vertexcount; i++,k+=3) 
			{
				vert = tm * mesh->verts[i];
				vert = Ver2GL(vert);

				emesh->vexs[k  ] = vert.x;
				emesh->vexs[k+1] = vert.y;
				emesh->vexs[k+2] = vert.z;

				centerx += vert.x;
				centery += vert.y;
				centerz += vert.z;

				if (vert.x > emesh->boundingbox[0]) emesh->boundingbox[0] = vert.x;
				if (vert.x < emesh->boundingbox[3]) emesh->boundingbox[3] = vert.x;

				if (vert.y > emesh->boundingbox[1]) emesh->boundingbox[1] = vert.y;
				if (vert.y < emesh->boundingbox[4]) emesh->boundingbox[4] = vert.y;

				if (vert.z > emesh->boundingbox[2]) emesh->boundingbox[2] = vert.z;
				if (vert.z < emesh->boundingbox[5]) emesh->boundingbox[5] = vert.z;

			}

			emesh->center[0] = centerx / emesh->vertexcount;
			emesh->center[1] = centery / emesh->vertexcount;
			emesh->center[2] = centerz / emesh->vertexcount;

			// export faces
			for (i = 0,k=0; i < emesh->facecount; i++,k+=4) 
			{
				emesh->faces[k  ] = mesh->faces[i].v[0];
				emesh->faces[k+1] = mesh->faces[i].v[1];
				emesh->faces[k+2] = mesh->faces[i].v[2];
				emesh->faces[k+3] = (int)mesh->faces[i].getMatID();
			}

			// export texture cordinates
			emesh->tvcount = mesh->getNumTVerts();
			emesh->texcords = new float [emesh->tvcount<<1]; // * 2
			emesh->tfaces = new int [emesh->facecount*3];

			if (emesh->tvcount) 
			{
				for (i = 0,k=0; i < emesh->tvcount; i++,k+=2) 
				{
					UVVert tvert = mesh->tVerts[i];
					emesh->texcords[k  ] = tvert.x;
					emesh->texcords[k+1] = 1-tvert.y;
				}

				for (i = 0,k=0; i < emesh->facecount; i++,k+=3)
				{
					TVFace tface = mesh->tvFace[i];
					emesh->tfaces[k  ] = tface.t[0];
					emesh->tfaces[k+1] = tface.t[1];
					emesh->tfaces[k+2] = tface.t[2];
				}                    
			}


			Point3 fn;  // Face normal
			Point3 vn;  // Vertex normal
			int  vert;
			Face* f;
			
			emesh->normals = new float [emesh->facecount*9];

			// Face and vertex normals.
			// In MAX a vertex can have more than one normal (but doesn't always have it).
			// This is depending on the face you are accessing the vertex through.
			// To get all information we need to export all three vertex normals
			// for every face.
			for (i=0,k=0; i<emesh->facecount; i++,k+=9) 
			{
				f = &mesh->faces[i];

				vert = f->getVert(0);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
				vn = Ver2GL(vn);
				emesh->normals[k  ] = vn.x;
				emesh->normals[k+1] = vn.y;
				emesh->normals[k+2] = vn.z;


				vert = f->getVert(1);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
				vn = Ver2GL(vn);
				emesh->normals[k+3] = vn.x;
				emesh->normals[k+4] = vn.y;
				emesh->normals[k+5] = vn.z;

				vert = f->getVert(2);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
				vn = Ver2GL(vn);
				emesh->normals[k+6] = vn.x;
				emesh->normals[k+7] = vn.y;
				emesh->normals[k+8] = vn.z;

			}

			/*oc[curo].cr = new unsigned char [mesh->getNumFaces() * 3];
			oc[curo].cg = new unsigned char [mesh->getNumFaces() * 3];
			oc[curo].cb = new unsigned char [mesh->getNumFaces() * 3];
			for (i=0;i<mesh->getNumFaces() * 3;i++)
			{
				oc[curo].cr[i] = fh.alr;
				oc[curo].cg[i] = fh.alg;
				oc[curo].cb[i] = fh.alb;
			}*/

			emesh->normalcount = 3 * emesh->facecount;
			//oc[curo].sfcount = 0;
			sprintf_s(emesh->objname,"%s\0",node->GetName());
			if (delMesh)delete tri;

			emesh->hasphysique=false;
			ExportPhysiqueData(currtime,&os,node,emesh);
			if (ISEXP(EXPORT_MATERIALS))
			{
				Mtl *m = node->GetMtl();
				if (m)
				{
					emesh->matid = ExportMaterial(m);
				}
				else
				{
					// nomaterial assigned
					emesh->matid = 0xFFFFFFFF; // a holly shit number
				}
			}
			om.add((void*)emesh);
			fileheader.objectcount++;
		}
	}

}

Point3 MyExporter::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	if (rv->rFlags & SPECIFIED_NORMAL) 
	{
		vertexNormal = rv->rn.getNormal();
	}
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) 
	{
		if (numNormals == 1) 
		{
			vertexNormal = rv->rn.getNormal();
		}
		else 
		{
			for (int i = 0; i < numNormals; i++) 
			{
				if (rv->ern[i].getSmGroup() & smGroup) 
				{
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else 
	{
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	return vertexNormal;
}


