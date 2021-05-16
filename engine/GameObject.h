#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

class GameObject
{
private:
	float mPositionX = 0.0f;
	float mPositionY = 0.0f;
	GLuint* mTexture;
	int mTextureCount;
	float gTextureSize = 0.0625f;

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

	void initTexture(const std::string fileName[]);
	void draw(const int& textureNumber);
	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}
};