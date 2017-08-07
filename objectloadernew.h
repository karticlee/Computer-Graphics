
#ifndef OBJLOADER_H
#define OBJLOADER_H
#define _CRT_SECURE_NO_DEPRECATE
# include <stdlib.h>
# include <GL/freeglut.h>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <Objectloadernew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int loadOBJ(
	std :: string  path,
	std::vector<glm::vec3> & out_vertices
	
	
);
#endif