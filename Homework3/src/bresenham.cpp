#include "bresenham.h"

const int width = 640;
const int height = 320;

void Bresenham::swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

vector<Point> Bresenham::bresenham(Point A, Point B)
{
	vector<Point> points;
	int flag;
	int ix, iy;
	int x = A.x, y = A.y;
	int dx = abs(B.x - A.x);
	int dy = abs(B.y - A.y);

	if (B.x > A.x) {
		ix = 1;
	}
	else {
		ix = -1;
	}
	if (B.y > A.y) {
		iy = 1;
	}
	else {
		iy = -1;
	}
	if (abs(B.y - A.y) > abs(B.x - A.x))
	{
		swap(&dx, &dy);
		flag = 1;
	}
	else
		flag = 0;

	int p = 2 * dy - dx;
	for (int i = 1; i <= dx; i++) {
		points.push_back(Point(x, y));
		if (p >= 0) {
			if(flag == 0)
				y = y + iy;
			else
				x = x + ix;
			p = p - 2 * dx;
		}
		if (flag == 0)
			x = x + ix;
		else
			y = y + iy;
		p = p + 2 * dy;
	}
	return points;
}

vector<Point> Bresenham::drawTriangle(Point A, Point B, Point C)
{
	vector<Point> points;
	vector<vector<Point>> list;
	list.push_back(bresenham(A, B));
	list.push_back(bresenham(A, C));
	list.push_back(bresenham(B, C));
	for (auto l : list) {
		for (auto p : l) {
			points.push_back(p);
		}
	}
	return points;
}

vector<Point> Bresenham::getCirclePoints(Point center, Point pt)
{
	vector<Point> points;
	points.push_back(Point(center.x + pt.x, center.y + pt.y));
	points.push_back(Point(center.x - pt.x, center.y + pt.y));
	points.push_back(Point(center.x + pt.x, center.y - pt.y));
	points.push_back(Point(center.x - pt.x, center.y - pt.y));
	points.push_back(Point(center.x + pt.y, center.y + pt.x));
	points.push_back(Point(center.x - pt.y, center.y + pt.x));
	points.push_back(Point(center.x + pt.y, center.y - pt.x));
	points.push_back(Point(center.x - pt.y, center.y - pt.x));
	return points;
}

vector<Point> Bresenham::drawCircle(Point center, int radius)
{
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
	vector<Point> points, res;
	while (x <= y) {
		points = getCirclePoints(center, Point(x, y));
		for (auto p : points) {
			res.push_back(p);
		}
		if (d < 0) {
			d = d + 4 * x + 6;
		}
		else {
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	points = getCirclePoints(center, Point(x, y));
	for (auto p : points) {
		res.push_back(p);
	}
	return res;
}

int Bresenham::getMin(int a, int b, int c)
{
	return min(a, min(b, c));
}

int Bresenham::getMax(int a, int b, int c)
{
	return max(a, max(b, c));
}

vector<Point> Bresenham::triangleRasterization(Point A, Point B, Point C)
{
	vector<Point> points;
	float k1 = (float)(B.y - C.y) / (B.x - C.x),
		b1 = B.y - k1 * B.x,
		k2 = (float)(A.y - C.y) / (A.x - C.x),
		b2 = A.y - k2 * A.x,
		k3 = (float)(A.y - B.y) / (A.x - B.x),
		b3 = A.y - k3 * A.x;
	int x_min = getMin(A.x, B.x, C.x);
	int x_max = getMax(A.x, B.x, C.x);
	int y_min = getMin(A.y, B.y, C.y);
	int y_max = getMax(A.y, B.y, C.y);
	for (int i = x_min; i <= x_max; ++i) {
		for (int j = y_min; j <= y_max; ++j) {
			float u = (k1 * i - j + b1) * (k1 * A.x - A.y + b1),
				v = (k2 * i - j + b2) * (k2 * B.x - B.y + b2),
				w = (k3 * i - j + b3) * (k3 * C.x - C.y + b3);
			if (!(u < 0.0f || v < 0.0f || w < 0.0f)) {
				points.push_back(Point(i, j));
			}
		}
	}
	return points;
}

float* Bresenham::produceVertices(vector<Point> points)
{
	float* vertices = new float[points.size() * 6];
	for (int i = 0; i < points.size(); i++) {
		vertices[i * 6] = (float)points[i].x / (float)width;
		vertices[i * 6 + 1] = (float)points[i].y / (float)height;
		vertices[i * 6 + 2] = 0.0f;
		vertices[i * 6 + 3] = 0.0f;
		vertices[i * 6 + 4] = 1.0f;
		vertices[i * 6 + 5] = 0.0f;
	}
	return vertices;
}