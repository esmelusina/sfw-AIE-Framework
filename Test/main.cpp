
#include <sfwdraw.h>



void main()
{
	sfw::initContext();

	unsigned d = sfw::loadTextureMap("../res/fontmap.png");

	while (sfw::stepContext())
	{
		sfw::drawTexture(d);
	}

	sfw::termContext();
}