#pragma once
#include <iostream>
#include <vector>
#include <map>

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
	std::vector<GameObject> v;
	GameObject* mGameObject;

	std::map<int, bool> keyStatus;

public:
	Game();
	~Game()
	{
		delete mGameObject;
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