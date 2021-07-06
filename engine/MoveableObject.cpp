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
  // update frame
  ++mCurrentFrame;
  if (mCurrentFrame == 60) {
    mCurrentFrame = 0;
  }

  // update gravity status
  if (mJumpStatus == eJumpStatus::JUMP) {
    isOnPlatform = false;
    mPositionY += 0.6f;
    if (mPositionY - jumpStartPositionY > 3.0f * 2.0f) {
      jumpDecelerationStartPositionY = mPositionY;
      mJumpStatus = eJumpStatus::JUMP_DECELERATION;
    }
  } else if (mJumpStatus == eJumpStatus::FALL) {
    mPositionY -= 0.6f;
  } else if (mJumpStatus == eJumpStatus::JUMP_DECELERATION) {
    jumpDecelerationSpendTime += 1.0f;

    float upSpeed = 0.8f + (gravityY * jumpDecelerationSpendTime);

    if (upSpeed < -0.6f) {
      mJumpStatus = eJumpStatus::FALL;
    }
    mPositionY += upSpeed;
  }

  // check collision status
  // ¡Ø Notice: Only C++20 or later only
  auto reverseObjects = objects | std::views::filter([this](Object* g) {
                          return mPositionX - 2.0f < g->getPositionX() &&
                                 g->getPositionX() < mPositionX + 2.0f &&
                                 mPositionY - 2.0f < g->getPositionY() &&
                                 g->getPositionY() < mPositionY + 2.0f;
                        }) |
                        std::views::reverse;

  isPreBottomCollision = isBottomCollision;

  Object* leftObj = nullptr;
  Object* rightObj = nullptr;
  Object* topObj = nullptr;
  Object* bottomObj = nullptr;

  for (Object* t : reverseObjects) {
    if (this == t) {
      continue;
    }

    float posX = t->getPositionX();
    float posY = t->getPositionY();

    // bottom collision
    if (mPositionY - 1.3f > posY) {
      if ((posX < mPositionX + 1.0f && mPositionX - 1.0f < posX) ||
          (mJumpStatus == eJumpStatus::FALL && leftObj == nullptr &&
           rightObj == nullptr &&
           (mPositionX + 1.9f > posX || mPositionX - 1.9f < posX))) {
        bottomObj = t;
        MessageQueue::AddMessage(this, t, CollisionType::Down);
      }
    }
    // left collision
    else if (mPositionY - 0.8f < posY && mPositionX - 1.6f > posX) {
      leftObj = t;
      MessageQueue::AddMessage(this, t, CollisionType::Left);
    }
    // right collision
    else if (mPositionY - 0.8f < posY && mPositionX + 1.6f < posX) {
      rightObj = t;
      MessageQueue::AddMessage(this, t, CollisionType::Right);
    }
    // top collision
    else if (mPositionY + 1.2f < posY) {
      topObj = t;
      mJumpStatus = eJumpStatus::FALL;
      MessageQueue::AddMessage(this, t, CollisionType::Top);
    }
  }

  // update collision
  if (topObj != nullptr) {
    mPositionY = topObj->getPositionY() - 2.0f;
  }

  if (leftObj != nullptr) {
    mPositionX = leftObj->getPositionX() + 2.0f;
  }

  if (rightObj != nullptr) {
    mPositionX = rightObj->getPositionX() - 2.0f;
  }

  if (bottomObj != nullptr) {
    isBottomCollision = true;

    mPositionY = bottomObj->getPositionY() + 2.0f;

    jumpDecelerationSpendTime = 0.0f;
    jumpStartPositionY = mPositionY;
    mJumpStatus = eJumpStatus::NO_JUMP;
    mMoveStatus = eMoveStatus::STOP;

    isOnPlatform = true;
  } else if (mJumpStatus == eJumpStatus::NO_JUMP &&
             isPreBottomCollision == true) {
    mJumpStatus = eJumpStatus::FALL;
  }

  std::cout << MessageQueue::getSize() << std::endl;

  mPositionY = (static_cast<int>(mPositionY * 10.0f) / 10.0f);
}

void MoveableObject::updateCollision(Object* obj,
                                     const CollisionType& collisiontype) {
  if (collisiontype == CollisionType::Top) {
    mPositionY = obj->getPositionY() - 2.0f;
  }

  if (collisiontype == CollisionType::Left) {
    mPositionX = obj->getPositionX() + 2.0f;
  }

  if (collisiontype == CollisionType::Right) {
    mPositionX = obj->getPositionX() - 2.0f;
  }

  if (collisiontype == CollisionType::Down) {
    isBottomCollision = true;

    mPositionY = obj->getPositionY() + 2.0f;

    jumpDecelerationSpendTime = 0.0f;
    jumpStartPositionY = mPositionY;
    mJumpStatus = eJumpStatus::NO_JUMP;
    mMoveStatus = eMoveStatus::STOP;

    isOnPlatform = true;
  } else if (mJumpStatus == eJumpStatus::NO_JUMP &&
             isPreBottomCollision == true) {
    mJumpStatus = eJumpStatus::FALL;
  }
}

void MoveableObject::inputControl(const eInputStatus& input) {
  if (input == eInputStatus::LEFT) {
    mPositionX += -0.3f;
    mMoveStatus = eMoveStatus::MOVE;
    mDirectionStatus = eDirectionStatus::LEFT;
  } else if (input == eInputStatus::RIGHT) {
    mPositionX += 0.3f;
    mMoveStatus = eMoveStatus::MOVE;
    mDirectionStatus = eDirectionStatus::RIGHT;
  } else if (input == eInputStatus::ARROW_RELEASE) {
    mPositionX += 0.0f;
    mMoveStatus = eMoveStatus::STOP;
  } else if (input == eInputStatus::JUMP_PRESS) {
    // if (mJumpStatus == eJumpStatus::NO_JUMP)
    if ((mJumpStatus == eJumpStatus::NO_JUMP ||
         mJumpStatus == eJumpStatus::FALL) &&
        isOnPlatform == true && (jumpStartPositionY - mPositionY <= 0.05f)) {
      isOnPlatform = false;
      jumpStartPositionY = mPositionY;
      mJumpStatus = eJumpStatus::JUMP;
    }
  } else if (input == eInputStatus::JUMP_RELEASE) {
    if (mJumpStatus == eJumpStatus::JUMP) {
      jumpDecelerationStartPositionY = mPositionY;
      mJumpStatus = eJumpStatus::JUMP_DECELERATION;
    }
  }
}