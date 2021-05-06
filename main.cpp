#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;

float gPositionX = 0.5f;
float gPositionY = 0.5f;
float moveX = 0.0f;
float moveY = 0.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		moveX = -0.1f;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		moveX = 0.0f;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		moveX = 0.1f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		moveX = 0.0f;
	}


	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		moveY = 0.1f;
	}
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		moveY = 0.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		moveY = -0.1f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		moveY = 0.0f;
	}
}

int main() {
	GLFWwindow* glfwWindow = nullptr;
	int width = DEFAULT_WIDTH;
	int height = DEFAULT_HEIGHT;
	string title = "GELDA";

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(glfwWindow);
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwGetFramebufferSize(glfwWindow, &width, &height);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);

	if (!glfwWindow)
	{
		return -1;
	}


	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glfwSwapInterval(1); // set 60fps

	const float aspect_ratio = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl;
	std::cout << "Press Escape key to exit" << std::endl;


	while (!glfwWindowShouldClose(glfwWindow))
	{
		glfwMakeContextCurrent(glfwWindow);
		// (100, 149, 237) is background color
		glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		gPositionX += moveX;
		gPositionY += moveY;


		// rendering loop
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2f(gPositionX, gPositionY);
		glVertex2f(gPositionX - 1.0f, gPositionY);
		glVertex2f(gPositionX - 1.0f, gPositionY - 1.0f);
		glVertex2f(gPositionX, gPositionY - 1.0f);
		glEnd();

		glPopMatrix();
		glFlush();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	std::cout << "See U GELDA!" << endl;

	return 0;
}