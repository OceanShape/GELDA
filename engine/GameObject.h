#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>

class GameObject
{
private:
	GLuint* mTexture;
	int mTextureCount;
	float mTextureRate = 0.0625f;


	float mPositionX = 0.0f;
	float mPositionY = 0.0f;


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

	void draw(const int& textureNumber);
	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}

	int isCollision(GameObject* obj)
	{
		//(x1, y1)------(x2, y1)
		//	|			    |
		//	|			    |
		//	|			    |
		//	|				|
		//(x1, y2)------(x2, y2)

		float r1x1 = mPositionX - 1.0f;
		float r1x2 = mPositionX + 1.0f;
		float r1y1 = mPositionY + 1.0f;
		float r1y2 = mPositionY - 1.0f;

		float r2x1 = obj->mPositionX - 1.0f;
		float r2x2 = obj->mPositionX + 1.0f;
		float r2y1 = obj->mPositionY + 1.0f;
		float r2y2 = obj->mPositionY - 1.0f;
		

		if (
			((r1x1 <= r2x1 && r2x1 <= r1x2) || (r1x1 <= r2x2 && r2x2 <= r1x2))
			&& ((r1y2 <= r2y1 && r2y1 <= r1y1) || (r1y2 <= r2y2 && r2y2 <= r1y1))
			)
		{
			std::cout << "collision detected" << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}

};