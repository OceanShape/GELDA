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
protected:
	GLuint* mTexture;
	int mTextureCount;
	int mCurrentFrame = 0;
	float mTextureRate = 0.0625f;

	float mPositionX = 0.0f;
	float mPositionY = 0.0f;

public:
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

	float getPositionX()
	{
		return mPositionX;
	}

	float getPositionY()
	{
		return mPositionY;
	}

	void draw(const int& textureNumber);
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
	virtual void setMode(const bool& isEditorMode)
	{}
	virtual void update(const bool& isEditorMode, const std::vector<Object*>& object)
	{}
	virtual void inputControl(const eInputStatus& input)
	{}
};