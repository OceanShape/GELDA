#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include <ranges>		// reverse range(¡Ø Notice: Only C++20 or later only)
#include <algorithm>	// for view

#include "CollisionDirection.h"
#include "Status.h"

class GameObject
{
private:
	GLuint* mTexture;
	int mTextureCount;
	float mTextureRate = 0.0625f;


	float mPositionX = 0.0f;
	float mPositionY = 0.0f;

	float gravityAccelerationY = -0.15f;
	float jumpStartPositionY = 0.0f;
	float jumpDecelerationStartPositionY = 0.0f;

	bool isBottomCollision = false;
	bool isJumpDeceleration = false;

	eDirectionStatus mDirectionStatus = eDirectionStatus::RIGHT;
	eMoveStatus mMoveStatus = eMoveStatus::STOP;
	eJumpStatus mJumpStatus = eJumpStatus::FALL;

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

	void move(const float& deltaX, const float& deltaY)
	{
		mPositionX += deltaX;
		mPositionY += deltaY;
	}
	void updateGravity(const bool& isJumpPressed);
	void updateCollision(const std::vector<GameObject*>& objects);
	void draw(const int& textureNumber);
};