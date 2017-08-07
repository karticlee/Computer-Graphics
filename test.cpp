#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <GLFW/glfw3.h>
# include "objectloadernew.h"
# include <stdlib.h>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
// for GLSL
GLhandleARB r, f, p;
GLhandleARB vsID, fsID, pID, VBO, VAO, EBO;
// windows parameter
int af, ar;

// for uniform input
GLfloat projectionMat[16] = { 0 }, modelViewMat[16] = { 0 }, translatiomatrixMat[16] = { 0 };
/// for fixed pipeline
// actual vector representing the camera's direction
// array for color for fragment shader
float colorvect[] = { 1.0, 0.0, 0.0 };

// XZ position of the camera
float nearvalue = 0.0f;
float farvalue = 4.0f;
float nearvaluer = 1.0f;
float farvaluer = 100.0f;

float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float anglerotate = 0.0;
// XZ position of the camera
float x = 0.0f, y = 0.0f, z = 0.0f;
//std::vector<glm::vec4> verticescamera(0.0f,0.0,0.0,0.0);
//verticescamera[0].i = 0.0f;
float tx = 0;
float ty = 0;
float tz = 0;
float tw = 0;
float minx = 0.0;
float maxx = 0.0;
float miny = 0.0;
float maxy = 0.0;
float minz = 0.0;
float maxz = 0.0;

float rotationangle = 0.0;
glm::vec3 u(1, 0, 0);
glm::vec3 v(0, 1, 0);
glm::vec3 n(0, 0, 1);

glm::mat4 uvn;


glm::vec3 rotationaxis(0, 0, 0);
float cameranglex = 0.0f;
float camerangley = 0.0f;
float cameraanglez = 0.0f;
glm::vec3 cameraposition(0.0f, 0.0f, 0.0f);
glm::vec3 camerapositionglu(0.0f, 0.0f, -1.0f);
glm::vec3 camerapositiontest(0.0f, 0.0f, 0.0f);
float nearvaluef = 4 * minz;
float farvaluef = 4 * maxz;
//glm::mat4 translationmatrix;

float upx = 0.0f, upy = 1.0f, upz = 0.0f;
int polygon = 1;
int wireframe = 0;
int points = 0;
int clockwise = 0;
int counterclockwise = 1;
float scalex = 1;
float scaley = 1;
float scalez = 1;
float zoomfraction = 0.5;
/*float lx = 0.0f, ly = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 1.0f, z = 5.0f;
float upx = 0.0f, upy = 1.0f, upz = 0.0f;*/
// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red = 1.0f, blue = 1.0f, green = 1.0f;

// angle for rotating triangle
float angle1 = 0.0f;
float angle2 = 0.0f;
//int dummy = 5;
glm::vec3 translation;
glm::vec3 camerapositionshader(0.0f, 0.0f, 4.0f);
glm::vec3 dirv(0.0f, 0.0f, -1.0f);
glm::vec3 upvec(0.0f, -1.0f, 0.0f);

bool stop = false;
std::vector<glm::vec3> vertices1;
std::string filename1 = "bunny.obj";
//std::vector<glm::vec2> uvs;
//std::vector<glm::vec3> normals; // Won't be used at the moment.
int res = loadOBJ(filename1, vertices1);
int b = vertices1.size();
// values for perspective projection matrix
float fov = 45.0; // FOV angle
float top = 0;
float bottom = 0;
float rightp = 0;
float leftp = 0;
float aspect = 0;
float pivalue = 3.14;

// values for orthographic projection matrix

float lefto = 4 * minx;
float righto = 4 * maxx;
float bottomo = 4 * miny;
float topo = 4 * maxy;

float nearvalo = 4 * minz + nearvalue;
float farvalo = 4 * maxz + farvalue;




//float nearval = 0;
//float farval = 0;

int numberfaces = res;

// Model view matrix



