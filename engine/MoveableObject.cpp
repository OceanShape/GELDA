#include "MoveableObject.h"

#include <bitset>

#include "MessageQueue.h"

GLuint MoveableObject::getTexture(bool& isRightSide) {
  isRightSide = (mDirectionStatus == eDirectionStatus::RIGHT);
  return (*mTexture)[mCurrentFrame / 30];
}
void MoveableObject::setMode(const bool& isEditorMode) {
  if (isEditorMode == true) {
    mCurrentFrame = 0;
    mJumpStatus = eJumpStatus::NO_JUMP;
  } else {
    mJumpStatus = eJumpStatus::FALL;
  }
  mDirectionStatus = eDirectionStatus::RIGHT;
  mMoveStatus = eMoveStatus::STOP;
}
void MoveableObject::update(const std::vector<Object*>& objects) {
  updateFrame();

  // update gravity status
  if (mJumpStatus == eJumpStatus::JUMP) {
    isOnPlatform = false;
    mPosY += 0.6f;
    if (mPosY - jumpStartPositionY > 3.0f * 2.0f) {
      jumpDecelerationStartPositionY = mPosY;
      mJumpStatus = eJumpStatus::JUMP_DECELERATION;
    }
  } else if (mJumpStatus == eJumpStatus::FALL) {
    mPosY -= 0.6f;
  } else if (mJumpStatus == eJumpStatus::JUMP_DECELERATION) {
    jumpDecelerationSpendTime += 1.0f;

    float upSpeed = 0.8f + (gravityY * jumpDecelerationSpendTime);

    if (upSpeed < -0.6f) {
      mJumpStatus = eJumpStatus::FALL;
    }
    mPosY += upSpeed;
  }

  // update life status
  if (mPosY < -20.0f) {
    mLifeStatus = eLifeStatus::DEAD;
    return;
  }

  // check collision status
  // ¡Ø Notice: Only C++20 or later only
  auto reverseObjects =
      objects | std::views::filter([this](Object* g) {
        return mPosX - 2.0f < g->mPosX && g->mPosX < mPosX + 2.0f &&
               mPosY - 2.0f < g->mPosY && g->mPosY < mPosY + 2.0f;
      }) |
      std::views::reverse;

  isPreBottomCollision = isBottomCollision;

  bool isLeftCollision = false;
  bool isRightCollision = false;
  bool isBottomCollision = false;

  for (Object* target : reverseObjects) {
    if (this == target) continue;

    float posX = target->mPosX;
    float posY = target->mPosY;

    // bottom
    if (mPosY - 1.3f > posY) {
      if ((posX < mPosX + 1.0f && mPosX - 1.0f < posX) ||
          (mJumpStatus == eJumpStatus::FALL &&
           (isLeftCollision == false && isRightCollision == false) &&
           (mPosX + 1.9f > posX || mPosX - 1.9f < posX))) {
        isBottomCollision = true;
        MessageQueue::push(this, target, CollisionType::Down);
      }
    }
    // left
    else if (mPosY - 0.8f < posY && mPosX - 1.6f > posX) {
      isLeftCollision = true;
      MessageQueue::push(this, target, CollisionType::Left);
    }
    // right
    else if (mPosY - 0.8f < posY && mPosX + 1.6f < posX) {
      isRightCollision = true;
      MessageQueue::push(this, target, CollisionType::Right);
    }
    // top
    else if (mPosY + 1.2f < posY) {
      mJumpStatus = eJumpStatus::FALL;
      MessageQueue::push(this, target, CollisionType::Top);
    }
  }

  if (isBottomCollision == false && isPreBottomCollision == true &&
      mJumpStatus == eJumpStatus::NO_JUMP) {
    MessageQueue::push(this, nullptr, CollisionType::EscapeDown);
  }

  mPosY = (static_cast<int>(mPosY * 10.0f) / 10.0f);
}

void MoveableObject::updatePosition(Object* obj,
                                    const CollisionType& collisionType) {
  if (collisionType == CollisionType::Top) mPosY = obj->mPosY - 2.0f;
  if (collisionType == CollisionType::Left) mPosX = obj->mPosX + 2.0f;
  if (collisionType == CollisionType::Right) mPosX = obj->mPosX - 2.0f;
  if (collisionType == CollisionType::EscapeDown)
    mJumpStatus = eJumpStatus::FALL;
  if (collisionType == CollisionType::Down) {
    isBottomCollision = true;

    mPosY = obj->mPosY + 2.0f;

    jumpDecelerationSpendTime = 0.0f;
    jumpStartPositionY = mPosY;
    mJumpStatus = eJumpStatus::NO_JUMP;
    mMoveStatus = eMoveStatus::STOP;

    isOnPlatform = true;
  }
}

void MoveableObject::inputControl(const eInputStatus& input) {
  if (input == eInputStatus::LEFT) {
    mPosX += -0.3f;
    mMoveStatus = eMoveStatus::MOVE;
    mDirectionStatus = eDirectionStatus::LEFT;
  } else if (input == eInputStatus::RIGHT) {
    mPosX += 0.3f;
    mMoveStatus = eMoveStatus::MOVE;
    mDirectionStatus = eDirectionStatus::RIGHT;
  } else if (input == eInputStatus::ARROW_RELEASE) {
    mPosX += 0.0f;
    mMoveStatus = eMoveStatus::STOP;
  } else if (input == eInputStatus::JUMP_PRESS) {
    // if (mJumpStatus == eJumpStatus::NO_JUMP)
    if ((mJumpStatus == eJumpStatus::NO_JUMP ||
         mJumpStatus == eJumpStatus::FALL) &&
        isOnPlatform == true && (jumpStartPositionY - mPosY <= 0.05f)) {
      isOnPlatform = false;
      jumpStartPositionY = mPosY;
      mJumpStatus = eJumpStatus::JUMP;
    }
  } else if (input == eInputStatus::JUMP_RELEASE) {
    if (mJumpStatus == eJumpStatus::JUMP) {
      jumpDecelerationStartPositionY = mPosY;
      mJumpStatus = eJumpStatus::JUMP_DECELERATION;
    }
  }
}