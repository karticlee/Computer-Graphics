#include <GL/freeglut.h>
#include<math.h>
#include <stdio.h>
#include<iostream>
#include <glm/glm.hpp> 

#define MX 35000
#define PV 100000

using namespace glm;
using namespace std;

int width, heigth;
bool stop = false;
int mode = 1;
float angle = 0.01;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;
int cl = 3,rot=1,oren=2;
GLfloat nearVal = 0.0, farVal = 1.0;
GLfloat xRot = 0.0, yRot = 0.0;
GLfloat color[][3] = { { -1.0, -1.0, -1.0 },{ 1.0, -1.0, -1.0 },{ 1.0, 1.0, -1.0 },{ -1.0, 1.0, -1.0 },{ -1.0, -1.0, 1.0 },{ 1.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ -1.0, 1.0, 1.0 } };
GLfloat color1[][3] = { {1.0, 0.0, 0.0},{0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0} };

GLfloat Cx = 0.0, Cy = 0.0, Cz = 0.0;
GLfloat ex = 0.0, ey = 0.0, ez = 0.0;
GLfloat Ux = 0.0, Uy = 1.0, Uz = 0.0;
GLfloat Tx = 0.0, Ty = 0.0, Tz = -1.0;
GLfloat Lx = 0.0, Ly = 0.0, Lz = 0.0;
GLfloat xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
GLfloat Sx = 1.0, Sy = 1.0, Sz = 1.0;
GLfloat x[MX] = { 0 }, y[MX] = { 0 }, z[MX] = { 0 };
GLfloat dist;

GLint p[PV][3] = { 0 };
int fCount = 0;

vec3 V(0.0, 1.0, 0.0);
vec3 N(0.0, 0.0, 1.0);
vec3 U(1.0, 0.0, 0.0);

//GLfloat m[4][4] = {0.0};
mat3 m;
vec3 t(0.0);
mat3 rotMat;
vec3 pos(0.0);

void clearModel()
{
	memset(x, 0, MX*sizeof(GLfloat));
	memset(y, 0, MX*sizeof(GLfloat));
	memset(z, 0, MX*sizeof(GLfloat));
	memset(p, 0, PV*3*sizeof(GLint));
}



void loadModel(char* fname)
{
	FILE  *file;
	int j = 0;
	char l1[20], l2[20], l3[20], h[10];
	file = fopen(fname, "r");
	int c = 0;
	int i = 0;

	while (fscanf(file, "%s", h) != EOF && strcmp(h, "v") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		x[c] = atof(l1);
		y[c] = atof(l2);
		z[c] = atof(l3);
		
		if (xmin > x[c])
		{
			xmin = x[c];
		}
		if (xmax < x[c])
		{
			xmax = x[c];
		}
		if (ymin > y[c])
		{
			ymin = y[c];
		}
		if (ymax < y[c])
		{
			ymax = y[c];
		}
		if (zmin > z[c])
		{
			zmin = z[c];
		}
		if (zmax < z[c])
		{
			zmax = z[c];
		}
		c++;
	}
	while (strcmp(h, "f") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		p[j][0] = atoi(l1);
		p[j][1] = atoi(l2);
		p[j][2] = atoi(l3);
		j++;
		if (fscanf(file, "%s", h) == EOF)
		{
			break;
		}
		
	}
	fCount = j;
	dist = zmax - zmin;
	nearVal = zmin;
	farVal = zmax;
	pos.z = 2 * (zmax-zmin);
	Tz = -1.0;
}
void draw(int i)
{
	glColor3fv(color1[cl]);
	glVertex3d((x[p[i][0] - 1]), (y[p[i][0] - 1]), (z[p[i][0] - 1]));
	glVertex3d((x[p[i][1] - 1]), (y[p[i][1] - 1]), (z[p[i][1] - 1]));
	glVertex3d((x[p[i][2] - 1]), (y[p[i][2] - 1]), (z[p[i][2] - 1]));
}


void triangle()
{
	//int cl;
	for (int i = 0; i < fCount; i++)
	{
		switch (mode)
		{
		case 1:
			glBegin(GL_POLYGON);
			draw(i);
			glEnd();
			break;
		case 2:
			glBegin(GL_POINTS);
			draw(i);
			glEnd();
			break;
		case 3:
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
			draw(i);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
			break;
		}

		//cl = i % 8;
		//cl = 2;
	}
}
void display(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	if (rot == 1)
	{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	if (rot == 2)
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	Cx = pos.x; Cy = pos.y; Cz = pos.z;
	gluLookAt(Cx, Cy, Cz, Cx+Tx, Cy+Ty, Cz+Tz, Ux, Uy, Uz);
	//glScalef(Sx, Sy, Sz);
	triangle();
	glFlush();
	glutSwapBuffers();
}

void drawmesh()
{
	/*theta[axis] += 2.0;
	if (theta[axis] > 360, 0) theta[axis] -= 360;*/
	glutPostRedisplay();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, nearVal, farVal);
	/*gluOrtho2D(-(xmax * 2), (xmax * 2), -(ymax * 2), (ymax * 2));
	gluPerspective(0.65, 1, 0, 20);*/
	//glFrustum(-(xmax * 2), (xmax * 2), -(ymax * 2), (ymax * 2), (zmin * 15), (zmax * 15));
	//glOrtho(-(xmax * 2), (xmax * 2), -(ymax * 2), (ymax * 2), (nearVal*3), (farVal*3));
	/*if (w <= h)
	{
	glOrtho(-(xmax * 2), (xmax * 2), -(ymax * 2) * (GLfloat)h / (GLfloat)w,
	(ymax * 2) * (GLfloat)h / (GLfloat)w, (zmin * 15), (zmax * 15));
	}
	else
	{
	glOrtho(-(xmax * 2) * (GLfloat)w / (GLfloat)h,
	(xmax * 2) * (GLfloat)w / (GLfloat)h, -(ymax * 2), (ymax * 2), (zmin * 15), (zmax * 15));
	}*/
	glMatrixMode(GL_MODELVIEW);
	width = w;
	heigth = h;

}
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void setCameraAxis(vec3 &a, GLfloat angle)
{
	vec3 a1;
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	vec3 vec = normalize(a);
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];
	cout << " x : " << x << endl;
	cout << " y : " << y << endl;
	cout << " z : " << z << endl;
	rotMat = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
		t*x*y + z*s, t*y*y + c, t*y*z - x*s,
		t*x*z - y*s, t*y*z + x*s, t*z*z + c);
}

