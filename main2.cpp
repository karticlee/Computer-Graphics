#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp> 

using namespace std;
using namespace glm;

char vertex[10];
char xv[20], yv[20], zv[20];
float angle = 0.01;
GLfloat xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
GLfloat xaxis[35000] = { 0 };
GLfloat yaxis[35000] = { 0 };
GLfloat zaxis[35000] = { 0 };
int points[100000][3] = { 0 };
char xf[20], yf[20], zf[20];
int fc, colormode = 0, flag = 1, cw = 1;
float znear, zfar;
float dist;
GLfloat ex = 0.0, ey = 0.0, ez = 0.0;
GLfloat Ux = 0.0, Uy = 1.0, Uz = 0.0;
GLfloat Lx = 0.0, Ly = 0.0, Lz = -1.0;
vec3 V(0.0, 1.0, 0.0);
vec3 N(0.0, 0.0, 1.0);
vec3 U(1.0, 0.0, 0.0);
mat3 rM;
vec3 p(0.0);
int wid, ht;

void generateRM(vec3 &a, GLfloat angle)
{
	vec3 a1;
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	vec3 vec = normalize(a);
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];
	rM = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
		t*x*y + z*s, t*y*y + c, t*y*z - x*s,
		t*x*z - y*s, t*y*z + x*s, t*z*z + c);
}

void construct_model(char modelname[])
{
	char face[10];
	int i = 0;
	int j = 0;
	FILE *pFile;
	pFile = fopen(modelname, "r");
	char l1[20], l2[20], l3[20], h[10];

	while (fscanf(pFile, "%s", vertex) != EOF && strcmp(vertex, "v") == 0)
	{
		fscanf(pFile, "%s %s %s", l1, l2, l3);
		xaxis[i] = atof(l1);
		yaxis[i] = atof(l2);
		zaxis[i] = atof(l3);
		if (xmin > xaxis[i])
		{
			xmin = xaxis[i];
		}
		if (xmax < xaxis[i])
		{
			xmax = xaxis[i];
		}
		if (ymin > yaxis[i])
		{
			ymin = yaxis[i];
		}
		if (ymax < yaxis[i])
		{
			ymax = yaxis[i];
		}
		if (zmin > zaxis[i])
		{
			zmin = zaxis[i];
		}
		if (zmax < zaxis[i])
		{
			zmax = zaxis[i];
		}
		i++;
	}
	while (strcmp(vertex, "f") == 0)
	{
		fscanf(pFile, "%s %s %s", l1, l2, l3);
		points[j][0] = atoi(l1);
		points[j][1] = atoi(l2);
		points[j][2] = atoi(l3);
		j++;
		if (fscanf(pFile, "%s", vertex) == EOF)
		{
			break;
		}

	}
	fclose(pFile);
	znear = zmin;
	zfar = zmax;
	fc = j;
	dist = zmax - zmin;
	p.z = 2 * (zmax - zmin);
}

GLfloat colors[][3] = { { 1.0, 1.0, 1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0 } };

bool stop = false;

