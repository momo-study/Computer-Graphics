# Homework 9 - Bezier Curve

[TOC]



*****



## 作业要求

**Basic:** 

1. 用户能通过左键点击添加Bezier曲线的控制点，右键点击则对当前添加的最后一个控制点进行消除 
2. 工具根据鼠标绘制的控制点实时更新Bezier曲线。 Hint: 大家可查询捕捉mouse移动和点击的函数方法 

**Bonus:**

1. 可以动态地呈现Bezier曲线的生成过程。



****



## 代码思路

> 贝塞尔曲线于[1962年](http://zh.wikipedia.org/wiki/1962%E5%B9%B4)，由[法国](http://zh.wikipedia.org/wiki/%E6%B3%95%E5%9C%8B)工程师[皮埃尔·贝塞尔](http://zh.wikipedia.org/w/index.php?title=%E7%9A%AE%E5%9F%83%E7%88%BE%C2%B7%E8%B2%9D%E8%8C%B2&action=edit&redlink=1)（Pierre Bézier）所广泛发表，他运用贝塞尔曲线来为[汽车](http://zh.wikipedia.org/wiki/%E6%B1%BD%E8%BB%8A)的主体进行设计。贝塞尔曲线最初由[Paul de Casteljau](http://zh.wikipedia.org/w/index.php?title=Paul_de_Casteljau&action=edit&redlink=1) 于[1959年](http://zh.wikipedia.org/wiki/1959%E5%B9%B4)运用[de Casteljau 算法](http://zh.wikipedia.org/wiki/De_Casteljau%E7%AE%97%E6%B3%95)开发，以[稳定数值](http://zh.wikipedia.org/wiki/%E6%95%B0%E5%80%BC%E7%A8%B3%E5%AE%9A%E6%80%A7)的方法求出贝塞尔曲线。



### 鼠标移动和点击

调用`glfwSetCursorPos()`函数来获取鼠标移动时的位置，用全局变量lastX和lastY来确定最后鼠标点击时的位置

![1559107241750](C:\Users\win8.1\AppData\Roaming\Typora\typora-user-images\1559107241750.png)

```c++
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	lastX = xpos;
	lastY = ypos;
}

glfwSetCursorPosCallback(window, mouse_callback);
```



用`glfwSetMouseButtonCallback()`函数来判定鼠标的点击，并在左键和右键点击时进行添加和删除的操作。

![1559107478926](C:\Users\win8.1\AppData\Roaming\Typora\typora-user-images\1559107478926.png)

```c++
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			Point new_point(-1.0 + 2 * (lastX / SCR_WIDTH), 1.0 - 2 * (lastY / SCR_HEIGHT));
			bezier.addPoint(new_point);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			bezier.deletePoint();
		}
	}
	return;
}

glfwSetMouseButtonCallback(window, mouse_button_callback);
```



### Bezier曲线

Bezier曲线的一般化公式

$ B(t) = \sum_{i =0}^{n}P_ib_{i,n}(t), t \in [0, 1]$

其中，Bernstein基函数$$b_{i,n}(t)$$为

$b_{i,n}(t) = (^n_i)t^i(1-t)^{n-i}, i = 0,1,...,n$

因此，可以写出辅助函数`BernsteinBasicFunc`

```c++
float Bezier::BernsteinBasicFunc(float t, int n, int i)
{
	return (fac(n) / (fac(i) * fac(n - i))) * pow(t, i) * pow((1 - t), (n - i));
}
```

其中，求阶乘的函数为

```c++
int Bezier::fac(int n) {
	if (n == 0) {
		return 1;
	}
    for (int i = n - 1; i >= 1; --i) {
		n *= i;
	}
	return n;
}
```

我们定义Point结构体，作为存储点的单元结构，并用vector容器进行存储，这里定义`vector<Point> points_list;`

```c++
struct Point {
	float x, y, z;
	Point() {};
	Point(float xPos, float yPos) {
		x = xPos;
		y = yPos;
		z = 0.0f;
	}
};
```

之后，由Bezier曲线公式，有

```c++
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
```

结果如图：

![1559108168239](C:\Users\win8.1\AppData\Roaming\Typora\typora-user-images\1559108168239.png)



### 动态展示

![1559108317835](C:\Users\win8.1\AppData\Roaming\Typora\typora-user-images\1559108317835.png)

动态展示的思路是，如图，蓝色点在红色两点间“滑动”，黄色点在蓝色两点间“滑动”，依此类推，我们可以用一个循环来表示该过程，这里的“滑动”，可以由一阶Bezier曲线来表示，即

$B(t) = P_0 + (P_1-P_0)t = (1-t)P_0+tP_1$

```c++
for (int count = num - 1; count >= 0; count--) {
	for (int i = 0; i < count; i++) {
		vertices[3 * i] = (1 - t) * vertices[3 * i] + t * vertices[3 * i + 3];
		vertices[3 * i + 1] = (1 - t) * vertices[3 * i + 1] + t * vertices[3 * i + 4];
		vertices[3 * i + 2] = 0.0f;
	}
}
```







