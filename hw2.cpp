#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include<math.h>
#include <stdio.h>
#include<iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <glm/glm.hpp> 

#define MX 35000
#define PV 100000
#define PI 3.14159265359

using namespace glm;
using namespace std;

GLuint vsID, fsID, pID, VBO, VAO, EBO;
int width, heigth;
float clr[] = { 1.0,0.0,0.0,0.0 };
int mw, sw1, sw2, border = 6;
bool stop = false;
int mode = 1;
float angle = 0.01;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;
int cl = 3, rot = 1, oren = 2;
GLfloat nearVal = 0.0, farVal = 1.0;
GLfloat xRot = 0.0, yRot = 0.0;
GLfloat color[][3] = { { -1.0, -1.0, -1.0 },{ 1.0, -1.0, -1.0 },{ 1.0, 1.0, -1.0 },{ -1.0, 1.0, -1.0 },{ -1.0, -1.0, 1.0 },{ 1.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ -1.0, 1.0, 1.0 } };
GLfloat color1[][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0 } };

GLfloat Cx = 0.0, Cy = 0.0, Cz = 0.0;
GLfloat ex = 0.0, ey = 0.0, ez = 0.0;
GLfloat Ux = 0.0, Uy = 1.0, Uz = 0.0;
GLfloat Tx = 0.0, Ty = 0.0, Tz = -1.0;
GLfloat Lx = 0.0, Ly = 0.0, Lz = 0.0;
GLfloat xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
GLfloat Sx = 1.0, Sy = 1.0, Sz = 1.0;
GLfloat x[MX] = { 0 }, y[MX] = { 0 }, z[MX] = { 0 };
GLfloat dist;
//GLhandleARB vsID, fsID, pID;
GLfloat fovy = 45, znear = 1.0, zfar = 100.0, aspect;

// for uniform input
GLfloat projectionMat[16] = { 0 }, modelViewMat[16] = { 0 }, perspectiveMat[16] = { 0 };

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

class Camera {
public:
	vec3 pos = vec3(0, 0, 4);
	vec3 dir = vec3(0, 0, -1);
	vec3 up = vec3(0,-1,0);
	vec3 rotate;
	double fov, pov;
};

Camera cam;

