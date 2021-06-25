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

	// check collision status

	// ¡Ø Notice: Only C++20 or later only
	auto reverseObjects = objects | std::views::filter([this](Object* g) {return
		mPositionX - 2.0f < g->getPositionX() && g->getPositionX() < mPositionX + 2.0f &&
		mPositionY - 2.0f < g->getPositionY() && g->getPositionY() < mPositionY + 2.0f; }) | std::views::reverse;

	// [][Top][][Left][Right][DownLeft][Down][DownRight]
	bool isTopCollision = false;
	bool isLeftCollision = false;
	bool isRightCollision = false;
	bool isDownCollision = false;
	bool isDownLeftCollision = false;
	bool isDownRightCollision = false;

	bool isPreBottomCollision = isBottomCollision;

	float leftPlatformX = -FLT_MAX;
	float rightPlatformX = FLT_MAX;
	float topPlatformY = FLT_MAX;
	float bottomPlatformY = -FLT_MAX;

	Object* leftObj = nullptr;
	Object* rightObj = nullptr;
	Object* topObj = nullptr;
	Object* bottomObj = nullptr;

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
			else if (mJumpStatus == eJumpStatus::FALL
				&& isLeftCollision == false && isRightCollision == false)
			{
				if (mPositionX + 1.9f > posX)
				{
					isDownLeftCollision = true;
				}
				else if (mPositionX - 1.9f < posX)
				{
					isDownRightCollision = true;
				}
			}

			bottomObj = g;
		}
		// left collision
		else if (mPositionY - 0.9f < posY && mPositionX - 1.6f > posX)
		{
			isLeftCollision = true;
			//leftPlatformX = posX;
			leftObj = g;
		}
		// right collision
		else if (mPositionY - 0.9f < posY && mPositionX + 1.6f < posX)
		{
			isRightCollision = true;
			//rightPlatformX = posX;
			rightObj = g;
		}
		// top collision
		else if (mPositionY + 1.2f < posY)
		{
			isTopCollision = true;
			//topPlatformY = posY;
			topObj = g;
			mJumpStatus = eJumpStatus::FALL;
		}
	}



	//if (isRightCollision == true)
	if (rightObj != nullptr)
	{
		//mPositionX = rightPlatformX - 2.0f;
		mPositionX = rightObj->getPositionX() - 2.0f;
	}

	//if (isLeftCollision == true)
	if (leftObj != nullptr)
	{
		//mPositionX = leftPlatformX + 2.0f;
		mPositionX = leftObj->getPositionX() + 2.0f;
	}

	//if (isTopCollision == true)
	if (topObj != nullptr)
	{
		//mPositionY = topPlatformY - 2.0f;
		mPositionY = topObj->getPositionY() - 2.0f;
	}

	isBottomCollision = isDownCollision || (isDownLeftCollision || isDownRightCollision);

	//if (isBottomCollision == true)
	if (bottomObj != nullptr)
	{
		float bottomY = bottomObj->getPositionY() + 2.0f;

		if (mPositionY < bottomY)
		{
			mPositionY = bottomY;
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