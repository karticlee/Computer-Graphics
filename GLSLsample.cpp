#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <sstream>

// for GLSL
GLuint vsID, fsID, pID, VBO, VAO, EBO;

// for uniform input
GLfloat projectionMat[16], modelViewMat[16];

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

void display(void)
{
	glUseProgram(pID);
	//glUseProgram(0);

	glValidateProgram(pID);
	GLint validate = 0;
	glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
}

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

	// Setup the view of the cube
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 1.0, 10.0);

	glGetFloatv(GL_PROJECTION_MATRIX, projectionMat);


	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
		0.0, 0.0, 0.0,        /* center is at (0,0,0) */
		0.0, 1.0, 0.);        /* up is in positive Y direction */

							  // Adjust cube position
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	glRotatef(-20, 0.0, 0.0, 1.0);

	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Hellow GLSL");
	glewInit();

	initBufferObject();
	setShaders();

	glutDisplayFunc(display);


	glutMainLoop();
	return 0;
}
