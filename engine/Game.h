#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"

class Game
{
private:
	GLFWwindow* glfwWindow = nullptr;
	int width = 512;
	int height = 512;
	std::string title = "GELDA";
	GameObject* mGameObject;

public:
	Game();
	~Game()
	{
		delete mGameObject;
	}
	void update();
	void draw();
	void run();
};