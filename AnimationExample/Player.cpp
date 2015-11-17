#include "GameState.h"
#include "Player.h"

void Player::update()
{
	fireDelay -= sfw::getDeltaTime();
	// example of switching between animations
	if (sfw::getKey(' ') && fireDelay < 0)
	{
		fireDelay = rateOfFire;
		gs()->makeBullet(x, y, 0, 300, 4.f); // Now we can use this to draw stuff!
	}

	if (animTimer > getAnimationDuration(textureName, animationName))
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