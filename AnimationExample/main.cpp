#include <sfwdraw.h>
#include "AssetLibrary.h"
#include "Player.h"

void main()
{
	sfw::initContext(800, 600, "NSFW Draw");
	sfw::setBackgroundColor(BLACK);	


	// Now I'm using the loadTexture from AssetLibrary! Notice how I Get to name it now.
	// The name lets me easily refer to it later!
	loadTexture("Dinosaur", "../res/explosion.png", 5, 3); // so this is an explosion sprite sheet. TLDR WE WILL MAKE DINOSAURS EXPLODE!

	// Since I know the name, I can reuse it anywhere!
	Player dinosaur;
	dinosaur.textureName = "Dinosaur";

	while (sfw::stepContext())
	{
		dinosaur.draw();	// Draw uses gameObject's draw, which just checks what the asset name is
							// and then fetches the appropriate handle for the draw.

		dinosaur.update(); 
	}

	sfw::termContext();
}