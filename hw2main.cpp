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

//char* fname = "cactus.obj";
int width, heigth;
int mw, sw1, sw2, border = 6;
bool stop = false;
int mode = 2;
float angle = 0.01;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;
int cl = 3, rot = 1, oren = 2;
GLfloat nearVal = 0.0, farVal = 1.0;
GLfloat xRot = 0.0, yRot = 0.0;
GLfloat color1[][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0 } };
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

int rmode = GL_POINTS;
GLint p[PV][3] = { 0 };
int fCount = 0;

//GLfloat vertex[210000];
//GLint face[300000];
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


void loadModel(char* fname)
{
	FILE  *file;
	//GLfloat color[3] = color1[cl];
	int j = 0;
	char l1[20], l2[20], l3[20], h[10];
	file = fopen(fname, "r");
	int c = 0;
	int i = 0;
	int m = 0;
	vector<GLfloat> tvertex;
	vector<GLint> tface;

	while (fscanf(file, "%s", h) != EOF && strcmp(h, "v") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		x[m] = atof(l1);
		y[m] = atof(l2);
		z[m] = atof(l3);
		m++;

		/*vertex[c] = atof(l1);
		c++;
		vertex[c] = atof(l2);
		c++;
		vertex[c] = atof(l3);
		c++;
		vertex[c] = color1[cl][0];
		c++;
		vertex[c] = color1[cl][1];
		c++;
		vertex[c] = color1[cl][2];
		c++;
		vc++;*/
		tvertex.push_back(atof(l1));
		tvertex.push_back(atof(l2));
		tvertex.push_back(atof(l3));
		tvertex.push_back(color1[cl][0]);
		tvertex.push_back(color1[cl][1]);
		tvertex.push_back(color1[cl][2]);

		if (xmin > x[m])
		{
			xmin = x[m];
		}
		if (xmax < x[c])
		{
			xmax = x[m];
		}
		if (ymin > y[m])
		{
			ymin = y[m];
		}
		if (ymax < y[m])
		{
			ymax = y[m];
		}
		if (zmin > z[m])
		{
			zmin = z[m];
		}
		if (zmax < z[m])
		{
			zmax = z[m];
		}
		
	}
	while (strcmp(h, "f") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		p[i][0] = atoi(l1);
		p[i][1] = atoi(l2);
		p[i][2] = atoi(l3);
		i++;

		/*face[j] = atoi(l1);
		j++;
		face[j] = atoi(l2);
		j++;
		face[j] = atoi(l3);
		j++;
		fc++;*/
		tface.push_back(atoi(l1));
		tface.push_back(atoi(l2));
		tface.push_back(atoi(l3));
		if (fscanf(file, "%s", h) == EOF)
		{
			break;
		}

	}
	
	vertex = tvertex;
	face = tface;
}
//void draw(int i)
//{
//	glColor3fv(color1[cl]);
//	glVertex3d((x[p[i][0] - 1]), (y[p[i][0] - 1]), (z[p[i][0] - 1]));
//	glVertex3d((x[p[i][1] - 1]), (y[p[i][1] - 1]), (z[p[i][1] - 1]));
//	glVertex3d((x[p[i][2] - 1]), (y[p[i][2] - 1]), (z[p[i][2] - 1]));
//}
//
//
//void triangle()
//{
//	//int cl;
//	for (int i = 0; i < fCount; i++)
//	{
//		switch (mode)
//		{
//		case 1:
//			glBegin(GL_POLYGON);
//			draw(i);
//			glEnd();
//			break;
//		case 2:
//			glBegin(GL_POINTS);
//			draw(i);
//			glEnd();
//			break;
//		case 3:
//
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			glBegin(GL_POLYGON);
//			draw(i);
//			glEnd();
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//			break;
//		}
//
//		//cl = i % 8;
//		//cl = 2;
//	}
//}

void draw(int i)
{
	int vp = (i)* 6;
	GLfloat tvert[][3] = { { vertex[vp++], vertex[vp++], vertex[vp] } };
	glColor3fv(color1[cl]);
	glVertex3fv(tvert[0]);
}


