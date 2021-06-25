#pragma once
#include <vector>

#include "Object.h"

struct CollisionMessage
{
	const std::vector<Object*>& object;
	const std::vector<Object*>& target;
	const char& collisionType;
};