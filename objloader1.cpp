#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <string>
#include "objload.h"
#include <stdlib.h>

using namespace std;
objLoader1::objLoader1()
{
	{
		for (int i = 0; i < maxVertex; i++) {
			v[i].x = 0;
			v[i].y = 0;
			v[i].z = 0;
		}
	}
}

objLoader1::~objLoader1()
{

}

void objLoader1::loadVertex(char* fname)
{
	FILE  *file;
	char l1[20],l2[20],l3[20],h[10];
	file = fopen(fname,"r");
	int c = 0;
	int p1, p2, p3;

	while (fscanf(file, "%s", h) !=EOF && strcmp(h, "v") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		v[c].x = atof(l1);
		v[c].y = atof(l2);
		v[c].z = atof(l3);
		c++;	
	}
	while (strcmp(h, "f") == 0)
	{
		fscanf(file, "%s %s %s", l1, l2, l3);
		p1 = atoi(l1);
		p2 = atoi(l2);
		p3 = atoi(l3);
		glBegin(GL_POLYGON);
		glVertex3d(v[p1 - 1].x, v[p1 - 1].y, v[p1 - 1].z);
		glVertex3d(v[p2 - 1].x, v[p2 - 1].y, v[p2 - 1].z);
		glVertex3d(v[p3 - 1].x, v[p3 - 1].y, v[p3 - 1].z);
		glEnd();
		if (fscanf(file, "%s", h) == EOF)
		{
			break;
		}

	}

}
