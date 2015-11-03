#pragma once

/***************************************

For use in AIE Year 1 Programming Classes.

This namespace is SUITED FOR WORK.

This framework is intended to provide a simple, modern implementation of OpenGL for educational use.
It can, primarily, load textures and sprite atlases (when given rows/cols). It does NOT use depth testing (no Z-ordering),
but does allow for transparency. This means that the order in which draw calls are made matters.
The default projection uses pixel-perfect screen coordinates, with 0,0 at the bottom left.

Where applicable, Colors use HEX values in RGBA format (0xRRGGBBAA)-> like html.
Where applicable, transformation matrix parameters are treated as parent matrices (pre-multiplied by other parameters).
Where applicable, 'centered' parameters indicate whether the image and transforms will occur from the center, or TOP-LEFT.



Contact me for more details/examples/explanations.
Author: Esmeralda Salamone, esmes@aie.edu.au

Uses GLFW (release for all configs), GLM, STB, and opengl loader generator.
For to whomever cares, this only link

NOTE: YOU MUST CALL initContext for any of the functions to work!

************************/

// Uses GLFW keycodes, research for additional options.
#define KEY_ESCAPE    256
#define KEY_ENTER	  257
#define KEY_TAB		  258
#define KEY_BACKSPACE 259

#define KEY_RIGHT     262
#define KEY_LEFT	  263
#define KEY_DOWN	  264
#define KEY_UP		  265

#define MOUSE_BUTTON_RIGHT	1
#define MOUSE_BUTTON_LEFT	0

// every 2 hexits corresponds to an 8-bit color component value.
// look at the following values to intuit what that means.
// In RGBA color format
#define WHITE	 0xffffffff
#define BLACK    0x000000ff
#define CYAN	 0x00ffffff
#define MAGENTA  0xff00ffff
#define YELLOW	 0xffff00ff
#define RED		 0xff0000ff
#define GREEN    0x00ff00ff
#define BLUE	 0x0000ffff
#define NONE     0x00000000

#define PI 3.14159265359

namespace sfw
{
	//Identity transformation matrix, primarily for internal use.
	const float identity[16] = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};
	// path	     : Path to the texture file
	// rows,cols : If this is a uniform spritesheet (also called sprite atlas), how many rows and columns of sub-images are there?
	// Returns a handle to be used with drawTexture/drawString/drawTextureMatrix functions. These handles are NOT openGL handles, and
	// are only intended to be used with those functions.
	unsigned loadTextureMap(const char* path, unsigned rows = 1, unsigned cols = 1);
	// Using the texture handle provided by loadTextureMap, returns the width in pixels.
	unsigned getTextureWidth(unsigned handle);
	// Using the texture handle provided by loadTextureMap, returns the height in pixels.
	unsigned getTextureHeight(unsigned handle);
	
	// verbose wrapper for drawTextureMatrix
	// Handle		  : Texture handle returned from loadTextureMap
	// x,y			  : translation (or position)
	// width,height	  : scale- use with getTextureWidth and getTextureHeight to draw a texture true to size.
	// angle		  : Rotation, in degrees.
	// centered		  : if true, scale/rotation/translation will occur relative to the top-left corner of the image.
	// index		  : If loadTextureMap was given rows and cols, index will fetch the corresponding sub-image
	// tint			  : a hex color value applied as a tint to the texture
	// transform	  : a 4x4 transformation matrix that could  be used for parenting.
	void drawTexture(unsigned handle, float x, float y, float width = 1, float height = 1, float angle = 0, bool centered = true, unsigned index = 0, unsigned int tint = WHITE, const float transform[16] = identity);
	
	// used internally for all texture draw calls.
	// handle		  : Texture handle returned from loadTextureMap
	// index		  : If loadTextureMap was given rows and cols, index will fetch the corresponding sub-image
	// tint			  : a hex color value applied as a tint to the texture
	// transform	  : a 4x4 transformation matrix
	void drawTextureMatrix(unsigned handle, unsigned index = 0, unsigned int tint = WHITE, const float transform[16] = identity);
	
	// verbose wrapper for drawTextureMatrix, focusing on drawing strings of text.
	// search "bitmap font" for some examples.
	// Uses 'centered = false' behavior- transformations applied to top-left corner of all the text.
	// handle		  : Texture handle returned from loadTextureMap
	// text			  : String of text to draw
	// x,y			  : translation (or position)
	// width,height	  : scale- applied to each individual character, not the entire text.
	// angle		  : Rotation, in degrees.
	// offset		  : What character does our bitmap font start with? ' ' is common.
	// tint			  : a hex color value applied as a tint to the texture
	// transform	  : a 4x4 transformation matrix, applied as a parent transform
	void drawString(unsigned handle, const char *text, float x, float y, float width = 1, float height = 1, float angle = 0, char offset = '\0', unsigned int tint = WHITE, const float transform[16] = identity);

	// draws just the diagonal line of a quad, using the transformation matrix to buff it out
	// tint			  : hex color value
	// transform	  : a 4x4 transformation matrix
	void drawLineMatrix(unsigned tint = WHITE, const float transform[16] = identity);

	// verbose wrapper for drawLineMatrix
	// x1,y1,x2,y2	  : first and second position of the line.
	// tint			  : hex color value
	// transform	  : a 4x4 transformation matrix, applied as parent transform
	void drawLine(float x1, float y1, float x2, float y2, unsigned tint = 0xffffffff, const float transform[16] = identity);

	// uses drawLine to generate a circle
	// x,y		: translation (position)
	// radius	: scale
	// steps	: how smooth is the surface? Larger radii should use larger step sizes
	// tint			  : hex color value
	// transform	  : a 4x4 transformation matrix, applied as parent transform
	void drawCircle(float x, float y, float radius, unsigned steps = 12, unsigned tint = 0xffffffff, const float transform[16] = identity);

	// used to initialize an openGL rendering context. USE THIS BEFORE ANYTHING ELSE!
	// width, height	: Dimensions of window.
	// title			: OS name of the window
	// initShaders		: Set to false if this is the second time calling initContext for the lifetime of the program.
	bool initContext(unsigned width = 800, unsigned height = 600, const char *title = "SFW Draw");
	// closes the rendering context AND DELETES ALL TEXTURES!
	void termContext();
	// Update the rendering context- use as driver for the game loop. Returns false if the window should close.
	bool stepContext();

	// set the clear color using a hexit color
	void setBackgroundColor(unsigned int tint);

	// for the brave, change the projection matrix used by the drawing functions
	// orthographic  projection is used by default
	void setProjectionMatrix(const float transform[16]);

	// for the brave, change the view matrix used by the drawing functions
	// could be used for camera logic.
	void setViewMatrix(const float transform[16]);

	// input using keycodes (see GLFW reference, most characters will work as per normal)
	bool getKey(unsigned keycode);

	// 0 for right, 1 for left
	bool getMouseButton(unsigned mousekeycode);
	
	// returns the x position of the mouse- Screen relative!
	float getMouseX();
	// returns the y position of the mouse- Screen relative!
	float getMouseY();

	// Get the total time passed since the application started!
	float getTime();

	// get the time passsed between the last two frames.
	// This value will NOT change until stepContext is called.
	// use getTime to implement your own timing system.
	float getDeltaTime();
}
