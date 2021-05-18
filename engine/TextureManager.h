#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <cassert>

class TextureManager
{
private:
	static TextureManager* mInstance;
	static std::vector<GLuint*> mTextures;

	TextureManager();
	~TextureManager()
	{
		for (GLuint* p : mTextures)
			delete[] p;
	}
public:
	static GLuint* getTexture(const int& objectNumber);
};