#ifndef BEZIER_H_
#define BEZIER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

using namespace std;

struct Point {
	float x, y, z;
	Point() {};
	Point(float xPos, float yPos) {
		x = xPos;
		y = yPos;
		z = 0.0f;
	}
};

class Bezier {
private:
	int num = 0;
	float vertices[10000];
	vector<Point> points_list;
	unsigned int VAO, VBO;
	// ��������
	int fac(int n); // ��n�Ľ׳�
	float BernsteinBasicFunc(float t, int n, int i); // Bernstein������
public:
	Bezier();
	~Bezier();
	void addPoint(Point new_point);
	void deletePoint();
	void drawPoints();
	void drawBezierCurve();
	void dynamicDisplay(float t);
};

#endif

