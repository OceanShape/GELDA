#include "Game.h"

Game::Game(const std::string title, int width, int height)
{
	assert(glfwInit());

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);


	glfwMakeContextCurrent(glfwWindow);
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


	// Init GameObejcts
	std::string name[] = { "./sample_sprite.png" };
	mGameObject = new GameObject(name);


	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl;
	std::cout << "Press Escape key to exit" << std::endl;
}

void Game::update()
{
	if (isKeyDown(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);

	if (isKeyDown(GLFW_KEY_LEFT))		moveX = -delta;
	else if (isKeyUp(GLFW_KEY_LEFT))	moveX = 0.0f;
	else if (isKeyDown(GLFW_KEY_RIGHT))	moveX = delta;
	else if (isKeyUp(GLFW_KEY_RIGHT))	moveX = 0.0f;
	else if (isKeyDown(GLFW_KEY_UP))	moveY = delta;
	else if (isKeyUp(GLFW_KEY_UP))		moveY = 0.0f;
	else if (isKeyDown(GLFW_KEY_DOWN))	moveY = -delta;
	else if (isKeyUp(GLFW_KEY_DOWN))	moveY = 0.0f;

	mGameObject->move(moveX, moveY);

	updateKeyStatus();
}

void Game::draw()
{
	glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	mGameObject->draw(0); // for Test

	glFlush();
}

void Game::run()
{
	while (!glfwWindowShouldClose(glfwWindow))
	{
		update();
		draw();

		updateKeyStatus();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Game::updateKeyStatus()
{
	for (const auto& [key, value] : keyStatus)
	{
		if (glfwGetKey(glfwWindow, key) == GLFW_PRESS)
		{
			keyStatus[key] = true;
		}
		else if (glfwGetKey(glfwWindow, key) == GLFW_RELEASE)
		{
			keyStatus[key] = false;
		}
	}
}

bool Game::isKeyPressed(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
}

bool Game::isKeyReleased(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_RELEASE;
}

bool Game::isKeyUp(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_RELEASE && keyStatus[key];
}

bool Game::isKeyDown(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_PRESS && !keyStatus[key];
}