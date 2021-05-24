#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include <ranges>		// reverse range(¡Ø Notice: Only C++20 or later only)
#include <algorithm>	// for view

#include "CollisionDirection.h"

class GameObject
{
private:
	GLuint* mTexture;
	int mTextureCount;
	float mTextureRate = 0.0625f;

	float mPositionX = 0.0f;
	float mPositionY = 0.0f;

	bool isBottomCollision = false;

public:
	GameObject(GLuint* texture, const float& posX, const float& posY)
	{
		mTextureCount = 1; // for Test
		mTexture = texture;
		mPositionX = posX;
		mPositionY = posY;
	}

	~GameObject()
	{
	}

	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}
	void updateGravity();
	void updateCollision(std::vector<GameObject*> objects);
	void draw(const int& textureNumber);
};