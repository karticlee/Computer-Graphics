#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <vector>
#include <glm/glm.hpp> 

#define PI 3.14159265359

using namespace std;
using namespace glm;


vector<GLfloat> fileVertices;
vector<GLint> fileFaces;

int draw = GL_TRIANGLES;
float cameraAxis[3][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };
float cameraPosition[3] = { 0,0,5 };
float cameraLookAtPoint[3] = { 0,0,0 };

float rotationMatrix[3][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
float nearClipping = 1.0;
float farClipping = 10;
int win1, win2;
int c = 3;
GLuint vsID, fsID, pID, VBO, VAO, EBO;
GLfloat projectionMat[16], modelViewMat[16];
GLfloat color[][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0 } };

void splitelements(const string &s, char delim, vector<string> &elems) 
{
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

vector<string> splitline(const string &s, char delim) 
{
	vector<string> elems;
	splitelements(s, delim, elems);
	return elems;
}

void ReadFile(char * filename) {

	ifstream myfile;

	vector<GLfloat> tempFileVertices;
	vector<GLint> tempFileFaces;
	myfile.open(filename, ios::in);
	bool abcd = myfile.is_open();


	if (myfile.is_open() == true)
	{

		string lineAbc;

		while (getline(myfile, lineAbc)) {
			vector<string> x = splitline(lineAbc, ' ');

			if (x[0] == "v") {
				tempFileVertices.push_back(std::stof(x[1]));
				tempFileVertices.push_back(std::stof(x[2]));
				tempFileVertices.push_back(std::stof(x[3]));
				tempFileVertices.push_back(color[c][0]);
				tempFileVertices.push_back(color[c][1]);
				tempFileVertices.push_back(color[c][2]);

			}
			else if (x[0] == "f") {
				tempFileFaces.push_back((std::stoi(x[1])) - 1);
				tempFileFaces.push_back((std::stoi(x[2])) - 1);
				tempFileFaces.push_back((std::stoi(x[3])) - 1);
			}


		}
	}

	fileFaces = tempFileFaces;
	fileVertices = tempFileVertices;
}

//code for fixed pipeline
void drawVextex(int i) {

	int vertexPosition = (i)* 6;
	GLfloat tempVertex[][3] = { { fileVertices[vertexPosition++],
		fileVertices[vertexPosition++],
		fileVertices[vertexPosition] } };
	glVertex3fv(tempVertex[0]);

}


void drawObject() {
	int sizeOfFaces = fileFaces.size();
	for (int i = 0; i < sizeOfFaces; i++) 
	{
		glBegin(draw);
		drawVextex(fileFaces[i++]);
		drawVextex(fileFaces[i++]);
		drawVextex(fileFaces[i]);

		glEnd();
	}
}
void changeSize(int w, int h) {

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	gluPerspective(60.0f, (float)w / (float)h, nearClipping, farClipping);

	glMatrixMode(GL_MODELVIEW);

}

void fixdisplay(void) {

	glutSetWindow(win1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60.0f, 1, nearClipping, farClipping);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		cameraLookAtPoint[0], cameraLookAtPoint[1], cameraLookAtPoint[2],
		cameraAxis[1][0], cameraAxis[1][1], cameraAxis[1][2]);
	
	drawObject();

	glutSwapBuffers();
}


void changeColor()
{
	for (unsigned int i = 0; i < fileVertices.size(); i = i + 6)
	{
		fileVertices[i + 3] = color[c][0];
		fileVertices[i + 4] = color[c][1];
		fileVertices[i + 5] = color[c][2];

	}
}

void modelview() 
{
	float matrix1[16];

	matrix1[0] = cameraAxis[0][0];
	matrix1[4] = cameraAxis[0][1];
	matrix1[8] = cameraAxis[0][2];

	matrix1[1] = cameraAxis[1][0];
	matrix1[5] = cameraAxis[1][1];
	matrix1[9] = cameraAxis[1][2];

	matrix1[2] = cameraAxis[2][0];
	matrix1[6] = cameraAxis[2][1];
	matrix1[10] = cameraAxis[2][2];

	matrix1[12] = -(cameraAxis[0][0] * cameraPosition[0] + cameraAxis[0][1] * cameraPosition[1] + cameraAxis[0][2] * cameraPosition[2]);
	matrix1[13] = -(cameraAxis[1][0] * cameraPosition[0] + cameraAxis[1][1] * cameraPosition[1] + cameraAxis[1][2] * cameraPosition[2]);
	matrix1[14] = -(cameraAxis[2][0] * cameraPosition[0] + cameraAxis[2][1] * cameraPosition[1] + cameraAxis[2][2] * cameraPosition[2]);


	matrix1[3] = matrix1[7] = matrix1[11] = 0.0;
	matrix1[15] = 1.0;
	memcpy(modelViewMat, matrix1, 16 * sizeof(float));
}



