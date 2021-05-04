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

	int display_w, display_h;

	glfwMakeContextCurrent(glfwWindow);
	glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);

	width = display_w;
	height = display_h;

	glfwSetWindowPos(glfwWindow, (mode->width - display_w) / 2, (mode->height - display_h) / 2);

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
		glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glPopMatrix();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "See U GELDA!" << endl;

	return 0;
}