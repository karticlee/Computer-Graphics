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

using namespace std;
using namespace glm;

GLuint vsID, fsID, pID, VBO, VAO, EBO;
int fixWin, shWin;
char vertex[10];
int wid, ht;
char xv[20], yv[20], zv[20];
float angle = 0.01;
int fc, cmode = 0, flag = 1, cw = 1;
GLfloat colors[][3] = { { 1.0, 1.0, 1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0 } };
float dist = -5.0;
GLfloat ex = 0.0, ey = 0.0, ez = 0.0;
GLfloat Ux = 0.0, Uy = 1.0, Uz = 0.0;
GLfloat Lx = 0.0, Ly = 0.0, Lz = -1.0;
GLfloat rM[3][3];
int mode = GL_POINTS;
GLfloat projectionMat[16] = { 0 }, modelViewMat[16] = { 0 };
vector<GLfloat> xyz;
vector<GLint> face;
//Camera Coordinates
float pos[3] = { 0,0,5 };
float dir[3] = { 0,0,0 };
float axis[3][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };


class persp
{
public:
	GLfloat fovy;
	GLfloat nearz;
	GLfloat farz;
	GLfloat aspectratio;
};

persp p;

void setPersp()
{
	p.fovy = 60.0f;
	p.nearz = 1.0f;
	p.farz = 10.0f;
	p.aspectratio = 1.0f;
}

void construct_model(char modelname[])
{
	int i = 0;
	int j = 0;
	vector<GLfloat> temp;
	vector<GLint> tface;
	FILE *pFile;
	pFile = fopen(modelname, "r");
	char l1[20], l2[20], l3[20], h[10];
	

	while (fscanf(pFile, "%s", vertex) != EOF && strcmp(vertex, "v") == 0)
	{
		fscanf(pFile, "%s %s %s", l1, l2, l3);
		temp.push_back(atof(l1));
		temp.push_back(atof(l2));
		temp.push_back(atof(l3));
		temp.push_back(colors[cmode][0]);
		temp.push_back(colors[cmode][1]);
		temp.push_back(colors[cmode][2]);
		
	}
	while (strcmp(vertex, "f") == 0)
	{
		fscanf(pFile, "%s %s %s", l1, l2, l3);
		tface.push_back(atoi(l1));
		tface.push_back(atoi(l2));
		tface.push_back(atoi(l3));
		if (fscanf(pFile, "%s", vertex) == EOF)
		{
			break;
		}

	}
	fclose(pFile);
	xyz.clear();
	face.clear();
	xyz = temp;
	face = tface;
}



void polygon(int a)
{
	int p = (a)* 6;
	GLfloat temp[][3] = { { xyz[p++], xyz[p++], xyz[p] } };
	glColor3fv(colors[cmode]);
	glVertex3fv(temp[0]);

}

void colorcube()
{
	for (unsigned int i = 0; i < face.size(); i++)
	{
		glBegin(mode);
		polygon(face[i++] - 1);
		polygon(face[i++] - 1);
		polygon(face[i] - 1);
		glEnd();

	}
}

void modifycolor()
{
	for (unsigned int i = 0; i < xyz.size(); i = i + 6)
	{
		xyz[i + 3] = colors[cmode][0];
		xyz[i + 4] = colors[cmode][1];
		xyz[i + 5] = colors[cmode][2];

	}
}

void projection()
{
	float top, bottom, left, right;
	float x, y;
	float t1, t2, t3, t4;
	p.aspectratio = 1;
	y = p.nearz * tanf(3.14 * p.fovy / 360);
	x = y * p.aspectratio;
	left = -x;
	right = x;
	bottom = -y;
	top = y;
	t1 = 2.0 * p.nearz;
	t2 = right - left;
	t3 = top - bottom;
	t4 = p.farz - p.nearz;

	projectionMat[0] = t1 / t2;
	projectionMat[1] = 0.0;
	projectionMat[2] = 0.0;
	projectionMat[3] = 0.0;
	projectionMat[4] = 0.0;
	projectionMat[5] = t1 / t3;
	projectionMat[6] = 0.0;
	projectionMat[7] = 0.0;
	projectionMat[8] = 0.0;
	projectionMat[9] = 0.0;
	projectionMat[10] = (-p.farz - p.nearz) / t4;
	projectionMat[11] = -1.0;
	projectionMat[12] = 0.0;
	projectionMat[13] = 0.0;
	projectionMat[14] = (-t1 * p.farz) / t4;
	projectionMat[15] = 0.0;
}

