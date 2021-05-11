#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Resource.h"

class GameObject
{
private:
	float mPosX = 0.0f; // position x
	float mPosY = 0.0f; // position y
	Resource* mResource;

public:
	GameObject()
	{
		std::string name[] = { "./sample_sprite.png" };
		mResource = new Resource(name, 1);
	}

	~GameObject()
	{
		delete mResource;
	}
};

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