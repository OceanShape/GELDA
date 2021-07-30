#pragma once
#include "CollisionType.h"
#include "MoveableObject.h"

struct CollisionMessage {
  MoveableObject* object;
  const int targetIdx;
  const CollisionType type;
};