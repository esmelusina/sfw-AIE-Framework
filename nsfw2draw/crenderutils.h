#pragma once
#include <glm\glm.hpp>

/*
https://github.com/esmelusina/sfw-AIE-Framework

Some utilities for creating openGL objects.

makeVAO is for loading geometry, designed to work with the vertex struct provided.
makeTextureSPRITE uses filtering params useful for images.
makeShader just builds/links a shader from the source provided.

Objects encapsulate openGL data specifically- they're just there for forward
compatibility in case I want to provide additional data from the function calls.

Author		 : esmeralda salamone, esmes@aie.edu.au
dependencies : glfw, glm, stb, and openGL Loader Generator

For educational use. All rights reserved(?).
*/

struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 texCoord;

	enum OFFSETS
	{
		POSITION = 0,
		NORMAL = POSITION + sizeof(glm::vec4),
		TEXCOORD = NORMAL + sizeof(glm::vec4)
	};
};


const Vertex QuadVerts[] =
{
	{ glm::vec4(-0.5f, 0.5f, 0, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0,0) },
	{ glm::vec4(0.5f, 0.5f, 0, 1),  glm::vec4(0, 0, -1, 0), glm::vec2(1,0) },
	{ glm::vec4(0.5f,-0.5f, 0, 1),  glm::vec4(0, 0, -1, 0), glm::vec2(1,1) },
	{ glm::vec4(-0.5f,-0.5f, 0, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0,1) },
};

const unsigned QuadTris[] = { 3,1,0, 3,2,1 };

// Wrapped in structs for forward compatibility
struct TextureData		{ unsigned handle;  };
struct RenderObject		{ unsigned vbo, ibo, vao, size; };
struct ShaderProgram	{ unsigned handle; };


TextureData		makeTextureSPRITE(unsigned w, unsigned h, unsigned depth, const unsigned char *pixels = nullptr);
RenderObject	makeVAO(const struct Vertex *verts, unsigned vsize, const unsigned * tris, unsigned tsize);
ShaderProgram	makeShader(const char *vsource, const char *fsource);

void freeShaderProgram(const ShaderProgram &sp);
void freeRenderObject(const RenderObject &ro);
void freeTextureData(const TextureData &td);