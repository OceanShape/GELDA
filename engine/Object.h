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

class Object
{
private:
	GLuint* mTexture;
	int mTextureCount;
	int mCurrentFrame = 0;

	float mTextureRate = 0.0625f;


	float mPositionX = 0.0f;
	float mPositionY = 0.0f;

	float gravityAccelerationY = -0.10f;
	float jumpStartPositionY = 0.0f;
	float jumpDecelerationStartPositionY = 0.0f;
	float jumpDecelerationSpendTime = 0.0f;

	bool isBottomCollision = false;
	bool isJumpDeceleration = false;

	eDirectionStatus mDirectionStatus = eDirectionStatus::RIGHT;
	eMoveStatus mMoveStatus = eMoveStatus::STOP;

public:
	eJumpStatus mJumpStatus = eJumpStatus::FALL;

	Object(GLuint* texture, const float& posX, const float& posY)
	{
		mTextureCount = 1; // for Test
		mTexture = texture;
		mPositionX = posX;
		mPositionY = posY;
	}

	~Object()
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
	void update(const bool& isEditorMode, const std::vector<Object*>& object)
	{
		if (isEditorMode == false)
		{
			updateGravity();
			updateCollision(object);
		}

		mPositionY = (static_cast<int>(mPositionY * 10.0f) / 10.0f);
	}
	void updateGravity();
	void updateCollision(const std::vector<Object*>& objects);
	void draw(const int& textureNumber);
};