void modelView()
{

	modelViewMat[0] = axis[0][0];
	modelViewMat[1] = axis[1][0];
	modelViewMat[2] = axis[2][0];
	modelViewMat[3] = 0.0;
	modelViewMat[4] = axis[0][1];
	modelViewMat[5] = axis[1][1];
	modelViewMat[6] = axis[2][1];
	modelViewMat[7] = 0.0;
	modelViewMat[8] = axis[0][2];
	modelViewMat[9] = axis[1][2];
	modelViewMat[10] = axis[2][2];
	modelViewMat[11] = 0.0;
	modelViewMat[12] = -(axis[0][0] * pos[0] + axis[0][1] * pos[1] + axis[0][2] * pos[2]);
	modelViewMat[13] = -(axis[1][0] * pos[0] + axis[1][1] * pos[1] + axis[1][2] * pos[2]);
	modelViewMat[14] = -(axis[2][0] * pos[0] + axis[2][1] * pos[1] + axis[2][2] * pos[2]);
	modelViewMat[15] = 1.0;
}

void rotationMatrix(float *a, GLfloat angle)
{
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	float mag = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / mag;
	a[1] = a[1] / mag;
	a[2] = a[2] / mag;
	float x = a[0];
	float y = a[1];
	float z = a[2];
	
	rM[0][0] = t * x * x + c;
	rM[0][1] = t * x * y - z * s;
	rM[0][2] = t * x * z + y * s;
	rM[1][0] = t * x * y + z * s;
	rM[1][1] = t * y * y + c;
	rM[1][2] = t * y * z - x * s;
	rM[2][0] = t * x * z - y * s;
	rM[2][1] = t * y * z + x * s;
	rM[2][2] = t * z * z + c;
}

void lookAt()
{
	float t[3][3] = { 0 };
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			t[i][j] = rM[j][0] * axis[i][0] + rM[j][1] * axis[i][1] + rM[j][2] * axis[i][2];
		}
	}

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			axis[i][j] = t[i][j];
		}
	}

	dir[0] = pos[0] + axis[2][0] * dist;
	dir[1] = pos[1] + axis[2][1] * dist;
	dir[2] = pos[2] + axis[2][2] * dist;
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

void fixedDisplay(void)
{
	float projectionMat1[16], modelViewMat1[16];
	glutSetWindow(fixWin);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, p.nearz, p.farz);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMat1);
	ex = pos[0]; ey = pos[1]; ez = pos[2];
	Lx = dir[0]; Ly = dir[1]; Lz = dir[2];
	Ux = axis[1][0]; Uy = axis[1][1]; Uz = axis[1][2];
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ex, ey, ez, Lx, Ly, Lz, Ux, Uy, Uz);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat1);
	colorcube();
	glFlush();
	glutSwapBuffers();
}

void shaderDisplay(void)
{
	glutSetWindow(shWin);
	glUseProgram(pID);
	glValidateProgram(pID);
	GLint validate = 0;
	glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);
	GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
	GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");

	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, xyz.size() * sizeof(GLfloat), &xyz[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLint), &face[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(VAO);
	glDrawElements(mode, face.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glutSwapBuffers();
}

void initBufferObject(void)
{

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, xyz.size() *sizeof(GLfloat), &xyz[0], GL_DYNAMIC_DRAW);

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

	projection();
	modelView();
}


