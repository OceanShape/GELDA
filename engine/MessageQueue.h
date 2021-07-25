#pragma once
#include <queue>
#include <vector>

#include "CollisionMessage.h"
#include "CollisionType.h"
#include "Object.h"

class MessageQueue {
 public:
  static MessageQueue& instance() {
    static MessageQueue* instance = new MessageQueue();
    return *instance;
  }

  static void push(Object* object, Object* target, CollisionType type);

  static void pop();

  static const CollisionMessage front();

  static size_t getSize();

 private:
  MessageQueue() {}
};