void clearModel()
{
	memset(x, 0, MX*sizeof(GLfloat));
	memset(y, 0, MX*sizeof(GLfloat));
	memset(z, 0, MX*sizeof(GLfloat));
	memset(p, 0, PV * 3 * sizeof(GLint));
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
	rotMat = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
		t*x*y + z*s, t*y*y + c, t*y*z - x*s,
		t*x*z - y*s, t*y*z + x*s, t*z*z + c);
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
	pos.z = 2 * (zmax - zmin);
	Tz = -1.0;
}
void draw(int i)
{
	//glColor3fv(color1[cl]);
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

//vec3 cross(vec3 a, vec3 b)
//{
//	vec3 res;
//	res.x = a.y*b.z - a.z*b.y;
//	res.y = a.z*b.x - a.x*b.z;
//	res.z = a.x*b.y - a.y*b.x;
//	return res;
//}

/*vec3 normalize(vec3 a)
{
	vec3 res;
	double m = sqrt((a.x*a.x) + (a.y*a.y)*(a.z*a.z));
	/*res.x = a.x / m;
	res.y = a.y / m;
	res.z = a.z / m;
	res = vec3(a.x / m, a.y / m, a.z / m);
	return res;
}*/

void ModelViewMatrix()
{
	vec3 n = vec3(cam.dir.x, cam.dir.y, cam.dir.z);
	n = normalize(n);
	vec3 u = vec3(cam.up.x, cam.up.y, cam.up.z);
	vec3 v = cross(u, n);
	v = normalize(v);
	u = cross(v, n);
	modelViewMat[0] = v.x;
	modelViewMat[1] = v.y;
	modelViewMat[2] = v.z;
	modelViewMat[4] = u.x;
	modelViewMat[5] = u.y;
	modelViewMat[6] = u.z;
	modelViewMat[8] = -n.x;
	modelViewMat[9] = -n.y;
	modelViewMat[10] = -n.z;
	modelViewMat[15] = 1;
}

void ProjectionMatrix()
{
	aspect = 1;
	float t, b, l, r;
	float a11, a22, a33, a34;
	t = znear * tan(PI * fovy / 360);
	b = -t;
	r = t * aspect;
	l = -r;
	a11 = 2 * znear / (r - l);
	//a11 = 1 / (tan(PI*fovy / 360));
	a22 = 2 * znear / (t - b);
	a33 = -(zfar + znear) / (zfar - znear);
	a34 = -2 * zfar*znear / (zfar - znear);
	projectionMat[0] = a11;
	projectionMat[5] = a22;
	projectionMat[10] = a33;
	projectionMat[11] = a34;
	projectionMat[14] = -1;
}

void readShaderFile(const GLchar* shaderPath, std::string& shaderCode)
{

	//std::ifstream shaderFile;

	//// ensures ifstream objects can throw exceptions:
	//shaderFile.exceptions(std::ifstream::badbit);
	//try
	//{
	//	// Open files
	//	shaderFile.open(shaderPath);
	//	std::stringstream shaderStream;

	//	//cout << shaderPath << endl;
	//	// Read file's buffer contents into streams
	//	shaderStream << shaderFile.rdbuf();

	//	// close file handlers
	//	shaderFile.close();

	//	// Convert stream into GLchar array
	//	shaderCode = shaderStream.str();
	//}
	//catch (std::ifstream::failure e)
	//{
	//	std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	//}

	std::ifstream shaderFile;

	// ensures ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// Convert stream into GLchar array
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

}

//void setShaders()
//{
//	//char *vs = NULL, *fs = NULL;
//
//	//std::string vertShaderString; //empty string
//	//std::string fragShaderString; //empty string
//
//	//vsID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
//	//fsID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
//
//	//readShaderFile("vertshader.txt", vertShaderString);
//	//readShaderFile("fragmentshader.txt", fragShaderString);
//
//	//cout << vertShaderString << endl;
//	//cout << fragShaderString << endl;
//
//	//const GLchar * pVertShaderSource = vertShaderString.c_str();
//	//const GLchar * pFragShaderSource = fragShaderString.c_str();
//
//	//glShaderSourceARB(vsID, 1, &pVertShaderSource, NULL);
//	//glShaderSourceARB(fsID, 1, &pFragShaderSource, NULL);
//
//	//glCompileShaderARB(vsID);
//	//glCompileShaderARB(fsID);
//
//	//pID = glCreateProgramObjectARB();
//	//glAttachShader(pID, vsID);
//	//glAttachShader(pID, fsID);
//
//	//glLinkProgramARB(pID);
//	//glUseProgramObjectARB(pID);
//
//	char *vs = NULL, *fs = NULL;
//
//	std::string vertShaderString; //empty string
//	std::string fragShaderString; //empty string
//
//	vsID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
//	fsID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
//
//	readShaderFile("vertshader.txt", vertShaderString);
//	readShaderFile("fragmentshader.txt", fragShaderString);
//
//	cout << vertShaderString << endl;
//	cout << fragShaderString << endl;
//
//	const GLchar * pVertShaderSource = vertShaderString.c_str();
//	const GLchar * pFragShaderSource = fragShaderString.c_str();
//
//	glShaderSourceARB(vsID, 1, &pVertShaderSource, NULL);
//	glShaderSourceARB(fsID, 1, &pFragShaderSource, NULL);
//
//	glCompileShaderARB(vsID);
//	glCompileShaderARB(fsID);
//
//	pID = glCreateProgramObjectARB();
//	glAttachShader(pID, vsID);
//	glAttachShader(pID, fsID);
//
//	glLinkProgramARB(pID);
//	glUseProgramObjectARB(pID);
//}

void setShaders()
{
	char *vs = NULL, *fs = NULL;

	std::string vertShaderString; //empty string
	std::string fragShaderString; //empty string

	vsID = glCreateShader(GL_VERTEX_SHADER);
	fsID = glCreateShader(GL_FRAGMENT_SHADER);

	readShaderFile("vs.txt", vertShaderString);
	readShaderFile("fs.txt", fragShaderString);

	const GLchar * pVertShaderSource = vertShaderString.c_str();
	const GLchar * pFragShaderSource = fragShaderString.c_str();

	glShaderSource(vsID, 1, &pVertShaderSource, NULL);
	glShaderSource(fsID, 1, &pFragShaderSource, NULL);

	glCompileShader(vsID);
	glCompileShader(fsID);

	pID = glCreateProgram();
	glAttachShader(pID, vsID);
	glAttachShader(pID, fsID);

	glLinkProgram(pID);
	glUseProgram(pID);

	//// get uniform input 
	//GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
	//GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");

	//glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	//glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);


	//glDeleteShader(vsID);
	//glDeleteShader(fsID);
}

void display(void)
{
	glUseProgram(pID);

	glValidateProgram(pID);
	GLint validate = 0;
	glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
		

	ProjectionMatrix();
	ModelViewMatrix();

	float translationMat[16] = {
		1,0,0,-cam.pos.x,
		0,1,0,-cam.pos.y,
		0,0,1,-cam.pos.z,
		0,0,0,1
	};
	GLint colorID = glGetUniformLocation(pID, "color");
	glUniform4fv(colorID, 1, clr);

	/*int tID = glGetUniformLocation(pID, "translation");
	int mvID = glGetUniformLocation(pID, "model");
	int persID = glGetUniformLocation(pID, "projection");

	
	glUniformMatrix4fv(mvID, 1, true, modelViewMat);
	glUniformMatrix4fv(persID, 1, true, projectionMat);
	glUniformMatrix4fv(tID, 1, true, translationMat);*/

	GLint modeViewMatLocation = glGetUniformLocation(pID, "model");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projection");
	GLint translationMatLocation = glGetUniformLocation(pID, "translation");

	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);
	glUniformMatrix4fv(translationMatLocation, 1, GL_FALSE, translationMat);

	cout << "Modelview" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << modelViewMat[i] << endl;
	}
	cout << endl;
	cout << "ProjectionMat" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << projectionMat[i] << endl;
	}
	cout << endl;
	cout << "Translation" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << translationMat[i] << endl;
	}

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
	triangle();
	glFlush();
	glutSwapBuffers();
}


