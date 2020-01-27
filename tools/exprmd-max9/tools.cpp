#include "stdafx.h"
#include "exporter.h"


void MyExporter::MxToM(Matrix3 mat,float m[12])
{
		Point3 row;
		row = mat.GetRow(0);
		m[0] = row.x;m[1] = row.y;m[2] = row.z;
		row = mat.GetRow(1);
		m[3] = row.x;m[4] = row.y;m[5] = row.z;
		row = mat.GetRow(2);
		m[6] = row.x;m[7] = row.y;m[8] = row.z;
		row = mat.GetRow(3);
		m[9] = row.x;m[10] = row.y;m[11] = row.z;
}

void AddDbg2(char* fmt,...)
{
	char text[512];
	va_list ap;
	if (fmt == NULL) return;
	va_start(ap,fmt);
	vsprintf_s(text,fmt,ap);
	va_end(ap);

	if (DEBUGMODE == 2 ||DEBUGMODE == 3)
	{
		FILE *logtxt = 0;
		fopen_s(&logtxt,"c://rmdexport.log","a+");
		fprintf(logtxt,"%s\n",text);
		fclose(logtxt);
	}
}

void MyExporter::AddDbg(char* fmt,...)
{
	char text[512];
	va_list ap;
	if (fmt == NULL) return;
	va_start(ap,fmt);
	vsprintf_s(text,fmt,ap);
	va_end(ap);

	if (DEBUGMODE == 1 || DEBUGMODE == 3)
	{
		SendDlgItemMessage(*DebugDlgs,IDC_DBGLIST,LB_ADDSTRING,0,(LPARAM)text);
	}
	if (DEBUGMODE == 2 ||DEBUGMODE == 3)
	{
		FILE *logtxt = 0;
		fopen_s(&logtxt,"c://rmdexport.log","a+");
		fprintf(logtxt,"%s\n",text);
		fclose(logtxt);
	}
}

Matrix3 MyExporter::Mat2GL(const Matrix3& _mat)
{
  //return _mat;

  //T = R x _mat x R-1
  Matrix3 tm,tmogl,tmoglInv;
  tmogl.IdentityMatrix();
  tmogl.RotateX(PI*0.5f);
  tmoglInv=tmogl;
  tmoglInv.Invert();
  tm=tmogl*_mat*tmoglInv;
  return tm;
}


Point3 MyExporter::Ver2GL(const Point3& _p)
{
  //return _p;
  return Point3(_p[0],_p[2],-_p[1]); 

} 

float MyExporter::dotp( const Point3 &v1,  const Point3 &v2 )
{
	return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z  );
}

Point3 MyExporter::crossp( const Point3 &v1,  const Point3 &v2 )
{
	Point3 vCrossProduct;

	vCrossProduct.x =  v1.y * v2.z - v1.z * v2.y;
	vCrossProduct.y = -v1.x * v2.z + v1.z * v2.x;
	vCrossProduct.z =  v1.x * v2.y - v1.y * v2.x;

	return vCrossProduct;
}

long MyExporter::shash(char *key)
{
	long hash = 0;
	int key_len = strlen(key);
	int i;

	for (i = 0; i < key_len; i++) 
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}