void perspective()
{
	float fovyInDegrees = 60.0f;
	float aspectRatio = 1;
	float znear = nearClipping;
	float zfar = farClipping;
	float top, right, left, bottom;
	float temp, temp2, temp3, temp4;
	top = znear * tanf(fovyInDegrees * PI / 360.0);
	right = top * aspectRatio;
	left = -right;
	bottom = -top;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	projectionMat[0] = temp / temp2;
	projectionMat[1] = 0.0;
	projectionMat[2] = 0.0;
	projectionMat[3] = 0.0;
	projectionMat[4] = 0.0;
	projectionMat[5] = temp / temp3;
	projectionMat[6] = 0.0;
	projectionMat[7] = 0.0;
	projectionMat[8] = (right + left) / temp2;
	projectionMat[9] = (top + bottom) / temp3;
	projectionMat[10] = (-zfar - znear) / temp4;
	projectionMat[11] = -1.0;
	projectionMat[12] = 0.0;
	projectionMat[13] = 0.0;
	projectionMat[14] = (-temp * zfar) / temp4;
	projectionMat[15] = 0.0;
}



//code for shader
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
		cout << "shader code ----" << shaderCode << endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

}

void shdisplay(void)
{
	glutSetWindow(win2);
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
	glBufferData(GL_ARRAY_BUFFER, fileVertices.size() * sizeof(GLfloat), &fileVertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fileFaces.size() * sizeof(GLint), &fileFaces[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(VAO);
	glDrawElements(draw, fileFaces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
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

	cout << "modeViewMatLocation  =  " << modeViewMatLocation << endl;
	cout << "projectionMatLocation  =  " << projectionMatLocation << endl;
	glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);


	glDeleteShader(vsID);
	glDeleteShader(fsID);
}


void initBufferObject(void)
{

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

	std::vector<GLfloat> tempfileVertices(vertData, vertData + sizeof vertData / sizeof vertData[0]);
	std::vector<GLint> tempfileFaces(indices, indices + sizeof indices / sizeof indices[0]);
	// position information
	int size = sizeof(vertData);
	int size1 = sizeof(fileVertices);
	int size2 = fileVertices.size();
	int size3 = fileVertices.size() * sizeof(GLfloat);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, fileVertices.size() *sizeof(GLfloat), &fileVertices[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fileFaces.size() * sizeof(GLint), &fileFaces[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// Use depth buffering for hidden surface elimination
	glEnable(GL_DEPTH_TEST);

	perspective();

	modelview();

	//glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat);
}
void render()
{
	initBufferObject();
	shdisplay();
	fixdisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	char mname[20];
	if (key == 'q' || key == 'Q') exit(0);

	if (key == 'l')
		draw = GL_POLYGON;
	if (key == 'k')
		draw = GL_POINTS;
	if (key == 'j')
		draw = GL_LINE_STRIP;


	if (key == 'n')
	{
		cout << "enter a new object file: ";
		cin >> mname;
		ReadFile(mname);
		render();
	}

	if (key == 'r')
		c = 0;
	if (key == 'g')
		c = 1;
	if (key == 'b')
		c = 2;


	if (key == 'p')
	{
		nearClipping += 0.5;
		if (farClipping < nearClipping)
			farClipping = nearClipping;
		render();
	}
	if (key == 'o')
	{

		nearClipping -= 0.5;
		if (nearClipping < 1.0)
			nearClipping = 1.0;
		render();
	}
	if (key == 'i')
	{
		farClipping += 0.5;
		render();
	}
	if (key == 'u')
	{
		farClipping -= 0.5;
		if (farClipping < nearClipping)
			farClipping = nearClipping;
		render();
	}

	if (key == 'f')
	{
		cameraAxis[0][0] = 1;
		cameraAxis[0][1] = 0;
		cameraAxis[0][2] = 0;

		cameraAxis[1][0] = 0;
		cameraAxis[1][1] = 1;
		cameraAxis[1][2] = 0;

		cameraAxis[2][0] = 0;
		cameraAxis[2][1] = 0;
		cameraAxis[2][2] = 1;

		cameraPosition[0] = 0;
		cameraPosition[1] = 0;
		cameraPosition[2] = 5;
		cameraLookAtPoint[0] = 0;
		cameraLookAtPoint[1] = 0;
		cameraLookAtPoint[2] = 0;
	}
	changeColor();
	render();

}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);

	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);

	win1 = glutCreateWindow("Fixed Pipeline");
	glutReshapeFunc(changeSize);
	glutDisplayFunc(fixdisplay);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);

	ReadFile("bunny.obj");

	glutInitWindowSize(500, 500);
	win2 = glutCreateWindow("Shader Pipeline");
	glewInit();
	initBufferObject();
	setShaders();
	glutDisplayFunc(shdisplay);
		
	glutMainLoop();
	return 0;
}
