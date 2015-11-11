#pragma once
#include <string>
#include <sfwdraw.h>
#include "AssetLibrary.h"

/*
To build a game, we need to create a data model that changes over time and input.
The model will typically be broken down into various agents, entities, or gameObjects.
Each may have different properties and types, but make up a smaller part of the whole.

To help ease production efforts and to create consistent behavior- we can consider what
qualities ALL objects in our game share. To achieve this, we can setup an inheritance hierarchy
with a powerful base class to represent the core principles of our game.

These would probably be limited to data necessary for rendering and collision detection-
possibly animation.


See Player.h for an example of inheritance!

*/

class GameObject
{
public:
	
	// Just the vars necessary for drawing
	std::string textureName; // see AssetLibrary
	float x, y, width, height, angle;		
	
	// Vars necessary for animation
	std::string animationName;		// Information for animation
	float animTimer;	
	unsigned currentFrame;

	GameObject() :x(400), y(300), width(60),height(60), angle(0),
					currentFrame(0), animTimer(0) {}
	
	// Draw function is the same
	virtual void draw() 
	{
		sfw::drawTexture(getTexture(textureName), x, y, width, height, angle, true, currentFrame);
	}

	// We could use the base class for updating the animation.
	// This REQUIRES that we use animations always when drawing, even if it's only one frame!
	virtual void update()
	{
		animTimer	+= sfw::getDeltaTime();
		currentFrame = sampleAnimation(textureName, animationName, animTimer);
	}

	virtual void onCollision(GameObject &go, float distance) { }
};

float doCollision(GameObject &go1, GameObject &go2);