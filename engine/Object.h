#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>  // for view
#include <cassert>
#include <cfloat>
#include <iostream>
#include <ranges>  // reverse range(¡Ø Notice: Only C++20 or later only)
#include <vector>

#include "CollisionType.h"
#include "ObjectType.h"
#include "Status.h"

class Object {
 protected:
  std::vector<GLuint>* mTexture;
  size_t mCurrentFrame = 0;
  float mTextureRate = 0.0625f;

  ObjectType mObjectType;

  float mPosX;
  float mPosY;

  eLifeStatus mLifeStatus = eLifeStatus::ALIVE;

 public:
  Object(std::vector<GLuint>* texture, const ObjectType& type, const float& posX,
         const float& posY)
      : mTexture(texture) {
    mObjectType = type;
    mPosX = posX;
    mPosY = posY;
  }

  friend class MoveableObject;

  ~Object() {}

  void input(const bool& isEditorMode, const eInputStatus& inputStatus) {
    if (isEditorMode == true)
      inputEditor(inputStatus);
    else
      inputControl(inputStatus);
  }
  void inputEditor(const eInputStatus& input) {
    if (input == eInputStatus::LEFT)
      mPosX += -0.5f;
    else if (input == eInputStatus::RIGHT)
      mPosX += 0.5f;
    else if (input == eInputStatus::UP)
      mPosY += 0.5f;
    else if (input == eInputStatus::DOWN)
      mPosY += -0.5f;
  }
  virtual void inputControl(const eInputStatus& input) {}

  void draw(const int& textureNumber);
  virtual GLuint getTexture(bool& isRightSide) { return (*mTexture)[0]; }
  virtual void setMode(const bool& isEditorMode) { mCurrentFrame = 0; }
  virtual void update(const std::vector<Object*>& object) { updateFrame(); }
  void updateFrame() {
    if (++mCurrentFrame == 60) mCurrentFrame = 0;
  }
};