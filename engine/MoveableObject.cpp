#include "MoveableObject.h"

GLuint MoveableObject::getTexture(bool& isRightSide)
{
	isRightSide = (mDirectionStatus == eDirectionStatus::RIGHT);
	return (*mTexture)[mCurrentFrame / 30];
}
void MoveableObject::setMode(const bool& isEditorMode)
{
	if (isEditorMode == true)
	{
		mCurrentFrame = 0;

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
void MoveableObject::update(const bool& isEditorMode, const std::vector<Object*>& objects)
{
	if (isEditorMode == false)
	{
		// update frame
		++mCurrentFrame;
		if (mCurrentFrame == 60)
		{
			mCurrentFrame = 0;
		}

		// update gravity status
		if (mJumpStatus == eJumpStatus::JUMP)
		{
			isOnPlatform = false;
			mPositionY += 0.6f;
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

			float upSpeed = 0.8f + (gravityY * jumpDecelerationSpendTime);

			if (upSpeed < -0.6f)
			{
				mJumpStatus = eJumpStatus::FALL;
			}
			mPositionY += upSpeed;
		}

		// update collision status

		// ¡Ø Notice: Only C++20 or later only
		auto reverseObjects = objects | std::views::filter([this](Object* g) {return
			mPositionX - 2.0f < g->getPositionX() && g->getPositionX() < mPositionX + 2.0f &&
			mPositionY - 2.0f < g->getPositionY() && g->getPositionY() < mPositionY + 2.0f; }) | std::views::reverse;

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

		for (Object* g : reverseObjects)
		{
			if (this == g)
			{
				continue;
			}

			float posX = g->getPositionX();
			float posY = g->getPositionY();

			// bottom collision
			if (mPositionY - 1.3f > posY)
			{
				if (posX < mPositionX + 1.0f && mPositionX - 1.0f < posX)
				{
					isDownCollision = true;
				}
				else if (isLeftCollision == false && isRightCollision == false)
				{
					if (posX - mPositionX < 1.8f)
					{
						isDownLeftCollision = true;
					}
					else if (mPositionX - posX < 1.8f)
					{
						isDownRightCollision = true;
					}
				}

				if (posY > highestBottomPlatformY)
				{
					highestBottomPlatformY = posY;
				}
			}
			// left collision
			else if (mPositionY - 0.9f < posY && mPositionX - 1.6f > posX)
			{
				isLeftCollision = true;
				leftPlatformX = posX;
			}
			// right collision
			else if (mPositionY - 0.9f < posY && mPositionX + 1.6f < posX)
			{
				isRightCollision = true;
				rightPlatformX = posX;
			}
			// top collision
			else if (mPositionY + 1.2f < posY)
			{
				isTopCollision = true;
				lowestTopPlatformY = posY;
				mJumpStatus = eJumpStatus::FALL;
			}
		}

		if (isRightCollision == true)
		{
			mPositionX = rightPlatformX - 2.0f;
		}

		if (isLeftCollision == true)
		{
			mPositionX = leftPlatformX + 2.0f;
		}



		if (isTopCollision == true)
		{
			mPositionY = lowestTopPlatformY - 2.0f;
		}

		isBottomCollision = isDownCollision || (isDownLeftCollision || isDownRightCollision);

		if (isBottomCollision == true)
		{
			if (mPositionY < highestBottomPlatformY + 2.0f)
			{
				mPositionY = highestBottomPlatformY + 2.0f;
			}
			jumpDecelerationSpendTime = 0.0f;
			jumpStartPositionY = mPositionY;
			mJumpStatus = eJumpStatus::NO_JUMP;
			mMoveStatus = eMoveStatus::STOP;

			isOnPlatform = true;
		}
		else if (mJumpStatus == eJumpStatus::NO_JUMP && isPreBottomCollision == true)
		{
			mJumpStatus = eJumpStatus::FALL;
		}
	}

	mPositionY = (static_cast<int>(mPositionY * 10.0f) / 10.0f);
}
void MoveableObject::inputControl(const eInputStatus& input)
{
	if (input == eInputStatus::LEFT)
	{
		mPositionX += -0.3f;
		mMoveStatus = eMoveStatus::MOVE;
		mDirectionStatus = eDirectionStatus::LEFT;
	}
	else if (input == eInputStatus::RIGHT)
	{
		mPositionX += 0.3f;
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
		//if (mJumpStatus == eJumpStatus::NO_JUMP)
		if ((mJumpStatus == eJumpStatus::NO_JUMP || mJumpStatus == eJumpStatus::FALL)
			&& isOnPlatform == true && (jumpStartPositionY - mPositionY <= 0.05f))
		{
			isOnPlatform = false;
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