void drawmesh()
{
	glutPostRedisplay();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspect = (float)w / (float)h;
	//gluPerspective(fovy, aspect, znear, zfar);
	if (w <= h) {
		glOrtho(-2.0, 2.0, -2.0*(GLfloat)h / (GLfloat)w,
			2.0*(GLfloat)h / (GLfloat)w,
			-10.0, 10.0);
	}
	else {
		glOrtho(-2.0*(GLfloat)w / (GLfloat)h,
			2.0*(GLfloat)w / (GLfloat)h,
			-2.0, 2.0, -10.0, 10.0);
	}
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

void dot()
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
	//if (key == 'p')
	//{
	//	nearVal += 1.0;
	//	if (farVal < nearVal)
	//		farVal = nearVal;
	//	myReshape(width, heigth);
	//}
	//if (key == 'o')
	//{

	//	nearVal -= 1.0;
	//	if (nearVal < 1.0)
	//		nearVal = 1.0;
	//	myReshape(width, heigth);
	//}
	//if (key == 'i')
	//{
	//	farVal += 1.0;
	//	myReshape(width, heigth);
	//}
	//if (key == 'u')
	//{
	//	farVal -= 1.0;
	//	if (farVal < nearVal)
	//		farVal = nearVal;
	//	myReshape(width, heigth);
	//}
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
		pos.x = 0.0; pos.y = 0.0; pos.z = 2 * (zmax - zmin);
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
		dot();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
	}
	if (key == 'l')
	{
		setCameraAxis(N, -angle);
		dot();
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
		dot();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_RIGHT:
		setCameraAxis(U, -angle);
		dot();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_UP:
		setCameraAxis(V, angle);
		dot();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	case GLUT_KEY_DOWN:
		setCameraAxis(V, -angle);
		dot();
		Tx = N.x*dist;
		Ty = N.y*dist;
		Tz = N.z*dist;
		break;
	}
	glutIdleFunc(drawmesh);
}



