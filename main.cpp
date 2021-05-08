#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;

// class : game
GLFWwindow* glfwWindow = nullptr;
int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
string title = "GELDA";

// class : game object
float positionX = 0.5f;
float positionY = 0.5f;
float moveX = 0.0f;
float moveY = 0.0f;
const float delta = 0.05f;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) moveX = -delta;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)	moveX = 0.0f;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) moveX = delta;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) moveX = 0.0f;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) moveY = delta;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) moveY = 0.0f;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) moveY = -delta;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) moveY = 0.0f;
}

bool init()
{
	if (!glfwInit()) return false;

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(glfwWindow);
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwGetFramebufferSize(glfwWindow, &width, &height);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);

	if (!glfwWindow) return false;


	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) return false;


	glfwSwapInterval(1); // 60fps

	glViewport(0, 0, width, height);
	const float aspect_ratio = (float)width / (float)height;
	glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl;
	std::cout << "Press Escape key to exit" << std::endl;

	return true;
}

void update()
{
	positionX += moveX;
	positionY += moveY;
}

void draw()
{
	glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	{
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2f(positionX, positionY);
		glVertex2f(positionX - 1.0f, positionY);
		glVertex2f(positionX - 1.0f, positionY - 1.0f);
		glVertex2f(positionX, positionY - 1.0f);
		glEnd();
	}

	glPopMatrix();

	glFlush();
}

int main() {

	init();

	while (!glfwWindowShouldClose(glfwWindow))
	{
		update();
		draw();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	std::cout << "See U GELDA!" << endl;

	return 0;
}