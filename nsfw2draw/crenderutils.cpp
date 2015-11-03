#include <ogl\gl_core_4_4.h>
#include "crenderutils.h"
#include <cstring>




TextureData makeTextureSPRITE(unsigned w, unsigned h, unsigned depth, const unsigned char *pixels )
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	return { texture };
}
RenderObject makeVAO(const struct Vertex *verts, unsigned vsize, const unsigned * tris, unsigned tsize)
{
	unsigned vbo, ibo, vao, size = tsize;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size  * sizeof(unsigned), tris, GL_STATIC_DRAW);
																							
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)Vertex::NORMAL);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::TEXCOORD);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return { vbo, ibo, vao, size };
}
ShaderProgram makeShader(const char *vsource, const char *fsource)
{
	GLuint shader = glCreateProgram();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vsource, 0);
	glShaderSource(fShader, 1, &fsource, 0);

	glCompileShader(vShader);
	glCompileShader(fShader);

	glAttachShader(shader, vShader);
	glAttachShader(shader, fShader);

	glLinkProgram(shader);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return {shader};
}

void freeShaderProgram(ShaderProgram &sp)
{
	glDeleteProgram(sp.handle);
	memset(&sp, 0, sizeof(ShaderProgram));
}
void freeRenderObject(RenderObject &ro)
{
	glDeleteBuffers(1, &ro.vbo);
	glDeleteBuffers(1, &ro.ibo);
	glDeleteVertexArrays(1, &ro.vao);
	memset(&ro, 0, sizeof(RenderObject));
}
void freeTextureData(TextureData &td)
{
	glDeleteTextures(1, &td.handle);
	memset(&td, 0, sizeof(TextureData));
}
