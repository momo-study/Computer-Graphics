#ifndef BRESENHAM_H_
#define BRESENHAM_H_

#include <vector>
#include <algorithm>

using namespace std;

struct Point {
	int x;
	int y;
	Point(int x_, int y_) :x(x_), y(y_) {}
};

class Bresenham 
{
public:
	Bresenham() {}
	~Bresenham() {}
	vector<Point> drawCircle(Point center, int radius);
	vector<Point> drawTriangle(Point ptA, Point ptB, Point ptC);
	vector<Point> bresenham(Point ptA, Point ptB);
	vector<Point> triangleRasterization(Point ptA, Point ptB, Point ptC);
	float* produceVertices(vector<Point> points);
private:
	void swap(int *a, int *b);
	vector<Point> getCirclePoints(Point center, Point pt);
	int getMin(int a, int b, int c);
	int getMax(int a, int b, int c);
};

#endif

