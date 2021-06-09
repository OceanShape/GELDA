#pragma once
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Object.h"
#include "MoveableObject.h"

#define LENGTH 1024

class Game
{
private:
	GLFWwindow* glfwWindow = nullptr;
	std::vector<Object*> mObject;
	std::vector<GLuint*> mTexture;
	std::vector<GLuint> mTextureCount;
	std::map<int, bool> keyStatus;

	bool isEditorMode = true;
	Object* mControllable;

public:
	Game(const std::string& title, int width, int height, const std::string& resourceDir, const std::string& objectDir);

	~Game()
	{
		for (Object* p : mObject)
		{
			delete p;
		}

		for (GLuint* p : mTexture)
		{
			free(p);
		}
	}

	void run();										// Entire game loop
	void update();									// Update input and game object status
	void draw();									// Draw all game objects
	void initTexture(const std::string& dir);		// Initialize textures
	void initObject(const std::string& dir);	// Initialize game objects(texture, position)

private:
	void updateKeyStatus();
	bool isKeyPressed(const int& key);
	bool isKeyReleased(const int& key);
	bool isKeyUp(const int& key);
	bool isKeyDown(const int& key);

};