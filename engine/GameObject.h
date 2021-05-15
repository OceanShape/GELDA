#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

class GameObject
{
private:
	float mPosX = 0.0f; // position x
	float mPosY = 0.0f; // position y
	GLuint* mTexture;
	int mTextureCount;

public:
	GameObject()
	{
		std::string name[] = { "./sample_sprite.png" };
		mTextureCount = 1; // for Test
		initTexture(name);
	}

	~GameObject()
	{
	}

	void draw(int textureNumber);

	void initTexture(const std::string fileName[]);
};