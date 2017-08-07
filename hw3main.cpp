#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include<math.h>
#include <stdio.h>
#include<iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#define MX 35000
#define PV 100000
#define PI 3.14159265359

using namespace glm;
using namespace std;

int width, heigth;
int fixWin, shWin;
bool stop = false;
int mode = 2;
float angle = 0.01;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;
int cl = 3, rot = 1, oren = 2;
GLfloat nearVal = 0.0, farVal = 1.0;
GLfloat xRot = 0.0, yRot = 0.0;
GLfloat c[][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 0.0, 0.0, 0.0 } };
float vdist = -5.0;
GLfloat Cx = 0.0, Cy = 0.0, Cz = 0.0;
GLfloat ex = 0.0, ey = 0.0, ez = 0.0;
GLfloat Ux = 0.0, Uy = 1.0, Uz = 0.0;
GLfloat Tx = 0.0, Ty = 0.0, Tz = -1.0;
GLfloat Lx = 0.0, Ly = 0.0, Lz = 0.0;
GLfloat xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
GLfloat Sx = 1.0, Sy = 1.0, Sz = 1.0;
GLfloat x[MX] = { 0 }, y[MX] = { 0 }, z[MX] = { 0 };
GLfloat dist;
GLfloat fovy = 60, znear = 1.0, zfar = 10.0, aspect = 1;

int rmode = GL_TRIANGLES;
GLint p[PV][3] = { 0 };
int fCount = 0;

//Lighting
bool ON = true;
int aIx = 4, aI = 4, dI = 3, sI = 3;

vector<GLfloat> vertex;
vector<GLint> face;
int vc = 0, fc = 0;

// for GLSL
GLuint vsID, fsID, pID, VBO, VAO, EBO;

// for uniform input
GLfloat projectionMat[16] = { 0 }, modelViewMat[16] = { 0 };
GLfloat projectionMat1[16], modelViewMat1[16];

vec3 V(0.0, 1.0, 0.0);
vec3 N(0.0, 0.0, 1.0);
vec3 U(1.0, 0.0, 0.0);

//GLfloat m[4][4] = {0.0};
mat3 m;
vec3 t(0.0);
mat3 rotMat;
vec3 pos(0.0);
GLfloat Nor[PV][3] = { 0.0 };
GLfloat Nx[MX] = { 0 }, Ny[MX] = { 0 }, Nz[MX] = { 0 };
GLint CNT[MX] = { 0 };


class Camera {
public:
	float pos[3];
	float dir[3];
	float uvn[3][3];
};

Camera cam;


void resetCamera() {
	cam.pos[0] = 0.0;
	cam.pos[1] = 0.0;
	cam.pos[2] = 5.0;

	cam.dir[0] = 0.0;
	cam.dir[1] = 0.0;
	cam.dir[2] = 0.0;

	cam.uvn[0][0] = 1.0;
	cam.uvn[0][1] = 0.0;
	cam.uvn[0][2] = 0.0;
	cam.uvn[1][0] = 0.0;
	cam.uvn[1][1] = 1.0;
	cam.uvn[1][2] = 0.0;
	cam.uvn[2][0] = 0.0;
	cam.uvn[2][1] = 0.0;
	cam.uvn[2][2] = 1.0;

}

void clearModel()
{
	/*memset(x, 0, MX*sizeof(GLfloat));
	memset(y, 0, MX*sizeof(GLfloat));
	memset(z, 0, MX*sizeof(GLfloat));
	memset(p, 0, PV * 3 * sizeof(GLint));*/
	vertex.clear();
	face.clear();
}

