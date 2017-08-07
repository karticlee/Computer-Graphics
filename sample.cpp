/*
Project 2
Cameron Smith
ECE/CS 412
Prof. Yang

Compilation: g++ -o Assignment2 Assignment2.cpp -lglut -lGL -lGLU -lm
Execution: ./Assignment2
Dependencies: Vec3.cpp
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "Vec3.h"
#include "Vec3.cpp"
#include <glm/glm.hpp>

#define MAXNUM 128
#define MAX_MATERIAL_COUNT 6
#define PI 3.14159265359

using namespace std;

class Model {
public:
	struct Triangle {
		Vec3 vertices[3];
		Vec3 normals[3];
		int colors[3];
		Vec3 face_normal;
	};

	struct Face {
		int x, y, z;
	};

	Triangle *triangles;

	std::vector<glm::vec3> vertices;
	std::vector<Face> faces;

	int material_count;
	int num_triangles;
	int color_index[3];
	int i, j, k;
	double min_x, min_y, min_z, max_x, max_y, max_z;
	double max;
	void load(const char *fileName);
	void bounds();
};

void Model::load(const char *fileName) {

	FILE* f = fopen(fileName, "r");
	ifstream file(fileName);
	if (file.is_open()) {
		do {
			string str;
			file >> str;
			if (str == "v") {
				glm::vec3 vertex;
				file >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if (str == "f") {
				Face face;
				file >> face.x >> face.y >> face.z;
				faces.push_back(face);
			}
			else if (str == "vn") {}
		} while (file.peek() != EOF);
		cout << vertices.size() << " " << faces.size() << endl;
	}

	fclose(f);
	file.close();

	// get max
	int length = vertices.size();
	bounds();
}

void Model::bounds() {
	for (int j = 0; j<vertices.size(); j++) {
		if (vertices[j].x>max_x)
			max_x = vertices[j].x;
		else if (vertices[j].x<min_x)
			min_x = vertices[j].x;
		if (vertices[j].y>max_y)
			max_y = vertices[j].y;
		else if (vertices[j].y<min_y)
			min_y = vertices[j].y;
		if (vertices[j].z>max_z)
			max_z = vertices[j].z;
		else if (vertices[j].z<min_z)
			min_z = vertices[j].z;
	}
}

class Camera {
public:
	Vec3 position;
	Vec3 direction;
	Vec3 up;
	Vec3 rotate;
	double aperture;
	int viewWidth, viewHeight;
	double fov, pov;
};

// -- OPENGL & GLUT ----------------------------------------------------------//
typedef struct {
	int width;
	int height;
	char* title;
	float fov_angle;
	float z_near;
	float z_far;
} glutWindow;

Model obj;
float g_rotation;
glutWindow win;
Camera cam;
float znear, zfar;
bool stop = false;
float color[4] = { 0.0,1.0,0.0,0.0 };
float scale = 1.0;

bool points, lines, fills;
GLhandleARB v, f, f2, p;
bool cw = false;
float angle = 0.0f;
char* model;
float aspect;

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);

	if (cw) glFrontFace(GL_CW);
	else glFrontFace(GL_CCW);

	int color_id = glGetUniformLocation(p, "color");
	glUniform4fvARB(color_id, 1, color);
	glLoadIdentity();

	float fl = 1 / (tan(PI*win.fov_angle / 360));
	float perspective[16] = {
		fl / aspect,0,0,0,
		0,fl,0,0,
		0,0,(win.z_far + win.z_near) / (win.z_near - win.z_far),(2 * win.z_far*win.z_near) / (win.z_near - win.z_far),
		0,0,-1,0
	};

	Vec3 f = Vec3(
		cam.direction.x,
		cam.direction.y,
		cam.direction.z
		);
	f.normalize();

	Vec3 up = Vec3(
		cam.up.x,
		cam.up.y,
		cam.up.z
		);
	Vec3 s = up.cross(f);
	Vec3 u = s.normalize().cross(f);

	float modelview[16] = {
		s.x,s.y,s.z,0,
		u.x,u.y,u.z,0,
		-f.x,-f.y,-f.z,0,
		0,0,0,1
	};

	float translation[16] = {
		1,0,0,-cam.position.x,
		0,1,0,-cam.position.y,
		0,0,1,-cam.position.z,
		0,0,0,1
	};

	int translation_id = glGetUniformLocation(p, "translation");
	int modelview_id = glGetUniformLocation(p, "model");
	int perspective_id = glGetUniformLocation(p, "projection");

	glUniformMatrix4fv(modelview_id, 1, true, modelview);
	glUniformMatrix4fv(perspective_id, 1, true, perspective);
	glUniformMatrix4fv(translation_id, 1, true, translation);

	if (points)glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (lines)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (fills)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i<obj.faces.size(); ++i) {
		glVertex3f(obj.vertices[obj.faces[i].x - 1].x*scale,
			obj.vertices[obj.faces[i].x - 1].y*scale,
			obj.vertices[obj.faces[i].x - 1].z*scale);

		glVertex3f(obj.vertices[obj.faces[i].y - 1].x*scale,
			obj.vertices[obj.faces[i].y - 1].y*scale,
			obj.vertices[obj.faces[i].y - 1].z*scale);

		glVertex3f(obj.vertices[obj.faces[i].z - 1].x*scale,
			obj.vertices[obj.faces[i].z - 1].y*scale,
			obj.vertices[obj.faces[i].z - 1].z*scale);
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspect = (float)w / (float)h;
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
}

void positionCamera() {

	double object_x = (obj.max_x + obj.min_x) / 2;
	double object_y = (obj.max_y + obj.min_y) / 2;
	//double radius = std::max(object_x, object_y);
	double radius = 1.0;
	double z = cam.position.z - (obj.min_z*scale);
	double fovy = tan((float)win.height / z);
	double r_max = z * sin(fovy*(PI / 180) / 2);
	scale += r_max / radius;
	printf("%f\n", scale);
}


void resetCamera() {
	cam.position.x = 0.0f;
	cam.position.y = 0.0f;
	cam.position.z = 4.0f;

	cam.direction.x = 0.0;
	cam.direction.y = 0.0;
	cam.direction.z = -1.0;

	cam.up.x = 0.0;
	cam.up.y = -1.0;
	cam.up.z = 0.0;

	scale = 1.0;
	//positionCamera();
}


void keyboard(unsigned char key, int x, int y) {
	float inc = .1;
	float angle = 0.01;
	switch (key) {
	case 27:
		exit(0);
	case 'w':
		cam.position.z += cam.direction.z*inc;
		cam.position.x += cam.direction.x*inc;
		break;
	case 'a':
		cam.position.x += cam.direction.z*inc;
		cam.position.z -= cam.direction.x*inc;
		break;
	case 's':
		cam.position.z -= cam.direction.z*inc;
		cam.position.x -= cam.direction.x*inc;
		break;
	case 'd':
		cam.position.x -= cam.direction.z*inc;
		cam.position.z += cam.direction.x*inc;
		break;
	case 'e':
	{ float x, z;
	x = cam.direction.x;
	z = cam.direction.z;
	cam.direction.x = x*cos(angle) - z*sin(angle);
	cam.direction.z = x*sin(angle) + z*cos(angle); }
	break;
	case 'q':
	{ float x, z;
	x = cam.direction.x;
	z = cam.direction.z;
	cam.direction.x = x*cos(-angle) - z*sin(-angle);
	cam.direction.z = x*sin(-angle) + z*cos(-angle); }
	break;
	case '-':
		scale += .1;
		break;

	case '=':
		if (scale>0)
			scale -= .1;
		break;
	case 'r':
		resetCamera();
		break;
	case 'p':
		points = true;
		lines = false;
		fills = false;
		break;
	case 'o':
		lines = true;
		points = false;
		fills = false;
		break;
	case 'i':
		lines = false;
		points = false;
		fills = true;
		break;
	case 'b':
		cw = !cw;
		break;
	case 'z':
		win.z_near -= inc;
		break;
	case 'x':
		win.z_near += inc;
		break;
	case 'c':
		win.z_far -= inc * 10;
		break;
	case 'v':
		win.z_far += inc * 10;
		break;
	case 'j':
		obj.vertices.clear();
		obj.faces.clear();
		model = (char*)"bunny.obj";
		obj.load(model);
		break;
	case 'k':
		obj.vertices.clear();
		obj.faces.clear();
		model = (char*)"cactus.obj";
		obj.load(model);
		break;
	case 'l':
		obj.vertices.clear();
		obj.faces.clear();
		model = (char*)"bar.obj";
		obj.load(model);
		break;
	case 'n':
		win.fov_angle += inc * 10;
		break;
	case 'm':
		win.fov_angle -= inc * 10;
		break;
	case '1':
		color[0] += inc;
		break;
	case '2':
		color[1] += inc;
		break;
	case '3':
		color[2] += inc;
		break;
	case '4':
		color[3] += inc;
		break;
	case '5':
		color[0] -= inc;
		break;
	case '6':
		color[1] -= inc;
		break;
	case '7':
		color[2] -= inc;
		break;
	case '8':
		color[3] -= inc;
		break;

	}
	glutPostRedisplay();
}

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

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char* vv = vs;
	const char* ff = fs;

	glShaderSourceARB(v, 1, &vv, NULL);
	glShaderSourceARB(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShaderARB(v);
	glCompileShaderARB(f);

	p = glCreateProgramObjectARB();
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgramARB(p);

	glUseProgramObjectARB(p);
}


int main(int argc, char *argv[]) {

	// set window values
	win.title = (char*) "Project 2";
	win.width = 700;
	win.height = 700;
	win.fov_angle = 45.0f;
	win.z_near = .00001f;
	win.z_far = 100.0f;

	// initialize and run program
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(win.width, win.height);
	glutCreateWindow(win.title);
	glutIgnoreKeyRepeat(0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glViewport(0, 0, win.width, win.height);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);

	resetCamera();
	glewInit();
	setShaders();
	glutMainLoop();
	return 0;
}







