#pragma once
#include "Player.h"
#include "Bullet.h"
#include <vector>

/*
	GameState is the 'database' of the game.

	It's a central hub to allow all of the objects
	in the game communicate with one another.

	It's also a good place to manage spawning new objects!
*/


class GameState
{
	Player player; 	// store all of our gameobjects
	std::vector<Bullet> bullets;

public:
				   // Parameters may match the constructor for the bullet
	void makeBullet(float x, float y, float dx, float dy, float lifespan)
	{
		for (int i = 0; i < bullets.size(); ++i)
		{
			if (!bullets[i].isActive) // Find an empty spot in our vector
			{
				bullets[i] = Bullet(x, y, dx, dy, lifespan);
				return;
			}
		}
		// if there is no empty spot, generate a new bullet into the vector
		bullets.push_back(Bullet(x,y,dx,dy,lifespan));
	}
	
	// Mimic the makeBullet pattern- 
	void makeExplosion();
	void makeEnemy();


	// MUY IMPORTANTE!
	GameState()
	{
		// Setup the gamestate pointer that all of the gameobjects share!
		GameObject::gs() = this;
	}
	// Update objects, check for collision,
	// respond to events or special logic that changes how the game plays (ie. player is dead)
	void update()
	{
		if(player.isActive)
			player.update();

		// example useful for determining when to spawn a new wave of enemies.
		int nBulletsActive = 0;

		for (int i = 0; i < bullets.size(); ++i)
			if(bullets[i].isActive)
			{
				bullets[i].update();
				if(player.isActive)
					doCollision(player, bullets[i]);
			}
			else nBulletsActive++;

			if (nBulletsActive == 0)
			{
				// Spawn a new wave of enemies (obviously not using Bullets >__>)
			}

		// Collision detection between two objects of the same type
		for (int i = 0; i + 1 < bullets.size(); ++i)
			for (int j = i + 1; j < bullets.size(); ++j)
		{
			doCollision(bullets[i], bullets[j]);
		}
	}

	// DRaw everything now!
	void draw()
	{
		if(player.isActive) player.draw();

		for (int i = 0; i < bullets.size(); ++i)
			if(bullets[i].isActive)
			bullets[i].draw();
	}

	// Needs some way to 'spawn/destroy' bullets/enemies.
	
	// Changes in gameplay happen here.
};