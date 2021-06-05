#include "GameObject.h"

void GameObject::input(const eInputStatus& input)
{
	if (input == eInputStatus::DEBUG_LEFT)
	{
		mPositionX += -0.5f;
		mPositionY += 0.0f;
	}
	else if (input == eInputStatus::DEBUG_RIGHT)
	{
		mPositionX += 0.5f;
		mPositionY += 0.0f;
	}
	else if (input == eInputStatus::DEBUG_UP)
	{
		mPositionX += 0.0f;
		mPositionY += 0.5f;
	}
	else if (input == eInputStatus::DEBUG_DOWN)
	{
		mPositionX += 0.0f;
		mPositionY += -0.5f;
	}
	else if (input == eInputStatus::DEBUG_ARROW_RELEASE)
	{
		mPositionX += 0.0f;
		mPositionY += 0.0f;
	}
	else if (input == eInputStatus::LEFT)
	{
		mPositionX += -0.1f;
		mMoveStatus = eMoveStatus::MOVE;
		mDirectionStatus = eDirectionStatus::LEFT;
	}
	else if (input == eInputStatus::RIGHT)
	{
		mPositionX += 0.1f;
		mMoveStatus = eMoveStatus::MOVE;
		mDirectionStatus = eDirectionStatus::RIGHT;
	}
	else if (input == eInputStatus::ARROW_RELEASE)
	{
		mPositionX += 0.0f;
		mMoveStatus = eMoveStatus::STOP;
	}
	else if (input == eInputStatus::JUMP_PRESS)
	{
		if (mJumpStatus == eJumpStatus::NO_JUMP)
		{
			jumpStartPositionY = mPositionY;
			mJumpStatus = eJumpStatus::JUMP;
		}
	}
	else if (input == eInputStatus::JUMP_RELEASE)
	{
		if (mJumpStatus == eJumpStatus::JUMP)
		{
			jumpDecelerationStartPositionY = mPositionY;
			mJumpStatus = eJumpStatus::JUMP_DECELERATION;
		}
	}
}

void GameObject::updateGravity()
{

	if (mJumpStatus == eJumpStatus::JUMP)
	{
		mPositionY += 0.8f;
		if (mPositionY - jumpStartPositionY > 3.0f * 2.0f)
		{
			jumpDecelerationStartPositionY = mPositionY;
			mJumpStatus = eJumpStatus::JUMP_DECELERATION;
		}
	}
	else if (mJumpStatus == eJumpStatus::FALL)
	{
		mPositionY -= 0.6f;
	}
	else if (mJumpStatus == eJumpStatus::JUMP_DECELERATION)
	{
		jumpDecelerationSpendTime += 1.0f;

		float upSpeed = 0.8f + (gravityAccelerationY * jumpDecelerationSpendTime);

		if (upSpeed < 0.0f)
		{
			mJumpStatus = eJumpStatus::FALL;
		}
		mPositionY += upSpeed;
	}
}

void GameObject::updateCollision(const std::vector<GameObject*>& objects)
{
	// ¡Ø Notice: Only C++20 or later only
	auto reverseObjects = objects | std::views::filter([this](GameObject* g) {return
		mPositionX - 2.0f < g->mPositionX && g->mPositionX < mPositionX + 2.0f &&
		mPositionY - 2.0f < g->mPositionY && g->mPositionY < mPositionY + 2.0f; }) | std::views::reverse;

	bool isTopCollision = false;
	bool isLeftCollision = false;
	bool isRightCollision = false;
	bool isDownCollision = false;
	bool isDownLeftCollision = false;
	bool isDownRightCollision = false;
	bool isPreBottomCollision = isBottomCollision;

	float leftPlatformX = -FLT_MAX;
	float rightPlatformX = FLT_MAX;
	float lowestTopPlatformY = FLT_MAX;
	float highestBottomPlatformY = -FLT_MAX;

	for (GameObject* g : reverseObjects)
	{
		if (this == g)
		{
			continue;
		}

		// bottom collision
		if (mPositionY - 1.0f > g->mPositionY)
		{
			if (g->mPositionX < mPositionX + 1.0f && mPositionX - 1.0f < g->mPositionX)
			{
				isDownCollision = true;
			}
			else if (isLeftCollision == false && isRightCollision == false)
			{
				if (g->mPositionX - mPositionX < 1.8f)
				{
					isDownLeftCollision = true;
				}
				else if (mPositionX - g->mPositionX < 1.8f)
				{
					isDownRightCollision = true;
				}
			}

			if (g->mPositionY > highestBottomPlatformY)
			{
				highestBottomPlatformY = g->mPositionY;
			}
		}
		// right collision
		else if (mPositionX + 1.8f < g->mPositionX)
		{
			isRightCollision = true;
			rightPlatformX = g->mPositionX;
		}
		// left collision
		else if (mPositionX - 1.8f > g->mPositionX)
		{
			isLeftCollision = true;
			leftPlatformX = g->mPositionX;
		}
		// top collision
		else if (mPositionY + 1.8f < g->mPositionY)
		{
			isTopCollision = true;
			lowestTopPlatformY = g->mPositionY;
			mJumpStatus = eJumpStatus::FALL;
		}
	}

	isBottomCollision = (isDownCollision || (isDownLeftCollision || isDownRightCollision)) ? true : false;

	if (isLeftCollision == true)
	{
		mPositionX = leftPlatformX + 2.0f;
	}
	else if (isRightCollision == true)
	{
		mPositionX = rightPlatformX - 2.0f;
	}
	else if (isTopCollision == true)
	{
		mPositionY = lowestTopPlatformY - 2.0f;
	}
	else if (isBottomCollision == true)
	{
		if (mPositionY < highestBottomPlatformY + 2.0f)
		{
			mPositionY = highestBottomPlatformY + 2.0f;
		}

		jumpDecelerationSpendTime = 0.0f;
		jumpStartPositionY = mPositionY;

		mJumpStatus = eJumpStatus::NO_JUMP;
		mMoveStatus = eMoveStatus::STOP;
	}
	else if (mJumpStatus == eJumpStatus::NO_JUMP && isPreBottomCollision == true)
	{
		mJumpStatus = eJumpStatus::FALL;
	}
}

void GameObject::draw(const int& textureNumber)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	{
		glScalef(mTextureRate, mTextureRate, mTextureRate);

		glBindTexture(GL_TEXTURE_2D, mTexture[textureNumber]);

		glBegin(GL_QUADS);

		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(mPositionX - 1.0f, mPositionY + 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(mPositionX - 1.0f, mPositionY - 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(mPositionX + 1.0f, mPositionY - 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(mPositionX + 1.0f, mPositionY + 1.0f);

		glEnd();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
}