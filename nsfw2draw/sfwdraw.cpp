

#include <ogl\gl_core_4_4.h>
#include <glfw\glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "sfwdraw.h"
#include "crenderutils.h"


//#include <iostream>
//#include <glm\glm.hpp>
//#include <glm\ext.hpp>
//#include <vector>
//#include <cctype>



namespace sfw
{
	static GLFWwindow *window;
	static RenderObject quad;

	static ShaderProgram textureShader;
	static ShaderProgram lineShader;

	static float lastTime;
	static float deltaTime;


	struct Texture { const unsigned handle; const unsigned int r, c, w, h, f; };

	static std::unordered_map<unsigned,Texture> textures;


	unsigned loadTextureMap(const char * path, unsigned int rows, unsigned int cols)
	{
		int w, h, f;
		unsigned d = 0;
		auto p = stbi_load(path, &w, &h, &f, STBI_default);
		switch (f)
		{
		case 1: d = GL_RED;	break; case 2: d = GL_RG; break; case 3: d = GL_RGB; break; case 4: d = GL_RGBA; break;
		default:std::cerr << "File not found or incorrect format." << std::endl; return 0;
		}

		auto td = makeTextureSPRITE(w, h, d, p);

		stbi_image_free(p);

		Texture t = { td.handle, rows, cols, (unsigned)w, (unsigned)h, (unsigned)f };
		textures.insert(std::pair<unsigned,Texture>(textures.size(), t));
		return textures.size() - 1;
	}

	void drawTexture(unsigned handle)
	{
		glUseProgram(textureShader.handle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.at(handle).handle);
		glUniform1i(glGetUniformLocation(textureShader.handle, "Texture"), 0);

		glBindVertexArray(quad.vao);
		glDrawElements(GL_TRIANGLES, quad.size, GL_UNSIGNED_INT, 0);
		glUseProgram(0);
	}

	//void drawTextureMap(unsigned handle, float x, float y, float scale, float angle, unsigned rgba, bool center, unsigned idx, const float transform[16])
	//{
	//	glUseProgram(textureShader);
	//	glm::mat4 offset = glm::mat4(1);
	//	if (!center) offset = glm::translate(-(float)textures[handle].ew , -(float)textures[handle].eh, 0.f);

	//	glm::mat4 model = glm::mat4(transform) * glm::scale(scale, scale, scale)  * glm::rotate(angle, glm::vec3(0,1,0))  * glm::translate(x, y, 0.f) * offset;

	//	
	//	glUniformMatrix4fv(glGetUniformLocation(textureShader, "Model"),1,GL_FALSE,glm::value_ptr(model));
	//	glUniformMatrix4fv(glGetUniformLocation(textureShader, "View"), 1, GL_FALSE, glm::value_ptr(View));
	//	glUniformMatrix4fv(glGetUniformLocation(textureShader, "Projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	//	
	//	glUniform1i(glGetUniformLocation(textureShader, "Rows"), textures[handle].r);
	//	
	//	glUniform1i(glGetUniformLocation(textureShader, "Cols"), textures[handle].h);
	//	
	//	glUniform1i(glGetUniformLocation(textureShader, "Index"), idx);
	//	glUniform1i(glGetUniformLocation(textureShader, "Tint"), rgba);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, textures[handle].handle);
	//	glUniform1i(glGetUniformLocation(textureShader, "TextureMap"), 0);
	//}

	//void drawString(const char *cstring, unsigned handle, float a_x, float a_y, float scale, float angle, unsigned rgba, bool center, unsigned offset, const float transform[16])
	//{
	//	float x, y;
	//	for (int i = 0; i < strlen(cstring); ++i)
	//	{
	//		if (cstring[i] == '\n' || cstring[i] == '\r')
	//		{
	//			x = a_x;
	//			y += textures[handle].eh;
	//		}
	//		else if (cstring[i] == '\t')
	//		{
	//			x += textures[handle].ew * 4;
	//		}
	//		else if(cstring[i] < 126 && cstring[i] > 32)
	//		{
	//			drawTextureMap(handle, x, y, angle, scale, rgba, center, cstring[i] - offset, transform);
	//			x += textures[handle].ew;
	//		}
	//	}
	//}

