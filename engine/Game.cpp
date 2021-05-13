#include "Game.h"

float positionX = 0.0f;
float positionY = 0.0f;
float moveX = 0.0f;
float moveY = 0.0f;
const float delta = 0.5f;

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

Game::Game()
{
	assert(glfwInit());

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);


	glfwMakeContextCurrent(glfwWindow);
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	glfwSetWindowPos(glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);


	assert(glfwWindow);
	glfwSwapInterval(1); // 60fps


	// Init GLEW
	glewExperimental = true;
	assert(glewInit() == GLEW_OK);


	glViewport(0, 0, width, height);
	const float aspect_ratio = (float)width / (float)height;
	glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

	
	mGameObject = new GameObject();


	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl;
	std::cout << "Press Escape key to exit" << std::endl;
}

void Game::update()
{
	positionX += moveX;
	positionY += moveY;
}

void Game::draw()
{
	glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//mResource->draw(0);

	glFlush();
}

void Game::run()
{
	while (!glfwWindowShouldClose(glfwWindow))
	{
		update();
		draw();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}