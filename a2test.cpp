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
using namespace glm;

vector<vec3> vertex;
vector<vec3> face;

double min_x, min_y, min_z, max_x, max_y, max_z;
bool cw = false;
GLfloat fovy = 45, znear = 1.0, zfar = 100.0, aspect;
class Camera {
public:
	Vec3 position;
	Vec3 direction;
	Vec3 up;
	Vec3 rotate;
	double fov, pov;
};

Camera cam;
float color[4] = { 1.0,0.0,0.0,0.0 };
GLhandleARB v, f, f2, p;
char* model;

void loadModel(const char *fName)
{
	FILE* f = fopen(fName, "r");
	ifstream file(fName);
	if (file.is_open()) {
		do {
			string str;
			file >> str;
			if (str == "v") {
				vec3 tv;
				file >> tv.x >> tv.y >> tv.z;
				vertex.push_back(tv);
			}
			else if (str == "f") {
				vec3 tf;
				file >> tf.x >> tf.y >> tf.z;
				face.push_back(tf);
			}
		} while (file.peek() != EOF);
		cout << vertex.size() << " " << face.size() << endl;
	}

	fclose(f);
	file.close();
	
	for (int j = 0; j < vertex.size(); j++)
	{
		if (vertex[j].x > max_x)
			max_x = vertex[j].x;
		else if (vertex[j].x < min_x)
			min_x = vertex[j].x;
		if (vertex[j].y > max_y)
			max_y = vertex[j].y;
		else if (vertex[j].y < min_y)
			min_y = vertex[j].y;
		if (vertex[j].z > max_z)
			max_z = vertex[j].z;
		else if (vertex[j].z < min_z)
			min_z = vertex[j].z;
	}

}

void display() {

	aspect = 1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);

	if (cw) glFrontFace(GL_CW);
	else glFrontFace(GL_CCW);

	int color_id = glGetUniformLocation(p, "color");
	glUniform4fvARB(color_id, 1, color);
	glLoadIdentity();

	float fl = 1 / (tan(PI*fovy / 360));
	float perspective[16] = {
		fl / aspect,0,0,0,
		0,fl,0,0,
		0,0,(zfar + znear) / (znear - zfar),(2 * zfar*znear) / (znear - zfar),
		0,0,-1,0
	};

	Vec3 f = Vec3(
		cam.direction.x,
		cam.direction.y,
		cam.direction.z
		);
	f = normalize(f);

	Vec3 up = Vec3(
		cam.up.x,
		cam.up.y,
		cam.up.z
		);
	vec3 s = cross(up, f);

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



