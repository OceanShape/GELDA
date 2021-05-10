#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

class Resource
{
public:
	GLuint* mTexture = nullptr;
	int mTextureCount = 0;

	Resource(const std::string fileName[], int resourceCount)
	{
		mTextureCount = resourceCount;
		init(fileName);
	}

	void draw(int textureNumber);

private:
	void init(const std::string fileName[]);
};