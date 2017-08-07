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


GLhandleARB r, f, p;
GLhandleARB vsID, fsID, pID, VBO, VAO, EBO;
int af, ar;
float nearvalue = 0.0f;
float farvalue = 4.0f;
float nearvaluer = 1.0f;
float farvaluer = 100.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float anglerotate = 0.0;

glm::vec3 u(1, 0, 0);
glm::vec3 v(0, 1, 0);
glm::vec3 n(0, 0, 1);

glm::mat4 uvn;
float upx = 0.0f, upy = 1.0f, upz = 0.0f;
int polygon = 1;
int wireframe = 0;
int points = 0;
int clockwise = 0;
int counterclockwise = 1;
glm::vec3 camerapositionshader(0.0f, 0.0f, 4.0f);
glm::vec3 dirv(0.0f, 0.0f, -1.0f);
glm::vec3 upvec(0.0f, -1.0f, 0.0f);
float fov = 45.0; // FOV angle
float top = 0;
float bottom = 0;
float rightp = 0;
float leftp = 0;
float aspect = 1;
float pivalue = 3.14;
glm::vec3 camerapositionshader(0.0f, 0.0f, 4.0f);
glm::vec3 dirv(0.0f, 0.0f, -1.0f);
glm::vec3 upvec(0.0f, -1.0f, 0.0f);
int clockwise = 0;
int counterclockwise = 1;

int mode = 1;

float colorvect[] = { 1.0, 0.0, 0.0 };
GLfloat nearVal = 0.0, farVal = 1.0;
GLfloat xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
GLfloat x[MX] = { 0 }, y[MX] = { 0 }, z[MX] = { 0 };
GLfloat dist;
GLint pts[PV][3] = { 0 };
int fCount = 0;
//GLhandleARB vsID, fsID, pID;
GLfloat fovy = 45, znear = 1.0, zfar = 100.0, aspect;
GLfloat projectionMat[16] = { 0 }, modelViewMat[16] = { 0 }, translatiomatrixMat[16] = { 0 };


class Camera {
public:
	vec3 pos = vec3(0, 0, 4);
	vec3 dir = vec3(0, 0, -1);
	vec3 up = vec3(0, -1, 0);
	vec3 rotate;
	double fov, pov;
};

Camera cam;



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
		pts[j][0] = atoi(l1);
		pts[j][1] = atoi(l2);
		pts[j][2] = atoi(l3);
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
	//pos.z = 2 * (zmax - zmin);
	//Tz = -1.0;
}
void draw(int i)
{
	//glColor3fv(color1[cl]);
	glVertex3d((x[pts[i][0] - 1]), (y[pts[i][0] - 1]), (z[pts[i][0] - 1]));
	glVertex3d((x[pts[i][1] - 1]), (y[pts[i][1] - 1]), (z[pts[i][1] - 1]));
	glVertex3d((x[pts[i][2] - 1]), (y[pts[i][2] - 1]), (z[pts[i][2] - 1]));
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
	}
}

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

	vsID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	fsID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	readShaderFile("vertshader.txt", vertShaderString);
	readShaderFile("fragmentshader.txt", fragShaderString);

	const GLchar * pVertShaderSource = vertShaderString.c_str();
	const GLchar * pFragShaderSource = fragShaderString.c_str();

	glShaderSourceARB(vsID, 1, &pVertShaderSource, NULL);
	glShaderSourceARB(fsID, 1, &pFragShaderSource, NULL);

	glCompileShaderARB(vsID);
	glCompileShaderARB(fsID);

	pID = glCreateProgramObjectARB();
	glAttachShader(pID, vsID);
	glAttachShader(pID, fsID);

	glLinkProgramARB(pID);
	glUseProgramObjectARB(pID);
}

void display()
{
	glUseProgram(pID);

	glValidateProgram(pID);
	GLint validate = 0;
	glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (counterclockwise == 1)
	{
		std::cout << "  cull face counterclockwise";

		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);


	}

	if (clockwise == 1)
	{
		std::cout << "  cull face clockwise";
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);


	}

	GLint colorlocation = glGetUniformLocation(pID, "color");
	glUniform4fv(colorlocation, 1, colorvect);

	ProjectionMatrix();
	ModelViewMatrix();

	float translationMat[16] = {
		1,0,0,-cam.pos.x,
		0,1,0,-cam.pos.y,
		0,0,1,-cam.pos.z,
		0,0,0,1
	};

	GLint modeViewMatLocation = glGetUniformLocation(pID, "model");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projection");
	GLint translationMatLocation = glGetUniformLocation(pID, "translation");
	glUniformMatrix4fv(modeViewMatLocation, 1, true, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, true, projectionMat);
	glUniformMatrix4fv(translationMatLocation, 1, true, translationMat);

	triangle();
	glFlush();
	glutSwapBuffers();
}

void drawmesh()
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	loadModel("bunny.obj");
	glutInitWindowSize(500, 500);

	glutCreateWindow("Shader Pipeline Window");
	glewInit();
	setShaders();

	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glutMainLoop();
	return 0;
}