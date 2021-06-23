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

#define LENGTH 4096

class Game
{
protected:
	GLFWwindow* glfwWindow = nullptr;
	std::vector<Object*> mObject;

	std::vector<std::vector<GLuint>*> mTexture;
	std::map<int, bool> keyStatus;

	bool isEditorMode = true;
	Object* mControllable = nullptr;

public:
	Game(const std::string& title, int width, int height, const std::string& resourceDir, const std::string& objectDir);

	~Game()
	{
		for (Object* o : mObject)
		{
			delete o;
		}

		for (std::vector<GLuint>* t : mTexture)
		{
			delete t;
		}
	}

	void run();										// Entire game loop
	virtual void update() {};						// Update input and game object status
	void draw();									// Draw all game objects
	void initTexture(const std::string& dir);		// Initialize textures
	void initObject(const std::string& dir);	// Initialize game objects(texture, position)

protected:
	void updateKeyStatus();
	bool isKeyPressed(const int& key);
	bool isKeyReleased(const int& key);
	bool isKeyUp(const int& key);
	bool isKeyDown(const int& key);

};