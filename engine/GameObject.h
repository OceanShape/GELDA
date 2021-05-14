#pragma once
#include "Resource.h"

class GameObject
{
private:
	float mPosX = 0.0f; // position x
	float mPosY = 0.0f; // position y
	Resource* mResource;

public:
	GameObject()
	{
		std::string name[] = { "./sample_sprite.png" };
		mResource = new Resource(name, 1);
	}

	~GameObject()
	{
		delete mResource;
	}

	void draw();

};