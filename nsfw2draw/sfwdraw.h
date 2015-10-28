#pragma once

#define KEY_UP		262
#define KEY_DOWN	263
#define KEY_LEFT	264
#define KEY_RIGHT	265

#define MOUSE_BUTTON_RIGHT	0
#define MOUSE_BUTTON_LEFT	1

#define PI 3.14159265359

namespace sfw
{
	const float identity[16] = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};
	// Push a texture from a file into the video card- rows and columns let us use monospace/uniform sprite maps, like bitmap fonts
	// Returns a handle to be used by drawTextureMap and freeTextureMap
	unsigned loadTextureMap(const char* path, unsigned rows = 1, unsigned cols = 1);
	
	
	void drawTextureMap(unsigned handle, float x, float y, float scale = 1, float angle = 0, unsigned rgba = 0xffffffff, bool center = true, unsigned idx = 0, const float transform[16] = identity);

	void drawString(const char *cstring,unsigned handle, float x, float y, float scale = 1, float angle = 0, unsigned rgba = 0xffffffff, bool center = true, unsigned offset = 32, const float transform[16] = identity);

	void drawLine(float x1, float y1, float x2, float y2, unsigned rgba = 0xffffffff, const float transform[16] = identity);

	void drawCircle(float x, float y, float radius, unsigned rgba = 0xffffffff, unsigned steps = 12, const float transform[16] = identity);


	// initialize a windowing context, width/height in pixels
	bool initContext(unsigned width, unsigned height, const char *title = "SFW Draw");
	void termContext();
	bool stepContext();

	bool getKey(unsigned keycode);
	bool getMouseButton(unsigned mousekeycode);

	float getMouseX();
	float getMouseY();


	float getTime();
	float getDeltaTime();

}