void spinCube()
{
	initBufferObject();
	fixedDisplay();
	shaderDisplay();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, p.nearz, p.farz);
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	wid = w;
	ht = h;
}

void tranCamera(int a, float len)
{
	for (int i = 0; i < 3; i++)
	{
		pos[i] += axis[a][i] * len;
		dir[i] += axis[a][i] * len;
	}
}


void keyboard(unsigned char key, int x, int y)
{
	float inc = 0.2;
	char modelname[20];
	if (key == 'q' || key == 'Q') exit(0);
	if (key == 'r')
		cmode = 1;
	if (key == 'g')
		cmode = 2;
	if (key == 'b')
		cmode = 3;
	if (key == 'p')
		mode = GL_POLYGON;
	if (key == 'o')
		mode = GL_POINTS;
	if (key == 'i')
		mode = GL_LINE_STRIP;

	if (key == 'm')
	{
		cout << "enter a new model name: ";
		cin >> modelname;
		construct_model(modelname);
		spinCube();
	}
	
	if (key == 'd')
		tranCamera(0, inc);
	if (key == 'a')
		tranCamera(0, -inc);
	if (key == 'w')
		tranCamera(1, inc);
	if (key == 's')
		tranCamera(1, -inc);
	if (key == 'x')
		tranCamera(2, inc);
	if (key == 'z')
		tranCamera(2, -inc);

	if (key == 'l')
	{
		p.nearz += 1.0;
		if (p.farz < p.nearz)
			p.farz = p.nearz;
		spinCube();
	}
	if (key == 'k')
	{

		p.nearz -= 1.0;
		if (p.nearz < 1.0)
			p.nearz = 1.0;
		spinCube();
	}
	if (key == 'j')
	{
		p.farz += 1.0;
		spinCube();
	}
	if (key == 'h')
	{
		p.farz -= 1.0;
		if (p.farz < p.nearz)
			p.farz = p.nearz;
		spinCube();
	}

	if (key == '+')
	{
		rotationMatrix(axis[2], angle);
		lookAt();
		spinCube();
	}
	if (key == '-')
	{
		rotationMatrix(axis[2], -angle);
		lookAt();
		spinCube();
	}

	if (key == 27)
	{
		pos[0] = 0.0;
		pos[1] = 0.0;
		pos[2] = 5.0;

		dir[0] = 0.0;
		dir[1] = 0.0;
		dir[2] = 0.0;

		axis[0][0] = 1.0;
		axis[0][1] = 0.0;
		axis[0][2] = 0.0;
		axis[1][0] = 0.0;
		axis[1][1] = 1.0;
		axis[1][2] = 0.0;
		axis[2][0] = 0.0;
		axis[2][1] = 0.0;
		axis[2][2] = 1.0;
		spinCube();
	}
	modifycolor();
	spinCube();
	
}

void SpecialKeys(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_LEFT:
		rotationMatrix(axis[1], angle);
		lookAt();
		spinCube();
		break;
	case GLUT_KEY_RIGHT:
		rotationMatrix(axis[1], -angle);
		lookAt();
		spinCube();
		break;
	case GLUT_KEY_UP:
		rotationMatrix(axis[0], angle);
		lookAt();
		spinCube();
		break;
	case GLUT_KEY_DOWN:
		rotationMatrix(axis[0], -angle);
		lookAt();
		spinCube();
		break;
	}
}

void Shaderwindow()
{
	glutInitWindowSize(500, 500);
	shWin = glutCreateWindow("Shader Pipeline Window");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glewInit();
	initBufferObject();
	setShaders();
	glutDisplayFunc(shaderDisplay);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);
}

void Fixedwindow()
{
	glutInitWindowSize(500, 500);
	fixWin = glutCreateWindow("Fixed Pipeline Window");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(fixedDisplay);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
	construct_model("cactus.obj");
	setPersp();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	Fixedwindow();
	Shaderwindow();
	glutMainLoop();
}
