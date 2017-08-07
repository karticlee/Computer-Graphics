
#ifndef OBJLOAD_H
#define OBJLOAD_H

#define maxVertex 35000

using namespace std;

struct Vertex {
	/*
	GLfloat x;
	GLfloat y;
	GLfloat z; */

	double x, y, z;


};

class objLoader1
{
private:
	Vertex v[maxVertex];

public:
	objLoader1();
	~objLoader1();

	void loadVertex(char* fname);
	//void drawModel(char* fname);
};


#endif
