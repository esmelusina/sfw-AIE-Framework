#include <sfwdraw.h>
#include "AssetLibrary.h"
#include <vector>
#include "GameState.h"

void main()
{
	sfw::initContext(800, 600, "NSFW Draw");
	sfw::setBackgroundColor(BLACK);	


	// Now I'm using the loadTexture from AssetLibrary! Notice how I Get to name it now.
	// The name lets me easily refer to it later!
	loadTexture("Explosion", "../res/explosion.png", 5, 3); // so this is an explosion sprite sheet. TLDR WE WILL MAKE DINOSAURS EXPLODE!
	addAnimation("Explosion", "BOOM", {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14});
	addAnimation("Explosion", "NOTVERYBOOM", { 0,1 });

	//MenuState menu;

	// All of the logic for our game goes here now!
	GameState game;

	while (sfw::stepContext())
	{
		//switch ()
		//{
		//case MENU:
		//	menu.update();
		//	menu.draw();
		//case GAME:
			game.update(); // Do the thing!
			game.draw();   // Draw it!
		//}
	}

	sfw::termContext();
}