void dotp()
{
	float t1, t2, t3;
	U.x = dot(rotMat[0], U);
	U.y = dot(rotMat[1], U);
	U.z = dot(rotMat[2], U);
	//U.x = t1; U.y = t2; U.z = t3;
	V.x = dot(rotMat[0], V);
	V.y = dot(rotMat[1], V);
	V.z = dot(rotMat[2], V);
	//V.x = t1; V.y = t2; V.z = t3;
	N.x = dot(rotMat[0], N);
	N.y = dot(rotMat[1], N);
	N.z = dot(rotMat[2], N);
	//N.x = t1; N.y = t2; N.z = t3;
}

void keyboard(unsigned char key, int x, int y)
{
	char fname[30];
	bool pause = false;
	if (key == 'd')
	{
		t.x = 0.2;
		pos = pos + U * t.x;
	
	}
	if (key == 'a')
	{
		t.x = -0.2;
		pos = pos + U * t.x;

	}
	if (key == 'w')
	{
		t.y = 0.2;
		pos = pos + V * t.y;
	}
	if (key == 's')
	{
		t.y = -0.2;
		pos = pos + V * t.y;
	}
	if (key == 'x')
	{
		t.z = -0.2;
		pos = pos + N * t.z;
	}
	if (key == 'z')
	{
		t.z = 0.2;
		pos = pos + N * t.z;
	}

	if (key == 'q' || key == 'Q') exit(0);
	if (key == 'c')
		rot = 1;
	if (key == 'v')
		rot = 2;
	if (key == 'p')
	{
		nearVal += 1.0;
		if (farVal < nearVal)
			farVal = nearVal;
		myReshape(width,heigth);
	}
	if (key == 'o')
	{
	
		nearVal -= 1.0;
		if (nearVal < 1.0)
			nearVal = 1.0;
		myReshape(width, heigth);
	}
	if (key == 'i')
	{
		farVal += 1.0;
		myReshape(width, heigth);
	}
	if (key == 'u')
	{
		farVal -= 1.0;
		if (farVal < nearVal)
			farVal = nearVal;
		myReshape(width, heigth);
	}
		if (key == 'n')
	{
		cout << "New File name : ";
		cin >> fname;
		clearModel();
		loadModel(fname);
	}
	if (key == 43)
	{
		//translate(0.0, 0.0, -0.25);
		t.z = -0.2;
		pos = pos + N * t.z;
		/*
		Sx = Sx + 0.5;
		Sy = Sy + 0.5;
		Sz = Sz + 0.5;*/
		/*nearVal = 1.0;
		farVal = 0.0;*/
	}
	if (key == 45)
	{
		t.z = 0.2;
		pos = pos + N * t.z;
		/*
		if (Sx > 0.5 && Sy > 0.5 && Sz > 0.5)
		{
			Sx = Sx - 0.5;
			Sy = Sy - 0.5;
			Sz = Sz - 0.5;*/
			/*nearVal = 0.0;
			farVal = 1.0;*/
		//translate(0.0, 0.0, 0.25);
		
	}
	if (key == 49)
		mode = 1; 
	if (key == 50)
		mode = 2;
	if (key == 51)
		mode = 3; 

	if (key == 27)
	{
		pos.x = 0.0; pos.y = 0.0; pos.z = 2 *(zmax-zmin);
		Ux = 0.0; Uy = 1.0; Uz = 0.0;
		Tx = 0.0; Ty = 0.0; Tz = -1.0;
		/*cl = 3;
		mode = 1;*/
		Sx = 1.0;
		Sy = 1.0;
		Sz = 1.0;
	}
	if (key == 'r') cl = 0;
	if (key == 'g') cl = 1;
	if (key == 'b') cl = 2;
	if (key == 'm')
	{
		setCameraAxis(N, angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
	}
	if (key == 'l')
	{
		setCameraAxis(N, -angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
	}

	/*if (stop)
		glutIdleFunc(NULL);
	else
	{
		glutIdleFunc(drawmesh);
	}*/
	glutIdleFunc(drawmesh);
}

void SpecialKeys(int key, int x, int y)
{
	
	switch (key) 
	{
	case GLUT_KEY_LEFT:
		setCameraAxis(U, angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_RIGHT:
		setCameraAxis(U, -angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_UP:
		setCameraAxis(V, angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_DOWN:
		setCameraAxis(V, -angle);
		dotp();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	}
	glutIdleFunc(drawmesh);
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	glutCreateWindow("3D Model Window");
	loadModel("bunny.obj");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(drawmesh);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}