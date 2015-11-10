#pragma once
#include <string>



void loadTexture(const std::string &name, const char *path,
											unsigned r, unsigned c);

unsigned getTexture(const std::string &name);