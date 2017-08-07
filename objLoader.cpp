#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <string>
#include "objload.h"
#include <stdlib.h>

using namespace std;
objLoader::objLoader()
{
	{
		for (int i = 0; i < maxVertex; i++) {
			v[i].x = 0;
			v[i].y = 0;
			v[i].z = 0;
		}
	}
}

objLoader::~objLoader()
{

}

void objLoader::loadVertex(char* fname)
{
	ifstream file;
	string line;
	file.open(fname);
	int flag = 1;
	int i, s, e,len;
	int c = 0;
	while (flag)
	{
		getline(file, line);
		i = 0;
		len = line.length();
		if (line[i] == 'v')
		{
			i = i + 2;
			s = i;
			while (line[i] != ' ')
			{
				i++;
			}
			e = i;
			v[c].x = atof((line.substr(s, (e - s))).c_str());
			i++;
			s = i;
			while (line[i] != ' ')
			{
				i++;
			}
			e = i;
			v[c].y = atof((line.substr(s, (e - s))).c_str());
			i++;
			s = i;
			e = len;
			v[c].z = atof((line.substr(s, (e - s))).c_str());
		}
		else
		{
			flag = 0;
		}
		c++;
	}

}

void objLoader::drawModel(char* fname)
{
	ifstream file;
	string line;
	file.open(fname);
	int flag = 1;
	int i, s, e,len;
	int c = 0;
	int p1, p2, p3;

	while (flag)
	{
		getline(file, line);
		if (line[0] == 'f')
			break;
	}

	while (flag)
	{
		i = 0;
		len = line.length();
		if (line[i] == 'f')
		{
			i = i + 2;
			s = i;
			while (line[i] != ' ')
			{
				i++;
			}
			e = i;
			p1 = atoi((line.substr(s, (e - s))).c_str());
			i++;
			s = i;
			while (line[i] != ' ')
			{
				i++;
			}
			e = i;
			p2 = atoi((line.substr(s, (e - s))).c_str());
			i++;
			s = i;
			e = len;
			p3 = atoi((line.substr(s, (e - s))).c_str());
		}
		else
		{
			flag = 0;
		}
		glBegin(GL_POLYGON);
		glVertex3d(v[p1 - 1].x, v[p1 - 1].y, v[p1 - 1].z);
		glVertex3d(v[p2 - 1].x, v[p2 - 1].y, v[p2 - 1].z);
		glVertex3d(v[p3 - 1].x, v[p3 - 1].y, v[p3 - 1].z);
		glEnd(); 
		getline(file, line);
		if (file.eof())
			break;
	}
}
