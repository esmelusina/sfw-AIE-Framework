#include <sfwdraw.h>
#include "AssetLibrary.h"
#include "Player.h"
#include <vector>
#include "collisiontests.h"

void main()
{
	sfw::initContext(800, 600, "NSFW Draw");
	sfw::setBackgroundColor(BLACK);	


	// Now I'm using the loadTexture from AssetLibrary! Notice how I Get to name it now.
	// The name lets me easily refer to it later!
	loadTexture("Explosion", "../res/explosion.png", 5, 3); // so this is an explosion sprite sheet. TLDR WE WILL MAKE DINOSAURS EXPLODE!
	addAnimation("Explosion", "BOOM", {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14});

	addAnimation("Explosion", "NOTVERYBOOM", { 0,1 });

	// Since I know the name, I can reuse it anywhere!
	Player dinosaur;
	GameObject barricade;
	barricade.x = 80;
	barricade.y = 80;
	barricade.textureName = "Explosion";
	barricade.animationName = "NOTVERYBOOM";

	while (sfw::stepContext())
	{
		barricade.draw();
		dinosaur.draw();	// Draw uses gameObject's draw, which just checks what the asset name is
							// and then fetches the appropriate handle for the draw.
		barricade.update();
		dinosaur.update(); 


		doCollision(barricade, dinosaur);
	}

	sfw::termContext();
}