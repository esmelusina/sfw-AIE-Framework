#include <ogl\gl_core_4_4.h>
#include <glfw\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include "sfwdraw.h"
#include <iostream>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include <cctype>

namespace sfw
{
	static GLFWwindow *window;

	static unsigned textureShader;
	static unsigned lineShader;

	static float lastTime;
	static float deltaTime;

	static glm::mat4 Projection;
	static glm::mat4 View;

	// glhandle, rows, cols, width, height, element width/height, format
	struct Texture { unsigned handle, r, c, w, h, ew, eh, f; };

	static std::vector<Texture> textures;


	unsigned loadTextureMap(const char * path, unsigned rows, unsigned cols)
	{
		int w, h, f;
		auto p = stbi_load(path,&w,&h,&f,STBI_default);
		switch (f)
		{
		case 1: f = GL_RED;		break;
		case 2: f = GL_RG;		break;
		case 3: f = GL_RGB;		break;
		case 4: f = GL_RGBA;	break;
		default:std::cerr << "File not found or incorrect format." << std::endl; return 0;
		}

		unsigned handle;
		glGenTextures(1,&handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, p);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(p);
		textures.push_back({handle, rows,cols,w,h,w/rows,h/cols,f});
		return textures.size()-1;
	}

	void drawTextureMap(unsigned handle, float x, float y, float scale, float angle, unsigned rgba, bool center, unsigned idx, const float transform[16])
	{
		glUseProgram(textureShader);
		glm::mat4 offset = glm::mat4(1);
		if (!center) offset = glm::translate(-(float)textures[handle].ew , -(float)textures[handle].eh, 0.f);

		glm::mat4 model = glm::mat4(transform) * glm::scale(scale, scale, scale)  * glm::rotate(angle, glm::vec3(0,1,0))  * glm::translate(x, y, 0.f) * offset;

		
		glUniformMatrix4fv(glGetUniformLocation(textureShader, "Model"),1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(textureShader, "View"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(textureShader, "Projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		
		glUniform1i(glGetUniformLocation(textureShader, "Rows"), textures[handle].r);
		
		glUniform1i(glGetUniformLocation(textureShader, "Cols"), textures[handle].h);
		
		glUniform1i(glGetUniformLocation(textureShader, "Index"), idx);
		glUniform1i(glGetUniformLocation(textureShader, "Tint"), rgba);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[handle].handle);
		glUniform1i(glGetUniformLocation(textureShader, "TextureMap"), 0);
	}

	void drawString(const char *cstring, unsigned handle, float a_x, float a_y, float scale, float angle, unsigned rgba, bool center, unsigned offset, const float transform[16])
	{
		float x, y;
		for (int i = 0; i < strlen(cstring); ++i)
		{
			if (cstring[i] == '\n' || cstring[i] == '\r')
			{
				x = a_x;
				y += textures[handle].eh;
			}
			else if (cstring[i] == '\t')
			{
				x += textures[handle].ew * 4;
			}
			else if(cstring[i] < 126 && cstring[i] > 32)
			{
				drawTextureMap(handle, x, y, angle, scale, rgba, center, cstring[i] - offset, transform);
				x += textures[handle].ew;
			}
		}
	}

	void drawLine(float x1, float y1, float x2, float y2, unsigned rgba, const float transform[16])
	{
		glUseProgram(textureShader);
		glUniformMatrix4fv(glGetUniformLocation(lineShader, "Model"), 1,GL_FALSE, transform);
		glUniformMatrix4fv(glGetUniformLocation(lineShader, "View"), 1,	 GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(lineShader, "Projection"), 1, GL_FALSE, glm::value_ptr(Projection));

		glUniform1i(glGetUniformLocation(lineShader, "Tint"), rgba);

		glUniform2f(glGetUniformLocation(lineShader, "Position1"), x1, y1);
		glUniform2f(glGetUniformLocation(lineShader, "Position2"), x2, y2);

		glUniform1i(glGetUniformLocation(lineShader, "Tint"), rgba);

	}

	void drawCircle(float x, float y, float radius, unsigned rgba, unsigned steps, const float transform[16])
	{
		const float step = PI / (steps-1);

		for (float i = 0; i <= 2 * PI; i += step)
		drawLine(x + cos(i  ) * radius, y + sin(i    ) * radius,
				 x + cos(i+1) * radius, y + sin(i + 1) * radius,
				rgba,	transform);
	}

	bool initContext(unsigned width, unsigned height, const char * title)
	{
		glfwInit();
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);
		ogl_LoadFunctions();
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
	}

	bool sfw::getKey(unsigned keycode)
	{
		return glfwGetKey(window, keycode);
	}

	bool sfw::getMouseButton(unsigned mousekeycode)
	{
		return glfwGetMouseButton(window, mousekeycode);
	}

	float sfw::getMouseX()
	{
		double x;
		glfwGetCursorPos(window, &x, nullptr);
		return x;
	}

	float sfw::getMouseY()
	{
		double y;
		glfwGetCursorPos(window, nullptr, &y);
		return y;
	}

	float sfw::getTime()
	{
		glfwGetTime();
	}

	float sfw::getDeltaTime() { return deltaTime; }

}