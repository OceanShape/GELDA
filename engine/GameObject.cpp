#include "GameObject.h"

void GameObject::input(const eInputStatus& input)
{
	if (input == eInputStatus::DEBUG_LEFT)
	{
		deltaX = -0.5f;
	}
	else if (input == eInputStatus::DEBUG_RIGHT)
	{
		deltaX = 0.5f;
	}
	else if (input == eInputStatus::DEBUG_UP)
	{
		deltaY = 0.5f;
	}
	else if (input == eInputStatus::DEBUG_DOWN)
	{
		deltaY = -0.5f;
	}
	else if (input == eInputStatus::DEBUG_ARROW_RELEASE)
	{
		deltaX = 0.0f;
		deltaY = 0.0f;
	}
	else if (input == eInputStatus::LEFT)
	{
		deltaX = -0.5f;
		mMoveStatus = eMoveStatus::MOVE;
		mDirectionStatus = eDirectionStatus::LEFT;
	}
	else if (input == eInputStatus::RIGHT)
	{
		deltaX = 0.5f;
		mMoveStatus = eMoveStatus::MOVE;
		mDirectionStatus = eDirectionStatus::RIGHT;
	}
	else if (input == eInputStatus::ARROW_RELEASE)
	{
		deltaX = 0.0f;
		deltaY = 0.0f;
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
		if (mPositionY - jumpStartPositionY > 3.5f * 2.0f)
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

		mPositionY += 0.8f + (gravityAccelerationY / 2 * jumpDecelerationSpendTime);
		if (jumpDecelerationStartPositionY > mPositionY)
		{
			mJumpStatus = eJumpStatus::FALL;
		}
	}
}

void GameObject::updateCollision(const std::vector<GameObject*>& objects)
{
	// ¡Ø Notice: Only C++20 or later only
	auto reverseObjects = objects | std::views::filter([this](GameObject* g) {return
		mPositionX - 2.0f <= g->mPositionX && g->mPositionX <= mPositionX + 2.0f &&
		mPositionY - 2.0f <= g->mPositionY && g->mPositionY <= mPositionY + 2.0f; }) | std::views::reverse;


	bool isLeftCollision = false;
	bool isRightCollision = false;
	bool isDownCollision = false;

	for (GameObject* g : reverseObjects)
	{
		if (this == g)
		{
			continue;
		}

		// bottom collision
		if (g->mPositionY + 1.5f <= mPositionY)
		{
			bool isDownLeftCollision = false;
			bool isDownRightCollision = false;
			bool isPreBottomCollision = isBottomCollision;

			if (mPositionX - 1.0f <= g->mPositionX && g->mPositionX <= mPositionX + 1.0f)
			{
				isDownCollision = true;
			}

			if (isLeftCollision == false && isRightCollision == false)
			{
				if (g->mPositionX < mPositionX - 1.0f)
				{
					if (mPositionX - 1.8f <= g->mPositionX)
					{
						isDownLeftCollision = true;
					}
				}
				else if (mPositionX + 1.0f < g->mPositionX)
				{
					if (g->mPositionX <= mPositionX + 1.8f)
					{
						isDownRightCollision = true;
					}
				}
			}

			isBottomCollision = (isDownCollision || (isDownLeftCollision || isDownRightCollision)) ? true : false;

			if (isBottomCollision == true)
			{
				if (mPositionY < g->mPositionY + 2.0f)
				{
					mPositionY = g->mPositionY + 2.0f;
				}

				
				jumpDecelerationSpendTime = 0.0f;
				jumpStartPositionY = mPositionY;
				
				mJumpStatus = eJumpStatus::NO_JUMP;
				mMoveStatus = eMoveStatus::STOP;
			}
			else if ((mJumpStatus == eJumpStatus::NO_JUMP) && isPreBottomCollision == true)
			{
				mJumpStatus = eJumpStatus::FALL;
			}

		}
		// top collision
		else if (g->mPositionX - 1.0f <= mPositionX && mPositionX <= g->mPositionX + 1.0f)
		{
			if (g->mPositionY - 2.0f <= mPositionY)
			{
				mPositionY = g->mPositionY - 2.0f;
				mJumpStatus = eJumpStatus::FALL;
			}
		}
		// right collision
		else if (mPositionX < g->mPositionX - 1.0f)
		{
			isRightCollision = true;

			if (g->mPositionX - 2.0f <= mPositionX)
			{
				mPositionX = g->mPositionX - 2.0f;
			}
		}
		// left collision
		else if (g->mPositionX + 1.0f < mPositionX)
		{
			isLeftCollision = true;

			if (mPositionX <= g->mPositionX + 2.0f)
			{
				mPositionX = g->mPositionX + 2.0f;
			}
		}
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