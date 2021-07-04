#pragma once
#include <queue>
#include <vector>

#include "CollisionMessage.h"
#include "CollisionType.h"
#include "Object.h"

static std::queue<CollisionMessage> messages;

class MessageQueue {
 public:
  static MessageQueue& instance() {
    static MessageQueue* instance = new MessageQueue();
    return *instance;
  }

  static void AddMessage(Object* object, Object* target, CollisionType type) {
    messages.push({object, target, type});
  }

  static int getSize() { return messages.size(); }

 private:
  MessageQueue() {}
};