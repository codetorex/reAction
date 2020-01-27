
#include "StdAfx.h"
#include "vskybox.h"

/*
	0 = Up
	1 = Down
	2 = Right
	3 = Left
	4 = Front
	5 = Back
*/

void inline DrawSkyBoxQuad(float x0,float y0,float z0,float x1,float y1,float z1)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(x0,y0,z0);

	glTexCoord2f(0.0f,1.0f);
	glVertex3f(x0,y1,z0); 
	
	glTexCoord2f(1.0f,1.0f);	
	glVertex3f(x1,y1,z1);

	glTexCoord2f(1.0,0.0);
	glVertex3f(x1,y0,z1);
	glEnd();
}

void VSkyBox::RenderSkyBox(vec3 *pos)
{
	glColor4f(1.0, 1.0, 1.0,1.0f);
	glPushMatrix();
	glTranslatef(pos->x,pos->y,pos->z); 
	glScalef(size.x,size.y,size.z);

	// RIPPED FROM GLUT 3.7 glut_shapes.c AND CHANGED SLIGHTLY
	unsigned int texIds[6];
	texIds[0] = day[SKYBOX_BACK].glID;
	texIds[1] = day[SKYBOX_DOWN].glID;
	texIds[2] = day[SKYBOX_FRONT].glID;
	texIds[3] = day[SKYBOX_LEFT].glID;
	texIds[4] = day[SKYBOX_RIGHT].glID;
	texIds[5] = day[SKYBOX_UP].glID;

	float seam = 0.004f;

	static const GLint faces[6][4] =
	{
		/*
		{0, 1, 2, 3}, // left
		{3, 2, 6, 7}, // top
		{7, 6, 5, 4}, // right
		{4, 5, 1, 0}, // bottom
		{5, 6, 2, 1}, // back
		{7, 4, 0, 3}  // front
		*/
		{5, 1, 2, 6}, // back
		{5, 4, 0, 1}, // bottom
		{0, 4, 7, 3}, // front
		{4, 5, 6, 7}, // right ( 'left' in crinity's labeling )
		{1, 0, 3, 2}, // left  ( 'right' in crinity's labeling )
		{2, 3, 7, 6}  // top
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;  // min x
	v[4][0] = v[5][0] = v[6][0] = v[7][0] =  1;  // max x
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;  // min y
	v[2][1] = v[3][1] = v[6][1] = v[7][1] =  1;  // max y
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -1;  // min z
	v[1][2] = v[2][2] = v[5][2] = v[6][2] =  1;  // max z

	for (i = 5; i >= 0; i--) 
	{
		glBindTexture(GL_TEXTURE_2D, texIds[i]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0 + seam,1 - seam);  glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1- seam,1 - seam);  glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1-seam,0+seam);  glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0+seam,0+seam);  glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	glPopMatrix();
}

void VSkyBox::RenderSkyBox()
{
	glColor4f(1.0, 1.0, 1.0,1.0f);
	glPushMatrix();
	glTranslatef(cam->xPos,cam->yPos,cam->zPos); 
	glScalef(size.x,size.y,size.z);

	// RIPPED FROM GLUT 3.7 glut_shapes.c AND CHANGED SLIGHTLY
	unsigned int texIds[6];
	texIds[0] = day[SKYBOX_BACK].glID;
	texIds[1] = day[SKYBOX_DOWN].glID;
	texIds[2] = day[SKYBOX_FRONT].glID;
	texIds[3] = day[SKYBOX_LEFT].glID;
	texIds[4] = day[SKYBOX_RIGHT].glID;
	texIds[5] = day[SKYBOX_UP].glID;

	float seam = 0.004f;

	static const GLint faces[6][4] =
	{
		/*
		{0, 1, 2, 3}, // left
		{3, 2, 6, 7}, // top
		{7, 6, 5, 4}, // right
		{4, 5, 1, 0}, // bottom
		{5, 6, 2, 1}, // back
		{7, 4, 0, 3}  // front
		*/
		{5, 1, 2, 6}, // back
		{5, 4, 0, 1}, // bottom
		{0, 4, 7, 3}, // front
		{4, 5, 6, 7}, // right ( 'left' in crinity's labeling )
		{1, 0, 3, 2}, // left  ( 'right' in crinity's labeling )
		{2, 3, 7, 6}  // top
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;  // min x
	v[4][0] = v[5][0] = v[6][0] = v[7][0] =  1;  // max x
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;  // min y
	v[2][1] = v[3][1] = v[6][1] = v[7][1] =  1;  // max y
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -1;  // min z
	v[1][2] = v[2][2] = v[5][2] = v[6][2] =  1;  // max z

	for (i = 5; i >= 0; i--) 
	{
		glBindTexture(GL_TEXTURE_2D, texIds[i]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0 + seam,1 - seam);  glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1- seam,1 - seam);  glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1-seam,0+seam);  glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0+seam,0+seam);  glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	glPopMatrix();
}

/*void VSkyBox::RenderSkyBox()
{
	// Begin DrawSkybox
	glColor4f(1.0, 1.0, 1.0,1.0f);
 
	// Save Current Matrix
	glPushMatrix();
 
	// Second Move the render space to the correct position (Translate)

	glTranslatef(cam->xPos,cam->yPos,cam->zPos);

	glRotatef(180.0f,1.0f,0.0f,0.0f);
 
	// First apply scale matrix
	glScalef(size.x,size.y,size.z);
 
	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f; // If you have border issues change this to 1.005f
	// Common Axis Z - FRONT Side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_FRONT].glID);
	glBegin(GL_QUADS);	
		glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
		glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
		glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r); 
		glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
	glEnd();
 
	// Common Axis Z - BACK side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_BACK].glID);
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
		glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
		glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r); 
		glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
	glEnd();
 
	// Common Axis X - Left side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_LEFT].glID);
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);	
		glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r); 
		glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
		glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);		
	glEnd();
 
	// Common Axis X - Right side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_RIGHT].glID);
	glBegin(GL_QUADS);		
		glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);	
		glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r); 
		glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
		glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
	glEnd();
 
	// Common Axis Y - Draw Up side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_UP].glID);
	glBegin(GL_QUADS);		
		glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
		glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f); 
		glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
	glEnd();
 
	// Common Axis Y - Down side
	glBindTexture(GL_TEXTURE_2D,day[SKYBOX_DOWN].glID);
	glBegin(GL_QUADS);		
		glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
		glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f); 
		glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
	glEnd();
 
	// Load Saved Matrix
	glPopMatrix();
}*/

