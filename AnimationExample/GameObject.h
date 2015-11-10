#pragma once
#include <string>
#include <sfwdraw.h>
#include "AssetLibrary.h"

class GameObject
{
public:
	float x, y, width, height, radius, angle;
	unsigned currentFrame;
	std::string textureName;

	GameObject() :x(400),y(300),width(60),height(60),
					radius(30), angle(0),
					currentFrame(0), textureName("") {}
	
	virtual void draw()
	{
		sfw::drawTexture(getTexture(textureName),x,y,
							width,height,angle,
							true, currentFrame);
	}
	virtual void update() {}
};