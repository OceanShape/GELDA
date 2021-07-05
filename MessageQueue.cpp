#include "MessageQueue.h"

static std::queue<CollisionMessage> messages;

void MessageQueue::AddMessage(Object* object, Object* target,
                              CollisionType type) {
  messages.push({object, target, type});
}

int MessageQueue::getSize() { return messages.size(); }