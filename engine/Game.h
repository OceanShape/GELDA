#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"

class Game
{
private:
	GLFWwindow* glfwWindow = nullptr;
	GameObject* mGameObject = nullptr;

	std::map<int, bool> keyStatus;
	float moveX = 0.0f;
	float moveY = 0.0f;
	const float delta = 0.5f;

public:
	Game(const std::string title, int width, int height);
	~Game()
	{
	}

	void run();		// entire game loop
	void update();	// update input and gameobject status
	void draw();	// draw all gameobjects

private:
	void updateKeyStatus();
	bool isKeyPressed(const int& key);
	bool isKeyReleased(const int& key);
	bool isKeyUp(const int& key);
	bool isKeyDown(const int& key);
};