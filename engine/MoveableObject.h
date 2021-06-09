#pragma once
#include "Object.h"
class MoveableObject :
	public Object
{
private:
	float gravityAccelerationY = -0.10f;
	float jumpStartPositionY = 0.0f;
	float jumpDecelerationStartPositionY = 0.0f;
	float jumpDecelerationSpendTime = 0.0f;

	bool isBottomCollision = false;
	bool isJumpDeceleration = false;

	eDirectionStatus mDirectionStatus = eDirectionStatus::RIGHT;
	eMoveStatus mMoveStatus = eMoveStatus::STOP;
	eJumpStatus mJumpStatus = eJumpStatus::FALL;
public:
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
	void update(const bool& isEditorMode, const std::vector<Object*>& objects)
	{
		if (isEditorMode == false)
		{
			// update gravity status
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
				if (mPositionY - 1.0f > posY)
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
				// right collision
				else if (mPositionX + 1.8f < posX)
				{
					isRightCollision = true;
					rightPlatformX = posX;
				}
				// left collision
				else if (mPositionX - 1.8f > posX)
				{
					isLeftCollision = true;
					leftPlatformX = posX;
				}
				// top collision
				else if (mPositionY + 1.8f < posY)
				{
					isTopCollision = true;
					lowestTopPlatformY = posY;
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

		mPositionY = (static_cast<int>(mPositionY * 10.0f) / 10.0f);
	}

	void inputControl(const eInputStatus& input)
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
};