void polygon(int a, int b, int c)
{
	if (flag == 1)
	{
		glBegin(GL_POLYGON);
		glColor3fv(colors[colormode]);
		glVertex3f(xaxis[a], yaxis[a], zaxis[a]);
		glVertex3f(xaxis[b], yaxis[b], zaxis[b]);
		glVertex3f(xaxis[c], yaxis[c], zaxis[c]);
		glEnd();
	}
	if (flag == 2)
	{
		glBegin(GL_POINTS);
		glColor3fv(colors[colormode]);
		glVertex3f(xaxis[a], yaxis[a], zaxis[a]);
		glVertex3f(xaxis[b], yaxis[b], zaxis[b]);
		glVertex3f(xaxis[c], yaxis[c], zaxis[c]);
		glEnd();
	}
	if (flag == 3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3fv(colors[colormode]);
		glVertex3f(xaxis[a], yaxis[a], zaxis[a]);
		glVertex3f(xaxis[b], yaxis[b], zaxis[b]);
		glVertex3f(xaxis[c], yaxis[c], zaxis[c]);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void colorcube()
{
	int p1, p2, p3;

	for (int k = 0; k < fc; k++)
	{
		p1 = points[k][0] - 1;
		p2 = points[k][1] - 1;
		p3 = points[k][2] - 1;
		polygon(p1, p2, p3);

	}
}

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	if (cw == 1)
	{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	if (cw == 2)
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	ex = p.x; ey = p.y; ez = p.z;
	gluLookAt(ex, ey, ez, ex + Lx, ey + Ly, ez + Lz, Ux, Uy, Uz);
	colorcube();
	glFlush();
	glutSwapBuffers();
}

void spinCube()
{
	glutPostRedisplay();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, znear, zfar);
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	wid = w;
	ht = h;

}
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void keyboard(unsigned char key, int x, int y)
{
	vec3 t;
	char modelname[20];
	if (key == 'q' || key == 'Q') exit(0);
	if (key == ' ') stop = !stop;
	if (key == 'r')
		colormode = 1;
	if (key == 'g')
		colormode = 2;
	if (key == 'b')
		colormode = 3;
	if (key == 'p')
		flag = 1;
	if (key == 'o')
		flag = 2;
	if (key == 'i')
		flag = 3;
	if (key == 'c')
		cw = 1;
	if (key == 'f')
		cw = 2;
	if (key == 'm')
	{
		cout << "enter a new model name: ";
		cin >> modelname;
		construct_model(modelname);
	}
	if (key == 'd')
	{
		t.x = 0.5;
		p = p + U * t.x;

	}
	if (key == 'a')
	{
		t.x = -0.5;
		p = p + U * t.x;

	}
	if (key == 'w')
	{
		t.y = 0.5;
		p = p + V * t.y;
	}
	if (key == 's')
	{
		t.y = -0.5;
		p = p + V * t.y;
	}
	if (key == 'x')
	{
		t.z = -0.5;
		p = p + N * t.z;
	}
	if (key == 'z')
	{
		t.z = 0.5;
		p = p + N * t.z;
	}
	if (key == 'l')
	{
		znear += 1.0;
		if (zfar < znear)
			zfar = znear;
		myReshape(wid, ht);
	}
	if (key == 'k')
	{

		znear -= 1.0;
		if (znear < 1.0)
			znear = 1.0;
		myReshape(wid, ht);
	}
	if (key == 'j')
	{
		zfar += 1.0;
		myReshape(wid, ht);
	}
	if (key == 'h')
	{
		zfar -= 1.0;
		if (zfar < znear)
			zfar = znear;
		myReshape(wid, ht);
	}
	if (key == 27)
	{
		p.x = 0.0; p.y = 0.0; p.z = 2 * (zmax - zmin);
		Ux = 0.0; Uy = 1.0; Uz = 0.0;
		Lx = 0.0; Ly = 0.0; Lz = -1.0;
	}
	glutIdleFunc(spinCube);
}

void dotProduct()
{
	float t1, t2, t3;
	U.x = dot(rM[0], U);
	U.y = dot(rM[1], U);
	U.z = dot(rM[2], U);
	//U.x = t1; U.y = t2; U.z = t3;
	V.x = dot(rM[0], V);
	V.y = dot(rM[1], V);
	V.z = dot(rM[2], V);
	//V.x = t1; V.y = t2; V.z = t3;
	N.x = dot(rM[0], N);
	N.y = dot(rM[1], N);
	N.z = dot(rM[2], N);
	//N.x = t1; N.y = t2; N.z = t3;
}
void SpecialKeys(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_LEFT:
		generateRM(U, angle);
		dotProduct();
		Lx = N.x*dist;
		Ly = N.y*dist;
		Lz = N.z*dist;
		break;
	case GLUT_KEY_RIGHT:
		generateRM(U, -angle);
		dotProduct();
		Lx = N.x*dist;
		Ly = N.y*dist;
		Lz = N.z*dist;
		break;
	case GLUT_KEY_UP:
		generateRM(V, angle);
		dotProduct();
		Lx = N.x*dist;
		Ly = N.y*dist;
		Lz = N.z*dist;
		break;
	case GLUT_KEY_DOWN:
		generateRM(V, -angle);
		dotProduct();
		Lx = N.x*dist;
		Ly = N.y*dist;
		Lz = N.z*dist;
		break;
	}
	glutIdleFunc(spinCube);
}

int main(int argc, char **argv)
{
	construct_model("bunny.obj");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	glutCreateWindow("colorcube");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}