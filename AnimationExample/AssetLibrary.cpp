#include "AssetLibrary.h"
#include "sfwdraw.h"
#include <map>
#include <iostream>
/*
	static variable is one whose memory address is constant and
	determined at the time in which a program boots up

	Associative Container:
	*/
static std::map<std::string, unsigned> textures;

void loadTexture(const std::string &name, const char *path,
											unsigned r, unsigned c)
{
	textures[name] = sfw::loadTextureMap(path,r,c);
}

unsigned getTexture(const std::string &name)
{
	if (name == "")
	{
		std::cerr << "Texture name not set!" << std::endl;
		return 0;
	}
	return textures[name];
}