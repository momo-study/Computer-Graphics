#include "bezier.h"

Bezier::Bezier() {}

Bezier::~Bezier() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

int Bezier::fac(int n) {
	if (n == 0) {
		return 1;
	}
    for (int i = n - 1; i >= 1; --i) {
		n *= i;
	}
	return n;
}

float Bezier::BernsteinBasicFunc(float t, int n, int i)
{
	return (fac(n) / (fac(i) * fac(n - i))) * pow(t, i) * pow((1 - t), (n - i));
}

void Bezier::addPoint(Point new_point){
	points_list.push_back(new_point);
	num++;
}

void Bezier::deletePoint() {
	points_list.pop_back();
	num--;
}

void Bezier::drawPoints() {
	if (!points_list.empty()) {
		for (int i = 0; i < num; i++) {
			vertices[3 * i] = points_list[i].x;
			vertices[3 * i + 1] = points_list[i].y;
			vertices[3 * i + 2] = 0.0f;
		}
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, num);
		glDrawArrays(GL_LINE_STRIP, 0, num);
		glBindVertexArray(0);
	}
}

void Bezier::drawBezierCurve() {
	if (num > 1) {
		float xPos, yPos;
		int count = 0;
		int freq = 200;
		float t;
		for (t = 0; t <= 1; t += 1.0 / freq) {
			xPos = yPos = 0;
			for (int i = 0; i < num; ++i) {
				xPos += BernsteinBasicFunc(t, num-1, i) * points_list[i].x;
				yPos += BernsteinBasicFunc(t, num-1, i) * points_list[i].y;
			}
			vertices[3 * count] = xPos;
			vertices[3 * count + 1] = yPos;
			vertices[3 * count + 2] = 0.0f;
			count++;
		}
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINE_STRIP, 0, count);

		glBindVertexArray(0);
	}
}

void Bezier::dynamicDisplay(float t) {
	if (num > 2) {
		for (int i = 0; i < num; i++) {
			vertices[3 * i] = points_list[i].x;
			vertices[3 * i + 1] = points_list[i].y;
			vertices[3 * i + 2] = 0.0f;
		}
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		for (int count = num - 1; count >= 0; count--) {
			for (int i = 0; i < count; i++) {
				vertices[3 * i] = (1 - t) * vertices[3 * i] + t * vertices[3 * i + 3];
				vertices[3 * i + 1] = (1 - t) * vertices[3 * i + 1] + t * vertices[3 * i + 4];
				vertices[3 * i + 2] = 0.0f;
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_LINE_STRIP, 0, count);
			glDrawArrays(GL_POINTS, 0, count);
		}
	}
}