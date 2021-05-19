#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GameObject.h"

class Game
{
private:
	GLFWwindow* glfwWindow = nullptr;
	std::vector<GameObject*> mGameObject;
	std::vector<GLuint*> mTexture;

	std::map<int, bool> keyStatus;
	float moveX = 0.0f;
	float moveY = 0.0f;
	const float delta = 0.5f;

public:
	Game(const std::string title, int width, int height);
	~Game()
	{
		for (GameObject* p : mGameObject)
			delete p;
	}

	void run();		// entire game loop
	void update();	// update input and gameobject status
	void draw();	// draw all gameobjects
	void initTexture();

private:
	void updateKeyStatus();
	bool isKeyPressed(const int& key);
	bool isKeyReleased(const int& key);
	bool isKeyUp(const int& key);
	bool isKeyDown(const int& key);
};