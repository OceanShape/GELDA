#pragma once
#include <queue>

#include "Object.h"

class MessageQueue {
public:
	static MessageQueue& instance()
	{
		static MessageQueue* instance = new MessageQueue();
		return *instance;
	}

private:
	MessageQueue() {}
};