/// second window fixed opengl
void triangle(int j)
{

	//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	//glBegin(GL_TRIANGLE_STRIP);
	//glColor3fv(colors[0]);


	/*std::cout << "the value of minx" << minx;
	std::cout << "the value of minx" << maxx;
	std::cout << "the value of minx" << miny;
	std::cout << "the value of minx" << maxy;
	std::cout << "the value of minx" << minz;
	std::cout << "the value of minx" << maxz;*/


	if (polygon == 1)
	{


		for (int i = 0; i < j - 3; i = i + 3)
		{


			glBegin(GL_POLYGON);
			glColor3f(red, blue, green);
			glVertex3f(vertices1[i].x, vertices1[i].y, vertices1[i].z);


			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 1].x, vertices1[i + 1].y, vertices1[i + 1].z);


			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 2].x, vertices1[i + 2].y, vertices1[i + 2].z);

			glColor3f(red, blue, green);
			glEnd();
			/*std::cout << "the value of minx" << minx;
			std::cout << "the value of minx" << maxx;
			std::cout << "the value of minx" << miny;
			std::cout << "the value of minx" << maxy;
			std::cout << "the value of minx" << minz;
			std::cout << "the value of minx" << maxz;*/

			////std::cout << "the value of vertex x coordinate" << vertices1[i].x;
			//triangle(i);
		}
	}

	if (points == 1)
	{

		for (int i = 0; i < j - 3; i = i + 3)
		{


			glBegin(GL_POINTS);
			glColor3f(red, blue, green);
			glVertex3f(vertices1[i].x, vertices1[i].y, vertices1[i].z);


			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 1].x, vertices1[i + 1].y, vertices1[i + 1].z);


			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 2].x, vertices1[i + 2].y, vertices1[i + 2].z);

			glColor3f(red, blue, green);
			glEnd();

			////std::cout << "the value of vertex x coordinate" << vertices1[i].x;
			//triangle(i);
		}
	}
	if (wireframe == 1)
	{


		for (int i = 0; i < j - 3; i = i + 3)
		{

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
			glColor3f(red, blue, green);
			glVertex3f(vertices1[i].x, vertices1[i].y, vertices1[i].z);

			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 1].x, vertices1[i + 1].y, vertices1[i + 1].z);

			glColor3f(red, blue, green);
			glVertex3f(vertices1[i + 2].x, vertices1[i + 2].y, vertices1[i + 2].z);

			glColor3f(red, blue, green);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			////std::cout << "the value of vertex x coordinate" << vertices1[i].x;
			//triangle(i);
		}
	}
	//glColor3fv(colors[3]);
	//glVertex3f(vertices1[j+1].x, vertices1[j+1].y, vertices1[j+1].z);
	//glColor3fv(colors[3]);
	//glVertex3f(vertices1[j+2].x, vertices1[j+2].y, vertices1[j+2].z);


	//glEnd();
}
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
void changeSize(int w, int h)
{


	for (int i = 0; i < b - 3; i = i + 3)
	{
		if (vertices1[i + 1].x <= minx)
		{
			minx = vertices1[i + 1].x;
		}
		if (vertices1[i + 1].x >= maxx)
		{
			maxx = vertices1[i + 1].x;
		}
		if (vertices1[i + 1].y <= miny)
		{
			miny = vertices1[i + 1].y;
		}
		if (vertices1[i + 1].y >= maxy)
		{
			maxy = vertices1[i + 1].y;
		}
		if (vertices1[i + 1].z <= minz)
		{
			minz = vertices1[i + 1].z;
		}
		if (vertices1[i + 1].z >= maxz)
		{
			maxz = vertices1[i + 1].z;
		}

		if (vertices1[i + 1].x <= minx)
		{
			minx = vertices1[i + 1].x;
		}
		if (vertices1[i + 1].x >= maxx)
		{
			maxx = vertices1[i + 1].x;
		}
		if (vertices1[i + 1].y <= miny)
		{
			miny = vertices1[i + 1].y;
		}
		if (vertices1[i + 1].y >= maxy)
		{
			maxy = vertices1[i + 1].y;
		}
		if (vertices1[i + 1].z <= minz)
		{
			minz = vertices1[i + 1].z;
		}
		if (vertices1[i + 1].z >= maxz)
		{
			maxz = vertices1[i + 1].z;
		}
		if (vertices1[i + 2].x <= minx)
		{
			minx = vertices1[i + 2].x;
		}
		if (vertices1[i + 2].x >= maxx)
		{
			maxx = vertices1[i + 2].x;
		}
		if (vertices1[i + 2].y <= miny)
		{
			miny = vertices1[i + 2].y;
		}
		if (vertices1[i + 2].y >= maxy)
		{
			maxy = vertices1[i + 2].y;
		}
		if (vertices1[i + 2].z <= minz)
		{
			minz = vertices1[i + 2].z;
		}
		if (vertices1[i + 2].z >= maxz)
		{
			maxz = vertices1[i + 2].z;
		}



	}

	float distancez = maxz - minz;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	aspect = ratio;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();


	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	/*if (w <= h)
	glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
	2.0 * (GLfloat)h / (GLfloat)w, nearvalue, farvalue);
	else
	//std::cout << "nearvalue" << nearvalue;
	//std::cout << "farvalue" << farvalue;
	glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
	2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, nearvalue, farvalue);*/
	std::cout << "the value of minx" << minx;
	std::cout << "the value of maxx" << maxx;
	std::cout << "the value of miny" << miny;
	std::cout << "the value of maxy" << maxy;
	std::cout << "the value of minz" << minz;
	std::cout << "the value of minz" << maxz;
	nearvaluef = 4 * minz + nearvalue;
	farvaluef = 4 * maxz + farvalue;
	//working ortho
	//if (w <= h)
	//
	glOrtho(4 * minx, 4 * maxx, 4 * miny * (GLfloat)h / (GLfloat)w,
		//
		4 * maxy * (GLfloat)h / (GLfloat)w, 4 * minz + nearvaluef, 4 * maxz + farvaluef);
	//else
	//
	//std::cout << "nearvalue" << nearvalue;
	//
	//std::cout << "farvalue" << farvalue;  
	//
	glOrtho(4 * minx * (GLfloat)w / (GLfloat)h,
		//
		4 * maxx * (GLfloat)w / (GLfloat)h, 4 * miny, 4 * maxy, 4 * minz + nearvaluef, 4 * maxz + farvaluef);
	//Above working ortho

	gluPerspective(60.0f, ratio, minz, maxz);
	// Set the correct perspective.
	//gluPerspective(45.0f, ratio, 0.1f, 50.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
/*void colorcube()
{
for (int i =0;i<numberfaces;i=i+1)
{
//std::cout << "the value of i" << i;
triangle(b);
std::cout << "the size of vertex" << b;
}




}*/


//static GLfloat theta[] = { 0.0, 0.0, 0.0 };
//static GLint axis1 = 2;




void nearfarvalue()
{
	int w1 = glutGet(GLUT_WINDOW_WIDTH);
	int h1 = glutGet(GLUT_WINDOW_HEIGHT);
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h1 == 0)
		h1 = 1;
	float ratio1 = w1 * 1.0 / h1;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();


	// Set the viewport to be the entire window
	glViewport(0, 0, w1, h1);
	/*if (w1 <= h1)
	glOrtho(4 * minx, 4 * maxx, 4 * miny * (GLfloat)h1 / (GLfloat)w1,
	4 * maxy * (GLfloat)h1 / (GLfloat)w1, 4 * minz + nearvalue, 4 * maxz + farvalue);
	else

	glOrtho(4 * minx * (GLfloat)w1 / (GLfloat)h1,
	4 * maxx * (GLfloat)w1 / (GLfloat)h1, 4 * miny, 4 * maxy, 4 * minz + nearvalue, 4 * maxz + farvalue);*/
	gluPerspective(60.0f, ratio1, minz, maxz);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
void modelviewmatrix()
{
	glm::vec3 nr(dirv.x, dirv.y, dirv.z);

	nr = normalize(nr);
	glm::vec3 ur(upvec.x, upvec.y, upvec.z);
	glm::vec3 vr;
	vr = cross(ur, nr);
	vr = normalize(vr);
	ur = cross(vr, nr);
	modelViewMat[0] = vr.x;
	modelViewMat[1] = vr.y;
	modelViewMat[2] = vr.z;
	modelViewMat[4] = ur.x;
	modelViewMat[5] = ur.y;
	modelViewMat[6] = ur.z;
	modelViewMat[8] = -nr.x;
	modelViewMat[9] = -nr.y;
	modelViewMat[10] = -nr.z;
	modelViewMat[15] = 1;





}
// projection matrix
void projectionmatrix()
{
	// perspective projection matrix
	float top = nearvaluer*tan((pivalue / 180)*(fov / 2));
	float bottom = -top;
	float rightp = top*aspect;
	float leftp = -rightp;
	cout << "aspect " << aspect << endl;
	cout << "top " << top << endl;
	cout << "bottom " << bottom << endl;
	cout << "right " << rightp << endl;
	cout << "left " << leftp << endl;
	cout << "nearvalue " << nearvaluer << endl;
	cout << "farvalue " << farvaluer << endl;

	projectionMat[0] = 2 * ((nearvaluer) / (rightp - leftp));
	projectionMat[2] = (rightp + leftp) / (rightp - leftp);
	projectionMat[5] = 2 * ((nearvaluer) / (top - bottom));
	projectionMat[6] = (top + bottom) / (top - bottom);
	projectionMat[10] = -((farvaluer + nearvaluer) / (farvaluer - nearvaluer));
	projectionMat[11] = -2 * ((farvaluer*nearvaluer) / (farvaluer - nearvaluer));
	projectionMat[14] = -1;

	// Orthgraphic projection matrix
	/*lefto = 4*minx;
	righto = 4*maxx;
	bottomo = 4*miny;
	topo = 4*maxy;
	nearvalo = 4*minz+nearvalue;
	farvalo = 4*maxz+farvalue;
	std::cout << "the value of minx" << minx << endl;
	std::cout << "the value of maxx" << maxx << endl;
	std::cout << "the value of miny" << miny << endl;
	std::cout << "the value of maxy" << maxy << endl;
	std::cout << "the value of minz" << minz << endl;
	std::cout << "the value of minz" << maxz << endl;
	cout << "righto value" << righto << endl << endl;
	cout << "leftoto value" << lefto << endl;
	cout << "bottomo value" << bottomo << endl;
	cout << "topo value" << topo << endl;
	cout << "nearvalo" << nearvalo << endl;
	cout << "farvalo" << farvalo << endl;

	projectionMat[0] = 2 / (righto - lefto);
	projectionMat[3] = -((righto + lefto) / (righto - lefto));
	projectionMat[5] = 2 / (topo - bottomo);
	projectionMat[7] = -((topo + bottomo) / (topo - bottomo));
	projectionMat[10] = -2/ (farvalo - nearvalo);
	projectionMat[11] = - ((farvalo+nearvalo) / (farvalo - nearvalo));
	projectionMat[15] = 1;*/









}
// translation
void Translationmatrix()
{

	translatiomatrixMat[0] = 1;
	translatiomatrixMat[3] = -camerapositionshader.x;
	translatiomatrixMat[5] = 1;
	translatiomatrixMat[7] = -camerapositionshader.y;
	translatiomatrixMat[10] = 1;
	translatiomatrixMat[11] = -camerapositionshader.z;
	translatiomatrixMat[15] = 1;

}

void readShaderFile(const GLchar* shaderPath, std::string& shaderCode)
{

	/*std::ifstream shaderFile;

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
	*/
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
	/*char *vs = NULL, *fs = NULL;

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

	glDeleteShader(vsID);
	glDeleteShader(fsID);
	*/
	char *vs = NULL, *fs = NULL;

	std::string vertShaderString; //empty string
	std::string fragShaderString; //empty string

	vsID = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	fsID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	readShaderFile("vertexshader.txt", vertShaderString);
	readShaderFile("fragshader.txt", fragShaderString);

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
void display1()
{
	glutSetWindow(ar);
	glUseProgram(pID);
	//glUseProgram(0);

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
	modelviewmatrix();
	Translationmatrix();
	projectionmatrix();

	GLint modeViewMatLocation = glGetUniformLocation(pID, "model");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projection");
	GLint translationMatLocation = glGetUniformLocation(pID, "translation");
	glUniformMatrix4fv(modeViewMatLocation, 1, true, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, true, projectionMat);
	glUniformMatrix4fv(translationMatLocation, 1, true, translatiomatrixMat);
	cout << "Modelview/n" << endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << modelViewMat[i] << endl;
	}
	std::cout << endl;
	std::cout << "ProjectionMat" << endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << projectionMat[i] << endl;
	}
	std::cout << endl;
	std::cout << "Translation" << endl;
	for (int i = 0; i < 16; i++)
	{
		std::cout << translatiomatrixMat[i] << endl;
	}


	triangle(b);

	//glBindVertexArray(VAO);
	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	glFlush();
	glutSwapBuffers();
}
void display()
{
	glutSetWindow(af);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);



	/*if (h == 0)
	h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();


	// Set the viewport to be the entire window*/
	/*glViewport(0, 0, w, h);
	if (w <= h)
	glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
	2.0 * (GLfloat)h / (GLfloat)w, nearvalue, farvalue);
	else
	std::cout << "nearvalue" << nearvalue;
	std::cout << "farvalue" << farvalue;
	glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
	2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, nearvalue, farvalue);*/

	// Set the viewport to be the entire window


	// Set the correct perspective.
	//gluPerspective(45.0f, ratio, 0.1f, 50.0f);

	// Get Back to the Modelview
	//glMatrixMode(GL_MODELVIEW);
	/*glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 1.0, 0.0, 1.0);*/
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
	glMatrixMode(GL_MODELVIEW);
	// Reset transformations
	glLoadIdentity();
	camerapositionglu.x = cameraposition.x;
	camerapositionglu.y = cameraposition.y;
	camerapositionglu.z = 2 * (maxz - minz) + cameraposition.z;
	// Set the camera
	//glScalef(2.0, 2.0, 2.0);
	gluLookAt(camerapositionglu.x, camerapositionglu.y, camerapositionglu.z,
		camerapositionglu.x + lx, camerapositionglu.y + ly, camerapositionglu.z + lz,
		upx, upy, upz);

	glColor3f(0.9f, 0.9f, 0.9f);




	triangle(b);
	//std::cout << "the size of vertex" << b;

	glFlush();
	glutSwapBuffers();
	display1();
}

