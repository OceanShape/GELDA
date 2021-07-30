#include "MessageQueue.h"

static std::queue<CollisionMessage> messages;

void MessageQueue::push(MoveableObject* object, const int& targetIdx,
                        CollisionType type) {
  messages.push({object, targetIdx, type});
}

void MessageQueue::pop() { messages.pop(); }

const CollisionMessage MessageQueue::front() { return messages.front(); }

size_t MessageQueue::getSize() { return messages.size(); }