//char *textFileRead(const char *fn) {
//	FILE *fp;
//	char *content = NULL;
//	int count = 0;
//	if (fn != NULL) {
//		fp = fopen(fn, "rt");
//		if (fp != NULL) {
//			fseek(fp, 0, SEEK_END);
//			count = ftell(fp);
//			rewind(fp);
//			if (count>0) {
//				content = (char *)malloc(sizeof(char)*(count + 1));
//				count = fread(content, sizeof(char), count, fp);
//				content[count] = '\0';
//			}
//			fclose(fp);
//		}
//	}
//	return content;
//}
//
//void setShaders() {
//	char *vs = NULL, *fs = NULL;
//
//	vsID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
//	fsID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
//
//	vs = textFileRead("vs.txt");
//	fs = textFileRead("fs.txt");
//
//	const char* vv = vs;
//	const char* ff = fs;
//
//	glShaderSourceARB(vsID, 1, &vv, NULL);
//	glShaderSourceARB(fsID, 1, &ff, NULL);
//
//	free(vs); free(fs);
//
//	glCompileShaderARB(vsID);
//	glCompileShaderARB(fsID);
//
//	pID = glCreateProgramObjectARB();
//	glAttachShader(pID, vsID);
//	glAttachShader(pID, fsID);
//
//	glLinkProgramARB(pID);
//
//	glUseProgramObjectARB(pID);
//}


void initBufferObject(void)
{
	// Setup cube vertex data
	GLfloat vertData[8 * 6];
	// position information
	vertData[0 * 6 + 0] = vertData[1 * 6 + 0] = vertData[2 * 6 + 0] = vertData[3 * 6 + 0] = -1.0f;
	vertData[4 * 6 + 0] = vertData[5 * 6 + 0] = vertData[6 * 6 + 0] = vertData[7 * 6 + 0] = 1.0f;
	vertData[0 * 6 + 1] = vertData[1 * 6 + 1] = vertData[4 * 6 + 1] = vertData[5 * 6 + 1] = -1.0f;
	vertData[2 * 6 + 1] = vertData[3 * 6 + 1] = vertData[6 * 6 + 1] = vertData[7 * 6 + 1] = 1.0f;
	vertData[0 * 6 + 2] = vertData[3 * 6 + 2] = vertData[4 * 6 + 2] = vertData[7 * 6 + 2] = 1.0f;
	vertData[1 * 6 + 2] = vertData[2 * 6 + 2] = vertData[5 * 6 + 2] = vertData[6 * 6 + 2] = -1.0f;

	// color
	vertData[0 * 6 + 3] = vertData[1 * 6 + 3] = vertData[2 * 6 + 3] = vertData[3 * 6 + 3]
		= vertData[4 * 6 + 3] = vertData[5 * 6 + 3] = vertData[6 * 6 + 3] = vertData[7 * 6 + 3] = 1.0f;

	vertData[0 * 6 + 4] = vertData[1 * 6 + 4] = vertData[2 * 6 + 4] = vertData[3 * 6 + 4]
		= vertData[4 * 6 + 4] = vertData[5 * 6 + 4] = vertData[6 * 6 + 4] = vertData[7 * 6 + 4] = 0.0f;

	vertData[0 * 6 + 5] = vertData[1 * 6 + 5] = vertData[2 * 6 + 5] = vertData[3 * 6 + 5]
		= vertData[4 * 6 + 5] = vertData[5 * 6 + 5] = vertData[6 * 6 + 5] = vertData[7 * 6 + 5] = 0.0f;

	GLuint indices[] =
	{ 0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		5, 6, 2, 1,
		7, 4, 0, 3 };

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Use depth buffering for hidden surface elimination
	glEnable(GL_DEPTH_TEST);

	ProjectionMatrix();
	ModelViewMatrix();
	
}

void init()
{
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);
}

void resetCamera() {
	cam.pos.x = 0.0f;
	cam.pos.y = 0.0f;
	cam.pos.z = 4.0f;

	cam.dir.x = 0.0;
	cam.dir.y = 0.0;
	cam.dir.z = -1.0;

	cam.up.x = 0.0;
	cam.up.y = -1.0;
	cam.up.z = 0.0;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	loadModel("bunny.obj");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	
	glutCreateWindow("Shader Pipeline Window");
	//display();
	glewInit();
	setShaders();
	glutDisplayFunc(display);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	//glutReshapeFunc(myReshape);
	init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	
	glutMainLoop();
	return 0;
}