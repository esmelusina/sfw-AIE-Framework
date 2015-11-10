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


version 1: We're just going to make sure everything has, at the very least, a way to draw.

*version 2: Will/May provide a basis for animation.

*version 3: Will unite how we handle collision detection.

See Player.h for an example of inheritance!
*/

class GameObject
{
public:
	float x, y, width, height, angle;			// Just the vars necessary for drawing
	
	std::string textureName, animationName;		// Information for animation
	float animTimer;	
	unsigned currentFrame;

	GameObject() :x(400),y(300),
				width(60),height(60),
					angle(0),
					currentFrame(0), animTimer(0) {}
	
	// See the main.cpp for an example of how concise the drawing code looks now.
	virtual void draw() 
	{
		sfw::drawTexture(getTexture(textureName),x,y,
							width,height,angle,true,currentFrame);
	}

	virtual void update()
	{
		animTimer += sfw::getDeltaTime();
		currentFrame = sampleAnimation(textureName, animationName, animTimer);
	} 
};