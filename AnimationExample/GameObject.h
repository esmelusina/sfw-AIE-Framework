#pragma once
#include <string>
#include <sfwdraw.h>
#include "AssetLibrary.h"

class GameObject
{
public:
	float x, y, width, height, angle;
	std::string textureName;

	GameObject() :x(400),y(300),
				width(60),height(60),
					angle(0), textureName("") {}
	
	virtual void draw()
	{
		sfw::drawTexture(getTexture(textureName),x,y,
							width,height,angle);
	}
	virtual void update() { }
};