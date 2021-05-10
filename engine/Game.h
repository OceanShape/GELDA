#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Resource.h"

class Game
{
private:
	GLFWwindow* glfwWindow = nullptr;
	int width = 512;
	int height = 512;
	std::string title = "GELDA";
	Resource* mResource;

public:
	Game();
	~Game()
	{
		delete mResource;
	}
	void update();
	void draw();
	void run();
};

class GameObject
{
private:
	float mPosX; // position x
	float mPosY; // position y
	
};