void processNormalKeys(unsigned char key, int x, int y)
{

	if (key == 27)
		exit(0);

	if (key == 'w')
	{
		angle1 = angle1 + 0.25;
		angle2 = angle2 + 0.1;
		std::cout << "angle1" << angle1;
		//std::cout << "angle2" << angle2;
		upz = -cos(angle1);
		std::cout << "upz" << upz;
		upx = sin(angle1);
		std::cout << "upx" << upx;
		std::cout << "size of vertices cactus" << b;
	}
	if (key == 's')
	{
		angle1 = angle1 - 0.25;
		upz = (-cos(angle1));
		upx = sin(angle1);
	}
	if (key == 'n')
	{
		//std::cout << "nearvalue" << nearvalue;
		//std::cout << "farvalue" << farvalue;
		nearvalue = nearvalue + 1.0f;
		nearfarvalue();
	}
	if (key == 'f')
	{
		farvalue = farvalue + 1.0f;
		//std::cout << "nearvalue" << nearvalue;
		//std::cout << "farvalue" << farvalue;
		//std::cout << "nearvalue" << nearvalue;
		//std::cout << "farvalue" << farvalue;
		nearfarvalue();
	}
	if (key == 'a')
	{
		nearvalue = nearvalue - 1.0f;
		nearfarvalue();
	}
	if (key == 'q')
	{
		farvalue = farvalue - 1.0f;
		nearfarvalue();
	}
	if (key == 'r')
	{
		red = 1.0f;
		blue = 0.0f;
		green = 0.0f;
		colorvect[0] = 1.0f;
		colorvect[1] = 0.0f;
		colorvect[2] = 0.0f;
	}
	if (key == 'g')
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
		colorvect[0] = 0.0f;
		colorvect[1] = 0.0f;
		colorvect[2] = 1.0f;
	}
	if (key == 'b')
	{
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
		colorvect[0] = 0.0f;
		colorvect[1] = 1.0f;
		colorvect[2] = 0.0f;
	}

	if (key == 'x')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle - 0.5;

		v = normalize(v);
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = rotationmatrix*u;
		v = rotationaxis*v;
		n = rotationmatrix*n;
		float distanceze = maxz - minz;
		lx = n.x*distanceze;
		ly = n.y*distanceze;
		lz = n.z*distanceze;
	}
	if (key == 'y')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle + 0.5;

		n = normalize(n);
		float x = n.x;
		float y = n.y;
		float z = n.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = rotationmatrix*u;
		v = rotationaxis*v;
		n = rotationmatrix*n;
		float distanceze = maxz - minz;
		lx = n.x*distanceze;
		ly = n.y*distanceze;
		lz = n.z*distanceze;
	}
	if (key == 'z')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle - 0.5;

		n = normalize(n);
		float x = n.x;
		float y = n.y;
		float z = n.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = rotationmatrix*u;
		v = rotationaxis*v;
		n = rotationmatrix*n;
		float distanceze = maxz - minz;
		lx = n.x*distanceze;
		ly = n.y*distanceze;
		lz = n.z*distanceze;
	}
	if (key == 'i')
	{
		polygon = 1;
		wireframe = 0;
		points = 0;
		glutPostRedisplay();
	}
	if (key == 'o')
	{
		polygon = 0;
		wireframe = 0;
		points = 1;
		glutPostRedisplay();
	}
	if (key == 'p')
	{
		polygon = 0;
		wireframe = 1;
		points = 0;
		glutPostRedisplay();
	}
	/*if (key == 'd')
	{
	anglerotate = anglerotate - 0.1;
	lx = sin(anglerotate);
	lz = -cos(anglerotate);

	}
	if (key == 'e')
	{
	anglerotate = anglerotate + 0.1;
	lx = sin(anglerotate);
	lz = -cos(anglerotate);
	}*/
	if (key == 'v')
	{
		anglerotate = anglerotate - 0.1;
		ly = sin(anglerotate);
		lz = -cos(anglerotate);
	}
	if (key == 'c')
	{
		anglerotate = anglerotate + 0.1;
		ly = sin(anglerotate);
		lz = -cos(anglerotate);
	}
	//if (key == 'k')
	//{
	//
	anglerotate = anglerotate - 0.1;
	//
	lx = sin(anglerotate);
	//
	ly = -cos(anglerotate);
	//}
	if (key == 'l')
	{
		anglerotate = anglerotate + 0.1;
		lx = sin(anglerotate);
		ly = -cos(anglerotate);
	}
	if (key == '1')
	{
		counterclockwise = 1;
		clockwise = 0;
		//glFrontFace(GL_CCW);






		std::cout << " counter clock wise";


		glutPostRedisplay();
	}
	if (key == '2')
	{
		counterclockwise = 0;
		clockwise = 1;
		//glFrontFace(GL_CW);
		std::cout << "  clock wise";
		glutPostRedisplay();

	}
	if (key == '3')
	{
		// for fixed pipeline
		translation.x = translation.x + 0.1;
		translation.y = 0;
		//translation.z = 0;
		translation.z = translation.z + 0.1;
		cameraposition = cameraposition + u*translation.x;
		cameraposition = cameraposition + n*translation.z;
		translation.z = 0;
		translation.x = 0;

		// for shader
		camerapositionshader.x = camerapositionshader.x + 0.1;

	}
	if (key == '4')
	{

		translation.x = translation.x - 0.1;
		translation.y = 0;
		//translation.z = 0;
		translation.z = translation.z - 0.1;
		cameraposition = cameraposition + u*translation.x;
		cameraposition = cameraposition + n*translation.z;
		translation.z = 0;
		translation.x = 0;


		// for shader
		camerapositionshader.x = camerapositionshader.x - 0.1;
	}
	if (key == '5')
	{
		// for fixed pipeline
		translation.y = translation.y + 0.1;
		translation.z = translation.z + 0.1;
		translation.x = 0;
		//translation.z = 0;
		cameraposition = cameraposition + v*translation.y;
		cameraposition = cameraposition + n*translation.z;
		translation.z = 0;
		translation.y = 0;



		// for shader
		camerapositionshader.y = camerapositionshader.y + 0.1;
	}
	if (key == '6')
	{
		// for fixed
		translation.y = translation.y - 0.1;
		translation.z = translation.z - 0.1;
		translation.x = 0;
		//translation.z = 0;
		cameraposition = cameraposition + v*translation.y;
		cameraposition = cameraposition + n*translation.z;
		translation.z = 0;
		translation.y = 0;
		// for shader
		camerapositionshader.y = camerapositionshader.y - 0.1;

	}

	if (key == '7')
	{

		// for fixed pipeline
		translation.z = translation.z + 1.0;
		translation.x = translation.x + 1.0;
		translation.y = translation.y + 1.0;
		std::cout << " translation z value" << translation.z;
		//translation.x = 0;
		//translation.y = 0;
		cameraposition = cameraposition + n*translation.z;
		cameraposition = cameraposition + v*translation.y;
		cameraposition = cameraposition + u*translation.x;
		translation.x = 0;
		translation.y = 0;
		translation.z = 0;

		// for shader
		camerapositionshader.z = camerapositionshader.z + 0.1;

	}
	if (key == '8')
	{
		// for fixed pipeline
		translation.z = translation.z - 1.0;
		translation.x = translation.x - 1.0;
		translation.y = translation.y - 1.0;
		std::cout << " translation z value" << translation.z;
		//translation.x = 0;
		//translation.y = 0;
		cameraposition = cameraposition + n*translation.z;
		cameraposition = cameraposition + v*translation.y;
		cameraposition = cameraposition + u*translation.x;
		translation.x = 0;
		translation.y = 0;
		translation.z = 0;
		// for shader
		camerapositionshader.z = camerapositionshader.z - 0.1;



	}
	if (key == '9')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle + 0.1;

		u = normalize(u);
		float x = u.x;
		float y = u.y;
		float z = u.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = u*rotationmatrix;
		v = v*rotationaxis;
		n = n*rotationmatrix;
		float distanceze = maxz - minz;
		lx = n.x*distanceze;
		ly = n.y*distanceze;
		lz = n.z*distanceze;


	}
	if (key == 'd')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle - 0.5;

		u = normalize(u);
		float x = u.x;
		float y = u.y;
		float z = u.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = u* rotationmatrix;
		v = v* rotationmatrix;
		n = n*rotationmatrix;
		float distanceze = maxz - minz;
		lx = n.x*distanceze;
		ly = n.y*distanceze;
		lz = n.z*distanceze;

	}

	if (key == 'e')
	{
		for (int i = 0; i < b - 3; i = i + 3)
		{


			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 1].z <= minz)
			{
				minz = vertices1[i + 1].z;
			}
			if (vertices1[i + 1].z >= maxz)
			{
				maxz = vertices1[i + 1].z;
			}



			if (vertices1[i + 2].z <= minz)
			{
				minz = vertices1[i + 2].z;
			}
			if (vertices1[i + 2].z >= maxz)
			{
				maxz = vertices1[i + 2].z;
			}



		}
		rotationangle = rotationangle + 0.5;

		v = normalize(v);
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float c = cos(rotationangle);
		float s = sin(rotationangle);
		float t = 1.0 - c;
		mat3 rotationmatrix = mat3(t*x*x + c, t*x*y - z*s, t*x*z + y*s,
			t*x*y + z*s, t*y*y + c, t*y*z - x*s,
			t*x*z - y*s, t*y*z + x*s, t*z*z + c);

		u = rotationmatrix*u;
		v = rotationaxis*v;
		n = rotationmatrix*n;
		float distanceze = maxz - minz;
		lx = v.x*distanceze;
		ly = v.y*distanceze;
		lz = v.z*distanceze;


	}
	if (key == '`')
	{
		rotationaxis.y = 1;
		camerangley = camerangley - 0.5f;
		cameranglex = 0.0f;
		cameraanglez = 0.0f;


		mat4 rotation;
		rotation = glm::rotate(camerangley, rotationaxis);
		//cameraposition = (rotation)* cameraposition;
		camerangley = 0.0f;
		rotationaxis.y = 0;

	}


	if (key == 't')
	{
		std::string filename;
		//char filename[80];
		//std::cin.getline(filename, sizeof(filename));
		//int res2;
		std::cout << "Please enter the name of the model file";
		std::cin >> filename;
		std::cout << "filename " << filename;
		vertices1.clear();
		res = loadOBJ(filename, vertices1);
		b = vertices1.size();
		std::cout << "size of vertices cactus" << b;
	}
	if (key == 'k')
	{
		cameraposition.x = 0.0f;
		cameraposition.y = 0.0f;
		cameraposition.z = 0.0f;
		camerapositionshader.x = 0.0f;
		camerapositionshader.y = 0.0f;
		camerapositionshader.z = 4.0f;
		dirv.x = 0.0f;
		dirv.y = 0.0f;
		dirv.z = -1.0f;
		upvec.x = 0.0f;
		upvec.y = -1.0f;
		upvec.z = 0.0f;
		// values for perspective projection matrix
		fov = 45.0; // FOV angle
		top = 0;
		bottom = 0;
		rightp = 0;
		leftp = 0;
		aspect = 0;
		pivalue = 3.14;

		// values for orthographic projection matrix

		lefto = 4 * minx;
		righto = 4 * maxx;
		bottomo = 4 * miny;
		topo = 4 * maxy;

		nearvalo = 4 * minz + nearvalue;
		farvalo = 4 * maxz + farvalue;


		lx = 0.0f, ly = 0.0f, lz = -1.0f;
		// XZ position of the camera
		x = 0.0f, y = 0.0f, z = 0.0f;
		upx = 0.0f, upy = 1.0f, upz = 0.0f;
		std::cout << " reset to the original position";
		nearvalue = 0.0f;
		farvalue = 0.0f;
		nearfarvalue();
	}




}

