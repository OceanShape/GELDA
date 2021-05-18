#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

#include "TextureManager.h"

class GameObject
{
private:
	GLuint* mTexture;
	int mTextureCount;
	float mTextureSize = 0.0625f;
	

	float mPositionX = 0.0f;
	float mPositionY = 0.0f;
	

public:
	GameObject(const int& objectNumber)
	{
		mTextureCount = 1; // for Test
		mTexture = TextureManager::getTexture(objectNumber);

	}

	GameObject(const int& objectNumber, const float& posX, const float& posY)
	{
		mTextureCount = 1; // for Test
		mTexture = TextureManager::getTexture(objectNumber);
		mPositionX = posX;
		mPositionY = posY;
	}

	~GameObject()
	{
	}

	void draw(const int& textureNumber);
	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}
};