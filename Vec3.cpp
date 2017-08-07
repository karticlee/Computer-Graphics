#include <iostream>
#include <math.h>
#include "Vec3.h"

using namespace std;

// constructors
//Vec3::Vec3(){this->x=0;this->y=0;this->z=0;}



Vec3::Vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

// functions
int Vec3::operator==(Vec3 that) {
	return(x == that.x&&y == that.y&&z == that.z);
}

Vec3 Vec3::operator+(Vec3 that) {
	return Vec3(x + that.x, y + that.y, z + that.z);
}

Vec3 Vec3::operator-(Vec3 that) {
	return Vec3(x - that.x, y - that.y, z - that.z);
}

Vec3 Vec3::operator*(Vec3 that) {
	return Vec3(x*that.x, y*that.y, z*that.z);
}

Vec3 Vec3::operator/(Vec3 that) {
	return Vec3(x / that.x, y / that.y, z / that.z);
}

Vec3 Vec3::operator+(float scalar) {
	return Vec3(x + scalar, y + scalar, y + scalar);
}

Vec3 Vec3::operator-(float scalar) {
	return Vec3(x - scalar, y - scalar, y - scalar);
}

Vec3 Vec3::operator*(float scalar) {
	return Vec3(x*scalar, y*scalar, y*scalar);
}

Vec3 Vec3::operator/(float scalar) {
	return Vec3(x / scalar, y / scalar, y / scalar);
}

float Vec3::dot(Vec3 that) {
	return (x*that.x + y*that.y + z*that.z);
}

Vec3 Vec3::cross(Vec3 that) {
	return Vec3(y * that.z - z * that.y,
		z * that.x - x * that.z,
		x * that.y - y * that.x);
}
Vec3 Vec3::normalize() {
	double foo = sqrt((x * x) + (y * y) + (z * z));
	return Vec3(x / foo, y / foo, z / foo);
}


float Vec3::magnitude() {
	return float(sqrt(x*x + y*y + z*z));
}


