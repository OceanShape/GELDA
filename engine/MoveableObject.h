#pragma once
#include "Object.h"

class MoveableObject : public Object {
 private:
  float gravityY = -0.10f;
  float jumpStartPositionY = 0.0f;
  float jumpDecelerationStartPositionY = 0.0f;
  float jumpDecelerationSpendTime = 0.0f;

  bool isPreBottomCollision = false;
  bool isBottomCollision = false;
  bool isJumpDeceleration = false;
  bool isOnPlatform = false;

  eDirectionStatus mDirectionStatus = eDirectionStatus::RIGHT;
  eMoveStatus mMoveStatus = eMoveStatus::STOP;
  eJumpStatus mJumpStatus = eJumpStatus::FALL;

 public:
  MoveableObject(std::vector<GLuint>* texture, const float& posX,
                 const float& posY)
      : Object(texture, posX, posY) {}

  GLuint getTexture(bool& isRightSide);
  void setMode(const bool& isEditorMode) override;
  void update(const std::vector<Object*>& objects) override;
  void inputControl(const eInputStatus& input) override;
  void updatePosition(Object* obj, const CollisionType& collisiontype);
};