void initLight(void)
{


	glClearColor(0.0, 0.0, 0.0, 0.0);
	if (mode == 1)
	{
		glShadeModel(GL_SMOOTH);
	}
	if (mode == 2)
	{
		glShadeModel(GL_FLAT);
	}

	GLfloat pos[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat amb[] = { c[aI][0], c[aI][1], c[aI][2], 1.0 };
	GLfloat dif[] = { c[dI][0], c[dI][1], c[dI][2], 1.0 };
	GLfloat spec[] = { c[sI][0], c[sI][1], c[sI][2], 1.0 };

	GLfloat glAmb[] = { c[aIx][0], c[aIx][1], c[aIx][2], 1.0 };
	glLoadIdentity();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glAmb);


	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	if (ON)
	{
		glEnable(GL_LIGHT0);
		//glEnable(GL_LIGHTING);
	}
	if (!ON)
	{
		glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHT_MODEL_AMBIENT);
		glAmb[0] = glAmb[1] = glAmb[2] = 0.25;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glAmb);
	}
	glEnable(GL_DEPTH_TEST);
}

void construct_model(char modelname[])
{
	FILE  *file;
	//GLfloat color[3] = color1[cl];
	int j = 0;
	char l1[20], l2[20], l3[20], h[10];
	file = fopen(modelname, "r");
	int i = 0;
	int m = 0;
	vector<GLfloat> tvertex;
	vector<GLint> tface;

	while (fscanf(file, "%s", h) != EOF && strcmp(h, "v") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		tvertex.push_back(atof(l1));
		tvertex.push_back(atof(l2));
		tvertex.push_back(atof(l3));
		tvertex.push_back(c[cl][0]);
		tvertex.push_back(c[cl][1]);
		tvertex.push_back(c[cl][2]);


	}
	while (strcmp(h, "f") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		tface.push_back(atoi(l1) - 1);
		tface.push_back(atoi(l2) - 1);
		tface.push_back(atoi(l3) - 1);
		if (fscanf(file, "%s", h) == EOF)
		{
			break;
		}

	}

	vertex = tvertex;
	face = tface;
}

void normalCal()
{
	unsigned int i, j = 0, k = 0;
	for (i = 0; i < face.size(); i = i + 3)
	{
		int vp = (face[i]) * 6;
		GLfloat v1[3] = { vertex[vp++], vertex[vp++], vertex[vp] };
		vp = (face[i + 1]) * 6;
		GLfloat v2[3] = { vertex[vp++], vertex[vp++], vertex[vp] };
		vp = (face[i + 2]) * 6;
		GLfloat v3[3] = { vertex[vp++], vertex[vp++], vertex[vp] };

		vec3 n1, n2, n;
		n1.x = v2[0] - v1[0];
		n1.y = v2[1] - v1[1];
		n1.z = v2[2] - v1[2];

		n2.x = v3[0] - v1[0];
		n2.y = v3[1] - v1[1];
		n2.z = v3[2] - v1[2];

		n = cross(n1, n2);

		Nor[j][0] = n.x;
		Nor[j][1] = n.y;
		Nor[j][2] = n.z;

		Nx[face[i]] += n.x;
		Ny[face[i]] += n.y;
		Nz[face[i]] += n.z;

		Nx[face[i + 1]] += n.x;
		Ny[face[i + 1]] += n.y;
		Nz[face[i + 1]] += n.z;

		Nx[face[i + 2]] += n.x;
		Ny[face[i + 2]] += n.y;
		Nz[face[i + 2]] += n.z;

		CNT[face[i]] += 1;
		CNT[face[i + 1]] += 1;
		CNT[face[i + 2]] += 1;

		j++;
	}

}


