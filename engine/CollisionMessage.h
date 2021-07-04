#pragma once
#include "CollisionType.h"
#include "Object.h"

struct CollisionMessage {
  Object* object;
  Object* target;
  CollisionType type;
};