void triangle()
{
	//int cl;
	unsigned int i;
	for ( i = 0; i < face.size(); i++)
	{
		glBegin(rmode);
		//cout << "face[0] : " << face[i] << endl;
		draw(face[i++]-1);
		draw(face[i++]-1);
		draw(face[i]-1);
		glEnd();
		//cout << "I value : " << i << endl;
	}
	//cout << "I value : " << i << endl;
}

void updatecolor()
{
	for (unsigned int i = 0; i < vertex.size(); i=i+6)
	{
		//cout << "I value : " << i << endl;
		vertex[i + 3] = color1[cl][0];
		vertex[i + 4] = color1[cl][1];
		vertex[i + 5] = color1[cl][2];

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
	//modelViewMat[0] = cam.uvn[0][0];
	//modelViewMat[1] = cam.uvn[0][1];
	//modelViewMat[2] = cam.uvn[0][2];
	//modelViewMat[3] = 0.0;
	//modelViewMat[4] = cam.uvn[1][0];
	//modelViewMat[5] = cam.uvn[1][1];
	//modelViewMat[6] = cam.uvn[1][2];
	//modelViewMat[7] = 0.0;
	//modelViewMat[8] = cam.uvn[2][0];
	//modelViewMat[9] = cam.uvn[2][1];
	//modelViewMat[10] = cam.uvn[2][2];
	//modelViewMat[11] = 0.0;
	//modelViewMat[12] = -(cam.uvn[0][0] * cam.pos[0] + cam.uvn[0][1] * cam.pos[1] + cam.uvn[0][2] * cam.pos[2]);
	//modelViewMat[13] = -(cam.uvn[1][0] * cam.pos[0] + cam.uvn[1][1] * cam.pos[1] + cam.uvn[1][2] * cam.pos[2]);
	//modelViewMat[14] = -(cam.uvn[2][0] * cam.pos[0] + cam.uvn[2][1] * cam.pos[1] + cam.uvn[2][2] * cam.pos[2]);
	//modelViewMat[15] = 1.0;

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


void readShaderFile(const GLchar* shaderPath, std::string& shaderCode)
{

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

void setShaders()
{
	char *vs = NULL, *fs = NULL;

	std::string vertShaderString; //empty string
	std::string fragShaderString; //empty string

	vsID = glCreateShader(GL_VERTEX_SHADER);
	fsID = glCreateShader(GL_FRAGMENT_SHADER);

	readShaderFile("vertexshader.txt", vertShaderString);
	readShaderFile("fragshader.txt", fragShaderString);

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

	// get uniform input 
	GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");

	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);


	glDeleteShader(vsID);
	glDeleteShader(fsID);
}

void renderScene() {
	glutSetWindow(mw);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void display2(void)
{
	glutSetWindow(sw2);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_SMOOTH); 
	/*
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
	}*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, znear, zfar);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMat1);
	
	cout << "ProjectionMat -- fixed" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << projectionMat1[i] << "  ";

	}
	cout << endl;

	cout << " znear : " << znear << endl;
	cout << " zfar : " << zfar << endl;

	/*Cx = cam.uvn[0][0] + cam.pos[0]; Cy = cam.uvn[0][1] + cam.pos[1]; Cz = cam.uvn[0][2] + cam.pos[2];
	Tx = -cam.uvn[2][0]; Ty = -cam.uvn[2][1]; Tz = -cam.uvn[2][2];
	Ux = cam.uvn[1][0]; Uy = cam.uvn[1][1]; Uz = cam.uvn[1][2];*/
	//Cx = pos.x; Cy = pos.y; Cz = pos.z;
	Cx = cam.pos[0]; Cy = cam.pos[1]; Cz = cam.pos[2];
	Tx = cam.dir[0]; Ty = cam.dir[1]; Tz = cam.dir[2];
	Ux = cam.uvn[1][0]; Uy = cam.uvn[1][1]; Uz = cam.uvn[1][2];
	/*cout << " Glu Look at values" << endl;
	cout << Cx << " " << Cy << " " << Cz << " " << Tx << " " << Ty << " " << Tz << " " << Ux << " " << Uy << " " << Uz << endl;
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Cx, Cy, Cz, Tx, Ty, Tz, Ux, Uy, Uz);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat1);

	cout << "Modelview --- fixed" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << modelViewMat1[i] << "  ";
	}
	cout << endl;

	//glScalef(Sx, Sy, Sz);
	triangle();
	glFlush();
	glutSwapBuffers();
}

void display1(void)
{
	//glutSetWindow(sw1);
	//glUseProgram(pID);
	////glUseProgram(0);

	//glValidateProgram(pID);
	//GLint validate = 0;
	//glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
	//GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");

	//glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	//glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex[0], GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLint), &face[0], GL_DYNAMIC_DRAW);

	//glBindVertexArray(VAO);
	//glDrawElements(rmode, face.size() * 3, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	//glutSwapBuffers();
	//display2();

	glutSetWindow(sw1);
	glUseProgram(pID);
	//glUseProgram(0);

	glValidateProgram(pID);
	GLint validate = 0;
	glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);
	GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");

	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLint), &face[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(VAO);
	glDrawElements(rmode, face.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
}

void initBufferObject(void)
{

//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//
//	// setup VAO
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLint), &face[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//
//	// Use depth buffering for hidden surface elimination
//	glEnable(GL_DEPTH_TEST);
//
//	// Setup the view of the cube
//	//glMatrixMode(GL_PROJECTION);
//	//gluPerspective(60.0, 1.0, 1.0, 10.0);
//
//	//glGetFloatv(GL_PROJECTION_MATRIX, projectionMat1);
//
//
//	//glMatrixMode(GL_MODELVIEW);
//	//gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
//	//	0.0, 0.0, 0.0,        /* center is at (0,0,0) */
//	//	0.0, 1.0, 0.);        /* up is in positive Y direction */
//
//	//						  // Adjust cube position
//
//	//glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat1);
//
//	ProjectionMatrix();
//	ModelViewMatrix();
//
//	/*cout << "Modelview --- fixed" << endl;
//	for (int i = 0; i < 16; i++)
//	{
//		cout << modelViewMat1[i] << endl;
//	}
//	cout << endl;*/
//
//	cout << "Modelview -- shader" << endl;
//	for (int i = 0; i < 16; i++)
//	{
//		cout << modelViewMat[i] << "  ";
//	}
//	cout << endl;
///*
//	cout << "ProjectionMat -- fixed" << endl;
//	for (int i = 0; i < 16; i++)
//	{
//		cout << projectionMat1[i] << "  ";
//	}
//	cout << endl;
//*/
//
//	cout << "ProjectionMat -- shader" << endl;
//	for (int i = 0; i < 16; i++)
//	{
//		cout << projectionMat[i] << "  ";
//	}
//	cout << endl;
//
//	GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
//	GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");
//
//	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
//	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() *sizeof(GLfloat), &vertex[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLint), &face[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// Use depth buffering for hidden surface elimination
	glEnable(GL_DEPTH_TEST);

	ProjectionMatrix();
	ModelViewMatrix();
}


void renderSceneAll() {

	// check for keyboard movement
	display1();
	display2();
}

void drawmesh()
{
	initBufferObject();
	renderSceneAll();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
}

void setCameraAxis(float *a, GLfloat angle)
{
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	float m = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] /= m;
	a[1] /= m;
	a[2] /= m;
	float x = a[0];
	float y = a[1];
	float z = a[2];
	/*cout << " x : " << x << endl;
	cout << " y : " << y << endl;
	cout << " z : " << z << endl;*/
	rotMat = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
		t*x*y + z*s, t*y*y + c, t*y*z - x*s,
		t*x*z - y*s, t*y*z + x*s, t*z*z + c);
}