void polygon()
{
	//int cl;
	unsigned int i, j = 0;
	for (i = 0; i < face.size(); i = i + 3)
	{
		glBegin(rmode);
		int vp = (face[i]) * 6;
		GLfloat v1[3] = { vertex[vp++], vertex[vp++], vertex[vp] };
		vp = (face[i + 1]) * 6;
		GLfloat v2[3] = { vertex[vp++], vertex[vp++], vertex[vp] };
		vp = (face[i + 2]) * 6;
		GLfloat v3[3] = { vertex[vp++], vertex[vp++], vertex[vp] };

		vec3 n1, n2, n3;

		if (mode == 2)
		{
			glNormal3f(Nor[j][0], Nor[j][1], Nor[j][2]);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glVertex3fv(v3);
			j++;
		}

		if (mode == 1)
		{
			n1.x = Nx[face[i]] / CNT[face[i]];
			n1.y = Ny[face[i]] / CNT[face[i]];
			n1.z = Nz[face[i]] / CNT[face[i]];
			n2.x = Nx[face[i + 1]] / CNT[face[i + 1]];
			n2.y = Ny[face[i + 1]] / CNT[face[i + 1]];
			n2.z = Nz[face[i + 1]] / CNT[face[i + 1]];
			n3.x = Nx[face[i + 2]] / CNT[face[i + 2]];
			n3.y = Ny[face[i + 2]] / CNT[face[i + 2]];
			n3.z = Nz[face[i + 2]] / CNT[face[i + 2]];

			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3fv(v1);
			glNormal3f(n2.x, n2.y, n2.z);
			glVertex3fv(v2);
			glNormal3f(n3.x, n3.y, n3.z);
			glVertex3fv(v3);

		}


		glEnd();
	}
}

void ModelViewMatrix()
{

	modelViewMat[0] = cam.uvn[0][0];
	modelViewMat[1] = cam.uvn[1][0];
	modelViewMat[2] = cam.uvn[2][0];
	modelViewMat[3] = 0.0;
	modelViewMat[4] = cam.uvn[0][1];
	modelViewMat[5] = cam.uvn[1][1];
	modelViewMat[6] = cam.uvn[2][1];
	modelViewMat[7] = 0.0;
	modelViewMat[8] = cam.uvn[0][2];
	modelViewMat[9] = cam.uvn[1][2];
	modelViewMat[10] = cam.uvn[2][2];
	modelViewMat[11] = 0.0;
	modelViewMat[12] = -(cam.uvn[0][0] * cam.pos[0] + cam.uvn[0][1] * cam.pos[1] + cam.uvn[0][2] * cam.pos[2]);
	modelViewMat[13] = -(cam.uvn[1][0] * cam.pos[0] + cam.uvn[1][1] * cam.pos[1] + cam.uvn[1][2] * cam.pos[2]);
	modelViewMat[14] = -(cam.uvn[2][0] * cam.pos[0] + cam.uvn[2][1] * cam.pos[1] + cam.uvn[2][2] * cam.pos[2]);
	modelViewMat[15] = 1.0;

}

void ProjectionMatrix()
{
	float t, b, l, r;
	float x, y;
	float a1, a2, a3, a4;
	aspect = 1;
	y = znear * tanf(PI * fovy / 360);
	x = y * aspect;
	l = -x;
	r = x;
	b = -y;
	t = y;
	a1 = 2.0 * znear;
	a2 = r - l;
	a3 = t - b;
	a4 = zfar - znear;

	projectionMat[0] = a1 / a2;
	projectionMat[5] = a1 / a3;
	projectionMat[10] = (-zfar - znear) / a4;
	projectionMat[11] = -1.0;
	projectionMat[14] = (-a1 * zfar) / a4;
}


void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}


void fixedDisplay(void)
{
	glutSetWindow(fixWin);
	initLight();
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, znear, zfar);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMat1);

	Cx = cam.pos[0]; Cy = cam.pos[1]; Cz = cam.pos[2];
	Tx = cam.dir[0]; Ty = cam.dir[1]; Tz = cam.dir[2];
	Ux = cam.uvn[1][0]; Uy = cam.uvn[1][1]; Uz = cam.uvn[1][2];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, Tx, Ty, Tz, Ux, Uy, Uz);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat1);
	//glScalef(Sx, Sy, Sz);
	polygon();
	glFlush();
	glutSwapBuffers();
}

GLhandleARB v, f, f2, pg;


