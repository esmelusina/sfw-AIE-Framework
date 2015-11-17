#pragma once
#include "GameObject.h"

/*
	See GameObject.h for rationale!


	Inheriting from gameobject, we can adopt all of the properties it has and
	contribute some of our own. A player, for example, may want to update
	it's position based on input.
*/

class GameState;

class Player : public GameObject
{
public:
	float speed;
	float fireDelay;
	float rateOfFire;

	Player() : speed(100), rateOfFire(0.1f), fireDelay(0.f)
	{
		width = 200; height = 200;
		animationName = "NOTVERYBOOM";
		textureName	  = "Explosion";
	}

	virtual void onCollision(GameObject &go, float distance)
	{
		animTimer = 0;
		animationName = "NOTVERYBOOM";
	}

	virtual void update(); // Moved to the .cpp!
};