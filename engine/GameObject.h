#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include <ranges>		// ¡Ø Notice: Only C++20 or later only: reverse range
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

	void draw(const int& textureNumber);
	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}

	void isCollision(std::vector<GameObject*> objects)
	{
		bool isLeftCollision = false;
		bool isRightCollision = false;
		bool isDownCollision = false;

		// ¡Ø Notice: Only C++20 or later only
		auto r = objects | std::views::filter([this](GameObject* g) {return
			mPositionX - 2.0f <= g->mPositionX && g->mPositionX <= mPositionX + 2.0f &&
			mPositionY - 2.0f <= g->mPositionY && g->mPositionY <= mPositionY + 2.0f; }) | std::views::reverse;

		for (GameObject* g : r)
		{
			if (this == g)
				continue;


			// bottom collision
			if (g->mPositionY + 1.5f <= mPositionY)
			{
				bool isDownLeftCollision = false;
				bool isDownRightCollision = false;
				bool isPreBottomCollision = isBottomCollision;

				if (mPositionX - 1.0f <= g->mPositionX && g->mPositionX <= mPositionX + 1.0f)
					isDownCollision = true;


				if (!isLeftCollision && !isRightCollision)
				{
					if (g->mPositionX < mPositionX - 1.0f)
					{
						if (mPositionX - 1.8f <= g->mPositionX)
							isDownLeftCollision = true;
					}
					else if (mPositionX + 1.0f < g->mPositionX)
					{
						if (g->mPositionX <= mPositionX + 1.8f)
							isDownRightCollision = true;
					}
				}

				isBottomCollision = (isDownCollision || (isDownLeftCollision || isDownRightCollision)) ? true : false;

				if (isBottomCollision)
				{
					if (mPositionY < g->mPositionY + 2.0f)
						mPositionY = g->mPositionY + 2.0f;

					// jumpStatus = NO_JUMP;
					// moveStatus = STAND;
				}
				//else if ((jumpStatus == NO_JUMP) && isPreBottomCollision)
					// jumpStatus = JUMP_FALL;

			}
			// top collision
			else if (g->mPositionX - 1.0f <= mPositionX && mPositionX <= g->mPositionX + 1.0f)
			{
				if (g->mPositionY - 2.0f <= mPositionY)
				{
					mPositionY = g->mPositionY - 2.0f;
					// jumpStatus = JUMP_FALL;
				}
			}
			// right collision
			else if (mPositionX < g->mPositionX - 1.0f)
			{
				isRightCollision = true;

				if (g->mPositionX - 2.0f <= mPositionX)
					mPositionX = g->mPositionX - 2.0f;
			}
			// left collision
			else if (g->mPositionX + 1.0f < mPositionX)
			{
				isLeftCollision = true;

				if (mPositionX <= g->mPositionX + 2.0f)
					mPositionX = g->mPositionX + 2.0f;
			}
		}

	} // void isCollision(std::vector<GameObject*> obj)
};