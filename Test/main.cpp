
#include <sfwdraw.h>




void main()
{
	sfw::initContext(800,600,"NSFW Draw");

	unsigned d = sfw::loadTextureMap("../res/fontmap.png",16,16);
	unsigned r = sfw::loadTextureMap("../res/background.jpg");
	unsigned u = sfw::loadTextureMap("../res/crosshair.png");

	float acc = 0;
	char c = '\0';

	sfw::setBackgroundColor(YELLOW);

	while (sfw::stepContext())
	{	
		sfw::drawTexture(r, 0, 600, 800, 600, 0, false,0, 0x88888888);

		acc += sfw::getDeltaTime();
		sfw::drawString(d, "TIME 4 FUN", 400, 300, 48, 48, -acc*24,'\0',MAGENTA);

		sfw::drawString(d, "6", 400, 32, 24, 24);
		sfw::drawString(d, "12", 400, 600 - 32, 24, 24);
		sfw::drawString(d, "3", 800-32, 300, 24, 24);
		sfw::drawString(d, "9", 32, 300, 24, 24);

		if(sfw::getMouseButton(MOUSE_BUTTON_RIGHT))
			sfw::drawTexture(u, sfw::getMouseX(), sfw::getMouseY(), sfw::getTextureWidth(u)/2.5, sfw::getTextureHeight(u)/2.5,45,true,0,0x88ffffff);
		else sfw::drawTexture(u, sfw::getMouseX(), sfw::getMouseY(), sfw::getTextureWidth(u)/2, sfw::getTextureHeight(u)/2);
	}

	sfw::termContext();
}