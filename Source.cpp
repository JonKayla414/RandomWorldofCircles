#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };

class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	ONOFF onoff;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
	};

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
};


class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.03;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
	ONOFF onoff;

	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
		onoff = ON;
	}

	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE && this->onoff == ON)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = -direction;
				x = x + 0.0003;
				y = y + 0.0004;
				this->onoff = OFF;
			}
		}
		else if (brk->brick_type == DESTRUCTABLE && this->onoff == ON)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				brk->red = GetRandomColor();
				brk->green = GetRandomColor();
				brk->blue = GetRandomColor();
				brk->drawBrick();
			}
		}
	}

	float GetRandomColor()
	{
		return (rand() % 255)/255.0f;
	}

	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	void MoveOneStep()
	{
		if (direction == 1 || direction == 5 || direction == 6)  // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 2 || direction == 5 || direction == 7)  // right
		{
			if (x < 1 - radius)
			{
				x += speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 3 || direction == 7 || direction == 8)  // down
		{
			if (y < 1 - radius) {
				y += speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 4 || direction == 6 || direction == 8)  // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
	}

	void DrawCircle()
	{
		if (onoff == ON)
		{
			glColor3f(red, green, blue);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++) {
				float degInRad = i * DEG2RAD;
				glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
			}
			glEnd();
		}
	}
};


vector<Circle> world;


int main(void) {
	srand(time(NULL));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "Random World of Circles", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	Brick brick(DESTRUCTABLE, 0.5, -0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick2(REFLECTIVE, -0.5, 0.33, 0.2, 72.0f/255.0f, 159.0f/255.0f, 223.0f/255.0f);
	Brick brick3(DESTRUCTABLE, -0.5, -0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick4(DESTRUCTABLE, 0.5, 0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick5(DESTRUCTABLE, -0.5, 0, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick6(DESTRUCTABLE, 0.5, 0, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick7(DESTRUCTABLE, 0, 0, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick8(DESTRUCTABLE, 0.0, -0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick9(DESTRUCTABLE, 0.0, 0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick10(DESTRUCTABLE, 0.25, 0.0, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick11(DESTRUCTABLE, -0.25, 0.0, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick12(DESTRUCTABLE, -0.25, -0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick13(DESTRUCTABLE, 0.25, -0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);
	Brick brick14(REFLECTIVE, -0.25, 0.33, 0.2, 1, 51.0f / 255.0f, 126.0f / 255.0f);
	Brick brick15(DESTRUCTABLE, 0.25, 0.33, 0.2, 1, 182.0f / 255.0f, 193.0f / 255.0f);


	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		//Movement
		for (int i = 0; i < world.size(); i++)
		{
			world[i].CheckCollision(&brick);
			world[i].CheckCollision(&brick2);
			world[i].CheckCollision(&brick3);
			world[i].CheckCollision(&brick4);
			world[i].CheckCollision(&brick5);
			world[i].CheckCollision(&brick6);
			world[i].CheckCollision(&brick7);
			world[i].CheckCollision(&brick8);
			world[i].CheckCollision(&brick9);
			world[i].CheckCollision(&brick10);
			world[i].CheckCollision(&brick11);
			world[i].CheckCollision(&brick12);
			world[i].CheckCollision(&brick13);
			world[i].CheckCollision(&brick14);
			world[i].CheckCollision(&brick15);
			world[i].MoveOneStep();
			world[i].DrawCircle();

		}

		brick.drawBrick();
		brick2.drawBrick();
		brick3.drawBrick();
		brick4.drawBrick();
		brick5.drawBrick();
		brick6.drawBrick();
		brick7.drawBrick();
		brick8.drawBrick();
		brick9.drawBrick();
		brick10.drawBrick();
		brick11.drawBrick();
		brick12.drawBrick();
		brick13.drawBrick();
		brick14.drawBrick();
		brick15.drawBrick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		double r, g, b;
		r = rand() / 10000;
		g = rand() / 10000;
		b = rand() / 10000;
		Circle B(0, 0, 02, 2, 0.05, r, g, b);
		world.push_back(B);
	}

	


}