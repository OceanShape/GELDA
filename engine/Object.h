#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include <ranges>		// reverse range(�� Notice: Only C++20 or later only)
#include <algorithm>	// for view
#include <cfloat>

#include "CollisionDirection.h"
#include "Status.h"

class Object
{
protected:
	std::vector<GLuint>* mTexture;
	size_t mCurrentFrame = 0;
	float mTextureRate = 0.0625f;

	float mPositionX;
	float mPositionY;

public:
	Object(std::vector<GLuint>* texture, const float& posX, const float& posY) : mTexture(texture)
	{
		mPositionX = posX;
		mPositionY = posY;
	}

	~Object()
	{
	}

	float getPositionX()
	{
		return mPositionX;
	}

	float getPositionY()
	{
		return mPositionY;
	}


	void inputEditor(const eInputStatus& input)
	{
		if (input == eInputStatus::LEFT)
		{
			mPositionX += -0.5f;
			mPositionY += 0.0f;
		}
		else if (input == eInputStatus::RIGHT)
		{
			mPositionX += 0.5f;
			mPositionY += 0.0f;
		}
		else if (input == eInputStatus::UP)
		{
			mPositionX += 0.0f;
			mPositionY += 0.5f;
		}
		else if (input == eInputStatus::DOWN)
		{
			mPositionX += 0.0f;
			mPositionY += -0.5f;
		}
		else if (input == eInputStatus::ARROW_RELEASE)
		{
			mPositionX += 0.0f;
			mPositionY += 0.0f;
		}
	}
	virtual void inputControl(const eInputStatus& input) {}

	void draw(const int& textureNumber);
	virtual GLuint getTexture(bool& isRightSide)
	{
		return (*mTexture)[0];
	}
	virtual void setMode(const bool& isEditorMode)
	{
		mCurrentFrame = 0;
	}
	virtual void update(const bool& isEditorMode, const std::vector<Object*>& object)
	{
		++mCurrentFrame;
		if (mCurrentFrame == 60)
		{
			mCurrentFrame = 0;
		}
	}

};