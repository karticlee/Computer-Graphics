#include <GL/freeglut.h>

#include <stdio.h>

#include "objload.h"

objLoader1 obj;
bool stop = false;

float angle;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;

void display(void)
{
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 1.0, 0.0, 1.0);
	obj.loadVertex("bunny.obj");
	//obj.drawModel("bunny.obj");
	glFlush();
	glutSwapBuffers();
	//angle += .1f;
}

void drawmesh()
{
	theta[axis] += 2.0;
	if (theta[axis] > 360, 0) theta[axis] -= 360;
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);
	if (key == ' ') stop = !stop;
	if (stop)
		glutIdleFunc(NULL);
	else
		glutIdleFunc(drawmesh);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0,
		2.0, -10.0, 10.0);
	/*if (w <= h)
	glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
	2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
	glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
	2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);*/

	glMatrixMode(GL_MODELVIEW);

} 

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitContextFlags(GLUT_COMPATIBILITY_PROFILE);
	glutCreateWindow("Bunny image");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(drawmesh); 
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}