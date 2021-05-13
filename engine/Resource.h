#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

class Resource
{
public:
	unsigned int* mTexture;
	int mTextureCount;

	Resource(const std::string fileName[], int resourceCount)
	{
		mTextureCount = resourceCount;
		init(fileName);
	}

	~Resource()
	{
		delete[] mTexture;
	}

	void draw(int textureNumber);

private:
	void init(const std::string fileName[]);
};