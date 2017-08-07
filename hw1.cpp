#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp> 

using namespace std;
using namespace glm;

char vert[10];
int wd, ht;
GLfloat znear = 0.0, zfar = 0.0;
GLfloat xver[35000] = { 0 }, yver[35000] = { 0 }, zver[35000] = { 0 };
int faces[100000][3] = { 0 };
int cnt, cmode = 0, flag = 1;
bool cw = true;
float d;
GLfloat eye_x = 0.0, eye_y = 0.0, eye_z = 0.0;
GLfloat look_x = 0.0, look_y = 0.0, look_z = -1.0;
vec3 U(1.0, 0.0, 0.0), V(0.0, 1.0, 0.0), N(0.0, 0.0, 1.0), pt(0.0);
GLfloat colors[][3] = { { 1.0, 1.0, 1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0 } };


void polygon(int a, int b, int c)
{
	if (flag == 1)
	{
		glBegin(GL_POLYGON);
		glColor3fv(colors[cmode]);
		glVertex3f(xver[a], yver[a], zver[a]);
		glVertex3f(xver[b], yver[b], zver[b]);
		glVertex3f(xver[c], yver[c], zver[c]);
		glEnd();
	}
	if (flag == 2)
	{
		glBegin(GL_POINTS);
		glColor3fv(colors[cmode]);
		glVertex3f(xver[a], yver[a], zver[a]);
		glVertex3f(xver[b], yver[b], zver[b]);
		glVertex3f(xver[c], yver[c], zver[c]);
		glEnd();
	}
	if (flag == 3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3fv(colors[cmode]);
		glVertex3f(xver[a], yver[a], zver[a]);
		glVertex3f(xver[b], yver[b], zver[b]);
		glVertex3f(xver[c], yver[c], zver[c]);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void model(char mname[])
{
	char face[10];
	int i = 0;
	int j = 0;
	FILE *pFile;
	pFile = fopen(mname, "r");
	char xx[20], yy[20], zz[20], h[10];
	while (fscanf(pFile, "%s", vert) != EOF && strcmp(vert, "v") == 0)
	{
		fscanf(pFile, "%s %s %s", xx, yy, zz);
		xver[i] = atof(xx);
		yver[i] = atof(yy);
		zver[i] = atof(zz);

		if (znear > zver[i])
		{
			znear = zver[i];
		}
		if (zfar < zver[i])
		{
			zfar = zver[i];
		}
		i++;
	}
	while (strcmp(vert, "f") == 0)
	{
		fscanf(pFile, "%s %s %s", xx, yy, zz);
		faces[j][0] = atoi(xx);
		faces[j][1] = atoi(yy);
		faces[j][2] = atoi(zz);
		j++;
		if (fscanf(pFile, "%s", vert) == EOF)
		{
			break;
		}

	}
	fclose(pFile);
	cnt = j;
	d = zfar - znear;
	pt.z = 4 * d;
}

void colorcube()
{
	int p1, p2, p3;

	for (int k = 0; k < cnt; k++)
	{
		p1 = faces[k][0] - 1;
		p2 = faces[k][1] - 1;
		p3 = faces[k][2] - 1;
		polygon(p1, p2, p3);
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	if (cw)
	{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	eye_x = pt.x; eye_y = pt.y; eye_z = pt.z;
	gluLookAt(eye_x, eye_y, eye_z, eye_x + look_x, eye_y + look_y, eye_z + look_z, 0.0, 1.0, 0.0);
	colorcube();
	glFlush();
	glutSwapBuffers();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	wd = w;
	ht = h;

}

void keyboard(unsigned char key, int x, int y)
{

	char mname[20];
	if (key == 'q' || key == 'Q') exit(0);
	
	if (key == 'l')
		flag = 1;
	if (key == 'k')
		flag = 2;
	if (key == 'j')
		flag = 3;

	if (key == 'c')
		cw = true;
	if (key == 'v')
		cw = false;

	if (key == 'n')
	{
		cout << "enter a new object file: ";
		cin >> mname;
		model(mname);
	}

	if (key == 'r')
		cmode = 1;
	if (key == 'g')
		cmode = 2;
	if (key == 'b')
		cmode = 3;

	if (key == 'd')
		pt = pt + U;
	if (key == 'a')
		pt = pt - U;
	if (key == 'w')
		pt = pt + V;
	if (key == 's')
		pt = pt - V;
	if (key == 'x')
		pt = pt - N;
	if (key == 'z')
		pt = pt + N;
	if (key == 'p')
	{
		znear += 1.0;
		myReshape(wd, ht);
	}
	if (key == 'o')
	{

		znear -= 1.0;
		myReshape(wd, ht);
	}
	if (key == 'i')
	{
		zfar += 1.0;
		myReshape(wd, ht);
	}
	if (key == 'u')
	{
		zfar -= 1.0;
		myReshape(wd, ht);
	}
	if (key == 'f')
	{
		pt.x = 0.0; pt.y = 0.0; pt.z = 4 * d;
		look_x = 0.0; look_y = 0.0; look_z = -1.0;
	}
	glutIdleFunc(display);
}

int main(int argc, char **argv)
{
	model("bunny.obj");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	glutCreateWindow("colorcube");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}