void processSpecialKeys(int key, int xx, int yy)
{

	float fraction = 0.1f;
	float radius = 10.0f;
	switch (key) {
	case GLUT_KEY_LEFT:
		//angle1 -= 0.1f;
		//lx =lx+ sin(angle1);
		//lz = lz+ (-cos(angle1));
		x = x + fraction;
		z = z + fraction;
		//lx = lx + fraction;
		//lz = lz + fraction;
		break;
	case GLUT_KEY_RIGHT:
		x = x - fraction;
		z = z - fraction;
		//lx = lx - fraction;
		//lz = lz - fraction;

		break;
	case GLUT_KEY_UP:
		y = y + fraction;
		z = z + fraction;
		//ly = ly + fraction;
		//lz = lz + fraction;
		break;
	case GLUT_KEY_DOWN:
		y = y - fraction;
		z = z - fraction;
		//ly = ly - fraction;
		//lz = lz - fraction;
		break;
	case GLUT_KEY_CTRL_L:
		upz = upz + (-cos(angle1));
		upx = upx + sin(angle1);
	}
}








int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);



	//glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	ar = glutCreateWindow("Shader-Pipeline");
	glewInit();
	glutReshapeFunc(changeSize);
	setShaders();

	glutDisplayFunc(display1);

	// Fixed Pipeline window
	//glutInitWindowSize(500, 500);
	//glutInitWindowSize(500, 500);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	glutInitWindowPosition(600, 100);
	glutInitWindowSize(500, 500);
	//glutPositionWindow(500, 500);
	af = glutCreateWindow("Fixed-Pipeline");
	//glutPositionWindow(500, 500);
	glutDisplayFunc(display);

	glutIdleFunc(display);
	//glutIdleFunc(spinCube);
	//glutMouseFunc(mouse);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glutReshapeFunc(changeSize);
	// enter GLUT event processing cycle
	glutMainLoop();


	return 1;
}