void matMul()
{
	float t[3][3] = { 0 };
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			t[i][j] = rotMat[j][0] * cam.uvn[i][0] + rotMat[j][1] * cam.uvn[i][1] + rotMat[j][2] * cam.uvn[i][2];
		}
	}

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			cam.uvn[i][j] = t[i][j];
		}
	}

	cam.dir[0] = cam.pos[0] + cam.uvn[2][0] * vdist;
	cam.dir[1] = cam.pos[1] + cam.uvn[2][1] * vdist;
	cam.dir[2] = cam.pos[2] + cam.uvn[2][2] * vdist;
}

void translate(int a, float len)
{
	for (int i = 0; i < 3; i++)
	{
		cam.pos[i] += cam.uvn[a][i] * len;
		cam.dir[i] += cam.uvn[a][i] * len;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	char fname[30];
	float len = 0.2;
	bool pause = false;
	if (key == 'd')
	{
		translate(0, len);
	
	}
	if (key == 'a')
	{
		translate(0, -len);

	}
	if (key == 'w')
	{
		translate(1, len);
	}
	if (key == 's')
	{
		translate(1, -len);
	}
	if (key == 'x')
	{
		translate(2, len);
	}
	if (key == 'z')
	{
		translate(2, -len);
	}

	if (key == 'q' || key == 'Q') exit(0);
	if (key == 'c')
		rot = 1;
	if (key == 'v')
		rot = 2;

	if (key == 'p')
	{
		znear += 0.5;
		if (zfar < znear)
			zfar = znear;
		drawmesh();
	}
	if (key == 'o')
	{

		znear -= 0.5;
		if (znear < 1.0)
			znear = 1.0;
		drawmesh();
	}
	if (key == 'i')
	{
		zfar += 0.5;
		drawmesh();
	}
	if (key == 'u')
	{
		zfar -= 0.5;
		if (zfar < znear)
			zfar = znear;
		drawmesh();
	}
	if (key == 'n')
	{
		cout << "New File name : ";
		cin >> fname;
		clearModel();
		loadModel(fname);
		drawmesh();
	}
	if (key == 49)
	{
		mode = 1;
		rmode = GL_POLYGON;
	}
	if (key == 50)
	{
		mode = 2;
		rmode = GL_POINTS;
	}
	if (key == 51)
	{
		mode = 3;
		rmode = GL_LINE_STRIP;
	}

	if (key == 27)
	{
		resetCamera();
		drawmesh();
	}
	if (key == 'r') cl = 0;
	if (key == 'g') cl = 1;
	if (key == 'b') cl = 2;
	if (key == 'm')
	{
		setCameraAxis(cam.uvn[2], angle);
		matMul();
		drawmesh();
	}
	if (key == 'l')
	{
		setCameraAxis(cam.uvn[2], -angle);
		matMul();
		drawmesh();
	}

	/*if (stop)
	glutIdleFunc(NULL);
	else
	{
	glutIdleFunc(drawmesh);
	}*/
	updatecolor();
	drawmesh();
}


void SpecialKeys(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_LEFT:
		setCameraAxis(cam.uvn[1], angle);
		matMul();
		drawmesh();
		break;
	case GLUT_KEY_RIGHT:
		setCameraAxis(cam.uvn[1], -angle);
		matMul();
		drawmesh();
		break;
	case GLUT_KEY_UP:
		setCameraAxis(cam.uvn[0], angle);
		matMul();
		drawmesh();
		break;
	case GLUT_KEY_DOWN:
		setCameraAxis(cam.uvn[0], -angle);
		matMul();
		drawmesh();
		break;
	}
}

void init()
{
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
	resetCamera();
	glutInit(&argc, argv);
	loadModel("cactus.obj");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	resetCamera();
	mw = glutCreateWindow("Shader Pipeline Window                                                                                             |                                                         Fixed Pipeline Window");
	glutDisplayFunc(renderScene);
	//init();

	sw1 = glutCreateSubWindow(mw, 10,10,500,500);
	glutDisplayFunc(display1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glewInit();
	initBufferObject();
	setShaders();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);

	sw2 = glutCreateSubWindow(mw, 510, 10, 500, 500);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display2);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(500, 500);
	//sw1 = glutCreateWindow("Shader Pipeline Window");
	//glewInit();
	//initBufferObject();
	//setShaders();
	//glutDisplayFunc(display1);
	//
	//glutInitWindowSize(500, 500);
	//sw2 = glutCreateWindow("Fixed Pipeline Window");
	//glutReshapeFunc(myReshape);
	//glutDisplayFunc(display2);
	////glutIdleFunc(drawmesh);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(SpecialKeys);
	//glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}