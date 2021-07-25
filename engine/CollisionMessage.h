#pragma once
#include "CollisionType.h"
#include "MoveableObject.h"

struct CollisionMessage {
  MoveableObject* object;
  Object* target;
  CollisionType type;
};