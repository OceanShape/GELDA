#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include <ranges>		// reverse range(¡Ø Notice: Only C++20 or later only)
#include <algorithm>	// for view
#include <cfloat>

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
	float jumpDecelerationSpendTime = 0.0f;

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

	void input(const eInputStatus& input);
	void setMode(const bool& isEditorMode)
	{
		if (isEditorMode == true)
		{
			mDirectionStatus = eDirectionStatus::RIGHT;
			mMoveStatus = eMoveStatus::STOP;
			mJumpStatus = eJumpStatus::NO_JUMP;
		}
		else
		{
			mDirectionStatus = eDirectionStatus::RIGHT;
			mMoveStatus = eMoveStatus::STOP;
			mJumpStatus = eJumpStatus::FALL;
		}
	}
	void update(const bool& isEditorMode, const std::vector<GameObject*>& object)
	{
		if (isEditorMode == false)
		{
			updateGravity();
		}

		// update game object physics status
		if (isEditorMode == false)
		{
			updateCollision(object);
		}

		mPositionY = (static_cast<int>(mPositionY * 10.0f) / 10.0f);
	}
	void updateGravity();
	void updateCollision(const std::vector<GameObject*>& objects);
	void draw(const int& textureNumber);
};