char *textFileRead(const char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count>0) {
				content = (char *)malloc(sizeof(char)*(count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void setShaders() {
	char *vs = NULL, *fs = NULL;

	v = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	vs = textFileRead("vertexShaderlight.txt");
	fs = textFileRead("fragShaderlight.txt");

	const char* vv = vs;
	const char* ff = fs;

	glShaderSourceARB(v, 1, &vv, NULL);
	glShaderSourceARB(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShaderARB(v);
	glCompileShaderARB(f);

	pg = glCreateProgramObjectARB();
	glAttachShader(pg, v);
	glAttachShader(pg, f);

	glLinkProgramARB(pg);
	glUseProgramObjectARB(pg);
}

void shaderDisplay()
{
	glutSetWindow(shWin);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	//setShader();

	GLfloat pos[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat amb[] = { c[aI][0], c[aI][1], c[aI][2], 1.0 };
	GLfloat dif[] = { c[dI][0], c[dI][1], c[dI][2], 1.0 };
	GLfloat spec[] = { c[sI][0], c[sI][1], c[sI][2], 1.0 };
	GLfloat glAmb[] = { c[aIx][0], c[aIx][1], c[aIx][2], 1.0 };

	if (!ON)
	{

		amb[0] = amb[1] = amb[2] = 0;
		dif[0] = dif[1] = dif[2] = 0;
		spec[0] = spec[1] = spec[2] = 0;
		glAmb[0] = glAmb[1] = glAmb[2] = 0;
	}

	/*GLfloat amb1[] = { 1,.5,.5,0 };
	GLfloat dif[] = { .5,.5,.5,0 };
	GLfloat spec[] = { 0,0,0,0 };
	GLfloat pos[] = { 1,0,1 };*/
	glLoadIdentity();

	int amb_id = glGetUniformLocation(pg, "amb");
	glUniform4fv(amb_id, 1, amb);
	int dif_id = glGetUniformLocation(pg, "dif");
	glUniform4fv(dif_id, 1, dif);
	int spec_id = glGetUniformLocation(pg, "spec");
	glUniform4fv(spec_id, 1, spec);
	int glAmb_id = glGetUniformLocation(pg, "glAmb");
	glUniform4fv(spec_id, 1, glAmb);
	int pos_id = glGetUniformLocation(pg, "lpos");
	glUniform4fv(pos_id, 1, pos);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, znear, zfar);

	Cx = cam.pos[0]; Cy = cam.pos[1]; Cz = cam.pos[2];
	Tx = cam.dir[0]; Ty = cam.dir[1]; Tz = cam.dir[2];
	Ux = cam.uvn[1][0]; Uy = cam.uvn[1][1]; Uz = cam.uvn[1][2];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, Tx, Ty, Tz, Ux, Uy, Uz);
	polygon();
	glFlush();
	glutSwapBuffers();
}

void drawmesh()
{

	shaderDisplay();
	fixedDisplay();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	char fname[30];

	if (key == 'q' || key == 'Q') exit(0);

	if (key == 'o')
	{
		ON = !ON;
	}

	if (key == 'g')
	{
		mode = 1;
	}
	if (key == 'f')
	{
		mode = 2;
	}

	if (key == '1')
	{
		aIx = 0;
	}
	if (key == '2')
	{
		aIx = 1;
	}
	if (key == '3')
	{
		aIx = 2;
	}
	if (key == '0')
	{
		aIx = 4;
		aI = 4;
		dI = sI = 3;
	}
	if (key == '4')
	{
		aI = dI = sI = 0;
		aIx = 4;
	}
	if (key == '5')
	{
		aI = dI = sI = 1; aIx = 4;
	}
	if (key == '6')
	{
		aI = dI = sI = 2;
		aIx = 4;
	}

	drawmesh();
}

void Shaderwindow()
{
	glutInitWindowSize(500, 500);
	shWin = glutCreateWindow("Shader Pipeline Window");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glewInit();
	setShaders();
	glutDisplayFunc(shaderDisplay);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
}

void Fixedwindow()
{
	glutInitWindowSize(500, 500);
	fixWin = glutCreateWindow("Fixed Pipeline Window");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(fixedDisplay);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
	resetCamera();
	glutInit(&argc, argv);
	construct_model("bunny.obj");
	normalCal();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	Fixedwindow();
	Shaderwindow();
	glutMainLoop();
	return 0;
}