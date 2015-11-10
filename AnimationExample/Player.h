#pragma once
#include "GameObject.h"

/*
	See GameObject.h for rationale!


	Inheriting from gameobject, we can adopt all of the properties it has and
	contribute some of our own. A player, for example, may want to update
	it's position based on input.
*/


class Player : public GameObject
{
public:
	float speed;
	Player() : speed(100) { width = 200; height = 200; }

	virtual void update()
	{
		GameObject::update();

		if (sfw::getKey(' '))
		{
			animTimer = 0;
			animationName = "NOTVERYBOOM";
		}

		if (animTimer > getAnimationDuration(textureName,animationName))
		{
			animTimer = 0;
			animationName = "BOOM";
		}

		float sdt = sfw::getDeltaTime() * speed;
		if (sfw::getKey('W')) y += sdt; // Euler integration
		if (sfw::getKey('S')) y -= sdt;
		if (sfw::getKey('A')) x -= sdt;
		if (sfw::getKey('D')) x += sdt;
	}
};