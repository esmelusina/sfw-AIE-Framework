


#include <sfwdraw.h>
#include <iostream>


void main()
{
	


	sfw::initContext(800,600,"NSFW Draw");

	
	// load Texture Map
	unsigned f = sfw::loadTextureMap("../res/tonc_font.png", 6, 16);	// test non-square atlas
	unsigned d = sfw::loadTextureMap("../res/fontmap.png",16,16);
	unsigned r = sfw::loadTextureMap("../res/background.jpg");
	unsigned u = sfw::loadTextureMap("../res/crosshair.png");

	float acc = 0;
	float scroll = 0;
	char c = '\0';

	float animTimer = 0;
	int index = 0;
	int animArray[2] = {1,'a'};

	sfw::setBackgroundColor(BLACK);


	float x = 400, y = 300, speed = 200.0f;
	float rot = 0;

	

	while (sfw::stepContext())
	{	
       

        float mat1[9] = {200,0,0,
                         0,200,0,
                         480,320,1};
        float mat2[9] = { 200,0,0,
                          0,200,0,
                          400,300,1 };

        
        sfw::drawTextureMatrix3(r, 0, 0x88888888, mat1, 10);
        sfw::drawTextureMatrix3(r, 0, 0x88888888, mat2, -1);
        sfw::drawLine3(0, 0, 800, 600, 9, GREEN, sfw::identity3);
        


		//x += sfw::getGamepadAxis(0, 0) * speed * sfw::getDeltaTime();
		//y -= sfw::getGamepadAxis(0, 1) * speed * sfw::getDeltaTime();

		//rot += sfw::getGamepadAxis(0, XBOX360_AXIS_TRIGGERS) * speed * sfw::getDeltaTime();

		//if (sfw::getGamepadButton(0, XBOX360_BUTTON_X))
		//{
		//	sfw::drawString(d, "BLAM!", x, y, 48, 48, rot);
		//}

		//0,1 left stick
		// 2 triggers
		//4,3 right stick
		// A,B,X,Y,L,R, Back, Start, LStick, RStick, DUP, DRIGHT, DDOWN, DLEFT


		//animTimer += sfw::getDeltaTime();
		//if (animTimer > 1/8.f)
		//{
		//	animTimer = 0;
		//	index++;
		//	if (index >= 2) index = 0;
		//}
		//sfw::drawTexture(d, 400, 300, 400, 300, 0, true, animArray[index]);



		//// test non-square atlas and drawstring
		//sfw::drawString(f, " !\"#$%&'()*+,./-\n0123456789:;<=>?\n@ABCDEFGHIJKLMNO\nPQRSTUVWXYZ[\\]^_\n`abcdefghijklmno\npqrstuvwxyz{|}~", 0, 600, 48, 48, 0, ' ');
		//
		////transparency'
		//if (scroll <= 0) scroll = 600;
		//else scroll -= sfw::getDeltaTime() * 100;


		////rotation/scale
		//acc += sfw::getDeltaTime();
		//sfw::drawString(d, "TIME 4 FUN", 400, 300, 48, 48, -acc*24,'\0',MAGENTA);

		//
		////spatial positions
		//sfw::drawString(d, "6", 400, 32, 24, 24);
		//sfw::drawString(d, "12", 400, 600 - 32, 24, 24);
		//sfw::drawString(d, "3", 800-32, 300, 24, 24);
		//sfw::drawString(d, "9", 32, 300, 24, 24);

		//// mouse input and getTextureWidth/Height
		//if(sfw::getMouseButton(MOUSE_BUTTON_RIGHT))
		//	sfw::drawTexture(u, sfw::getMouseX(), sfw::getMouseY(), sfw::getTextureWidth(u)/2.5, sfw::getTextureHeight(u)/2.5,45,true,0,0x88ffffff);
		//else sfw::drawTexture(u, sfw::getMouseX(), sfw::getMouseY(), sfw::getTextureWidth(u)/2, sfw::getTextureHeight(u)/2);
	}

	sfw::termContext();
}