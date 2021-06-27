#pragma once
#include <queue>

class MessageQueue {
public:
	static MessageQueue& instance() {
		static MessageQueue* instance = new MessageQueue();
		return *instance;
	}

private:
	MessageQueue() {}
};