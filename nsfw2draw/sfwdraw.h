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
	// Returns a handle to be used by drawTextureMap
	unsigned loadTextureMap(const char* path, unsigned rows = 1, unsigned cols = 1);

	void drawTexture(unsigned handle);

	
	// handle	 : to the texture map that, we're hoping, is a font
	// x,y		 : screen coords
	// scale	 : size
	// angle	 : of rotation in degrees
	// center    : draw from center or top-right?
	// idx		 : if this is texture has rows and cols, which image do we draw?
	// rgba		 : tint, every two hexits cooresponds to RGBA : 0xRRGGBBAA
	void drawTextureMap(unsigned handle, float x, float y, float scale = 1, float angle = 0, bool center = true, unsigned idx = 0, unsigned rgba = 0xffffffff, const float transform[16] = identity);

	// text		 : String to draw
	// handle	 : to the texture map that, we're hoping, is a font
	// x,y		 : screen coords
	// scale	 : size
	// angle	 : of rotation in degrees
	// charStart : What character does this map start with? Assume all other characters are ascii ascending!
	// rgba		 : tint, every two hexits cooresponds to RGBA : 0xRRGGBBAA
	void drawString(const char *text, unsigned handle, float x, float y, float scale = 1, float angle = 0, char charStart = ' ', unsigned rgba = 0xffffffff, const float transform[16] = identity);

	// x1,y1	 : First point
	// x2,y2	 : Second point
	// rgba		 : tint, every two hexits cooresponds to RGBA : 0xRRGGBBAA
	// transform : model transformation matrix. Applied BEFORE the rest of the parameters are considered.
	void drawLine(float x1, float y1, float x2, float y2, unsigned rgba = 0xffffffff, const float transform[16] = identity);


	void drawCircle(float x, float y, float radius, unsigned steps = 12, unsigned rgba = 0xffffffff, const float transform[16] = identity);


	// initialize a windowing context, width/height in pixels
	// Return whether the window successfully initialized or not (it isn't necessary to TERM if it fails).
	bool initContext(unsigned width = 800, unsigned height = 600, const char *title = "SFW Draw");

	// shut down and clean up openGL resources
	void termContext();

	// update the rendering context (poll for input, update deltaTime)
	bool stepContext();

	//KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT now defined
	bool getKey(unsigned keycode);
	
	// 0 for left, 1 for right
	bool getMouseButton(unsigned mousekeycode);

	// get mouse's x position (screen)
	float getMouseX();
	// get mouse's y position (screen)
	float getMouseY();

	// Get time since application starts.
	// This is a realtime value (if you want to do your own timing).
	float getTime();

	// provides the time since stepContext() was last called.
	// Multiple calls will be the same until stepContext is called!
	float getDeltaTime();

}