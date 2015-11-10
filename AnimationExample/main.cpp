#include <sfwdraw.h>
#include "AssetLibrary.h"
#include "GameObject.h"

void main()
{
	sfw::initContext(800, 600, "NSFW Draw");
	sfw::setBackgroundColor(BLACK);	
	//unsigned explosion = sfw::loadTextureMap();	

	loadTexture("Dinosaur", "../res/explosion.png", 5, 3);

	GameObject dinosaur;
	dinosaur.textureName = "Dinosaur";

	while (sfw::stepContext())
	{
		dinosaur.draw();
		dinosaur.update();
	}

	sfw::termContext();
}