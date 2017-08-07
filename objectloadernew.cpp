#define _CRT_SECURE_NO_DEPRECATE
#include "objectloadernew.h"
# include <stdlib.h>
#include <iostream>
# include <GL/freeglut.h>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Objectloadernew.h"
#include <glm/gtc/matrix_transform.hpp>


int loadOBJ(
	 std :: string  path,
	std::vector<glm::vec3> & out_vertices
	
	
) 
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices;
	std::vector<glm::vec3> temp_vertices;
	
	const char * c = path.c_str();

	FILE * file = fopen(c, "r");
	if (file == NULL) {
		printf("not possible to open the file");
		getchar();
		return false;
	}
	else
	{
		printf("File open succesfully");
	}
	int number_of_faces = 0;
	while (1) {

		char lineHeader[128];
		
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; 

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		
		else if (strcmp(lineHeader, "f") == 0) {
			
			unsigned int vertexIndex[3];
			int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			number_of_faces = number_of_faces + 1;
			
			
		}
		else {
			
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		
		unsigned int vertexIndex = vertexIndices[i];
		

		
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		

		
		out_vertices.push_back(vertex);
		

	}
	int a = 4;
	//std::cout << "the value of a" << a;
	//std::cout << "number of faces " << number_of_faces;
	
	return number_of_faces;
}


