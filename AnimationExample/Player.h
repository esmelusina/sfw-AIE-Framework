#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	virtual void update()
	{
		if (sfw::getKey('W'))
			y += sfw::getDeltaTime() * 15;
		// Check for movement
		// Update position, etc.
	}

	virtual void draw()
	{
		GameObject::draw();
		sfw::drawTexture(getTexture(textureName), x+width, y,
			width*2, height, angle);
	}
};