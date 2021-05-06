#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;

int main() {
	GLFWwindow* glfwWindow = nullptr;
	int width = DEFAULT_WIDTH;
	int height = DEFAULT_HEIGHT;
	string title = "GELDA";

	if (!glfwInit())
	{
		return -1;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);


	glfwMakeContextCurrent(glfwWindow);
	glfwGetFramebufferSize(glfwWindow, &width, &height);

	glfwSetWindowPos(glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);

	if (!glfwWindow)
	{
		return -1;
	}

	glfwMakeContextCurrent(glfwWindow);


	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}


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
		
		// rendering loop
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
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