	//void drawLine(float x1, float y1, float x2, float y2, unsigned rgba, const float transform[16])
	//{
	//	glUseProgram(textureShader);
	//	glUniformMatrix4fv(glGetUniformLocation(lineShader, "Model"), 1,GL_FALSE, transform);
	//	glUniformMatrix4fv(glGetUniformLocation(lineShader, "View"), 1,	 GL_FALSE, glm::value_ptr(View));
	//	glUniformMatrix4fv(glGetUniformLocation(lineShader, "Projection"), 1, GL_FALSE, glm::value_ptr(Projection));

	//	glUniform1i(glGetUniformLocation(lineShader, "Tint"), rgba);

	//	glUniform2f(glGetUniformLocation(lineShader, "Position1"), x1, y1);
	//	glUniform2f(glGetUniformLocation(lineShader, "Position2"), x2, y2);

	//	glUniform1i(glGetUniformLocation(lineShader, "Tint"), rgba);

	//}

	//void drawCircle(float x, float y, float radius, unsigned rgba, unsigned steps, const float transform[16])
	//{
	//	const float step = PI / (steps-1);

	//	for (float i = 0; i <= 2 * PI; i += step)
	//	drawLine(x + cos(i  ) * radius, y + sin(i    ) * radius,
	//			 x + cos(i+1) * radius, y + sin(i + 1) * radius,
	//			rgba,	transform);
	//}

	bool initContext(unsigned width, unsigned height, const char * title)
	{
		if (!glfwInit()) return false;
		window = glfwCreateWindow( width, height, title, nullptr, nullptr );
		if (!window) { glfwTerminate(); return false; }
		glfwMakeContextCurrent(window);
		if (!ogl_LoadFunctions()) { termContext(); return false; }

		quad = makeVAO(QuadVerts, 4, QuadTris, 6);

		const char *vsourceTex = "#version 410\n\
				layout(location = 0) in vec4 Position;\
				layout(location = 2) in vec2 TexCoord;\
				out vec2 vTexCoord;\
				uniform int rows = 16;\
				uniform int cols = 16;\
				uniform int index = 5;\
				void main()\
				{\
				float c = (index % rows)/16.f; \
				float r = (index / rows)/16.f; \
				vTexCoord = (TexCoord + vec2(c,r))/16.f;\
				gl_Position = Position;\
				}";

		const char *fsourceTex = "#version 410\nout vec4 outColor; in vec2 vTexCoord; uniform sampler2D Texture; void main() { outColor = texture(Texture,vTexCoord); }";

		const char *vsourceLine = "";
		const char *fsourceLine = "";

		textureShader = makeShader(vsourceTex, fsourceTex);
		lineShader	  = makeShader(vsourceLine, fsourceLine);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return true;
	}

	void termContext()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool stepContext()
	{
		float t = getTime();
		deltaTime = t - lastTime;
		lastTime = t;

		glfwPollEvents();
		glfwSwapBuffers(window);

		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		return !glfwWindowShouldClose(window);
	}

	bool sfw::getKey(unsigned keycode)
	{
		return glfwGetKey(window, keycode) == GLFW_PRESS;
	}

	bool sfw::getMouseButton(unsigned mousekeycode)
	{
		return glfwGetMouseButton(window, mousekeycode) == GLFW_PRESS;
	}

	float sfw::getMouseX()
	{
		double x;
		glfwGetCursorPos(window, &x, nullptr);
		return (float)x;
	}

	float sfw::getMouseY()
	{
		double y;
		glfwGetCursorPos(window, nullptr, &y);
		return (float)y;
	}

	float sfw::getTime()	{ return (float)glfwGetTime(); }

	float sfw::getDeltaTime() { return deltaTime; }

}