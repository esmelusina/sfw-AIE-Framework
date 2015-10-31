#pragma once
#include <glm\glm.hpp>

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
	{ glm::vec4(-1, 1, 0, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0,0) },
	{ glm::vec4(1, 1, 0, 1),  glm::vec4(0, 0, -1, 0), glm::vec2(1,0) },
	{ glm::vec4(1,-1, 0, 1),  glm::vec4(0, 0, -1, 0), glm::vec2(1,1) },
	{ glm::vec4(-1,-1, 0, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0,1) },
};

const unsigned QuadTris[] = { 3,1,0, 3,2,1 };


struct TextureData		{ unsigned handle;  };
struct RenderObject		{ unsigned vbo, ibo, vao, size; };
struct ShaderProgram	{ unsigned handle; };

TextureData		makeTextureSPRITE(unsigned w, unsigned h, unsigned depth, const unsigned char *pixels = nullptr);
RenderObject	makeVAO(const struct Vertex *verts, unsigned vsize, const unsigned * tris, unsigned tsize);
ShaderProgram	makeShader(const char *vsource, const char *fsource);

void freeShaderProgram(const ShaderProgram &sp);
void freeRenderObject(const RenderObject &sp);
void freeTextureData(const TextureData &sp);