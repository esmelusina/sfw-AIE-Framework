

#include <ogl\gl_core_4_4.h>

#include <glfw\glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw\glfw3native.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <iostream>
#include <unordered_map>

#include "sfwdraw.h"
#include "crenderutils.h"


#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace sfw
{
	static bool isDead;
	static GLFWwindow *window;
	static RenderObject quad;

	static ShaderProgram textureShader;
	static ShaderProgram lineShader;

	static float lastTime;
	static float deltaTime;

	static glm::mat4 cameraProjection;
	static glm::mat4 cameraView;

	glm::vec4 hexToVec4(unsigned tint)
	{
		//struct RGB colorConverter(int hexValue)
		//{
		//	struct RGB rgbColor;
		//	rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
		//	rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
		//	rgbColor.b = ((hexValue)& 0xFF) / 255.0;        // Extract the BB byte

		//	return rgbColor;
		//}
		return glm::vec4(
			(tint >> 24) & 0xff,
			(tint >> 16) & 0xff,
			(tint >>  8) & 0xff,
			(tint >>  0) & 0xff)/255.f;

		//return glm::vec4(glm::make_vec4((unsigned char*)&tint)*1.f) / 255;
	}


	struct Texture { const unsigned handle; const unsigned int r, c, w, h, f; }; //opengl handle, atlas rows, atlas cols, width, height, color channels

	static std::unordered_map<unsigned,Texture> textures; // map our texture data to the handles we provide via loadTextureMap

	void setProjectionMatrix(const float transform[16]) { cameraProjection = glm::make_mat4(transform); }
	void setViewMatrix(const float transform[16])		{ cameraView = glm::make_mat4(transform); }

	unsigned getTextureWidth(unsigned handle) { if (!textures.count(handle)) { std::cerr << "Invalid texture handle." << std::endl; return 0; }; return textures.at(handle).w; }
	unsigned getTextureHeight(unsigned handle) { if (!textures.count(handle)) { std::cerr << "Invalid texture handle." << std::endl; return 0; };  return textures.at(handle).h; }

	unsigned loadTextureMap(const char * path, unsigned rows, unsigned cols)
	{
		int w, h, f;
		unsigned d = 0;
		auto p = stbi_load(path, &w, &h, &f, STBI_default);
		switch (f)
		{
		case 1: d = GL_RED;	break; case 2: d = GL_RG; break; case 3: d = GL_RGB; break; case 4: d = GL_RGBA; break;
		default:std::cerr << path << ": File not found or incorrect format." << std::endl; return 0;
		}

		auto td = makeTextureSPRITE(w, h, d, p);

		stbi_image_free(p);

		Texture t = { td.handle, rows, cols, (unsigned)w, (unsigned)h, (unsigned)f };
		textures.insert(std::pair<unsigned,Texture>(textures.size()+1, t));
		return textures.size();
	}

	void drawTextureMatrix(unsigned handle, unsigned index, unsigned int tint, const float transform[16])
	{
		if (!textures.count(handle)) { std::cerr << "Invalid texture handle, draw operation aborted." << std::endl; return; }

		glUseProgram(textureShader.handle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.at(handle).handle);

		glUniform1i(glGetUniformLocation(textureShader.handle, "Texture"), 0);
		glUniform1i(glGetUniformLocation(textureShader.handle, "rows"), textures.at(handle).r);
		glUniform1i(glGetUniformLocation(textureShader.handle, "cols"), textures.at(handle).c);
		glUniform1i(glGetUniformLocation(textureShader.handle, "index"), index);

		glUniformMatrix4fv(glGetUniformLocation(textureShader.handle, "View"), 1, GL_FALSE, glm::value_ptr(cameraView));
		glUniformMatrix4fv(glGetUniformLocation(textureShader.handle, "Projection"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
		glUniformMatrix4fv(glGetUniformLocation(textureShader.handle, "Model"), 1, GL_FALSE, transform);

		auto t = hexToVec4(tint);
		glUniform4fv(glGetUniformLocation(textureShader.handle, "Tint"), 1, glm::value_ptr(t));

		glBindVertexArray(quad.vao);
		glDrawElements(GL_TRIANGLES, quad.size, GL_UNSIGNED_INT, 0);
		glUseProgram(0);
	}

	void drawLineMatrix(unsigned tint, const float transform[16])
	{
		glUseProgram(lineShader.handle);

		glUniformMatrix4fv(glGetUniformLocation(lineShader.handle, "View"), 1, GL_FALSE, glm::value_ptr(cameraView));
		glUniformMatrix4fv(glGetUniformLocation(lineShader.handle, "Projection"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
		glUniformMatrix4fv(glGetUniformLocation(lineShader.handle, "Model"), 1, GL_FALSE, transform);
		
		auto t = hexToVec4(tint);
		glUniform4fv(glGetUniformLocation(lineShader.handle, "Tint"), 1, glm::value_ptr(t));

		glBindVertexArray(quad.vao);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		glUseProgram(0);
	}

	void drawLine(float x1, float y1, float x2, float y2, unsigned tint, const float transform[16])
	{
		drawLineMatrix(tint, glm::value_ptr(glm::make_mat4(transform)*glm::translate(x1,y1,0.f) * glm::scale(x2-x1, y2-y1, 1.f) * glm::translate(0.5f, 0.5f, 0.f)));
	}

	void drawCircle(float x, float y, float radius, unsigned steps, unsigned tint, const float transform[16])
	{
		const float step = (float)PI / (float)steps;

		for (float i = step; i < 2 * PI; i += step)
			drawLine(x + cos(i)*radius, y+sin(i)*radius, x+cos(i-step)* radius, y+sin(i-step) * radius,tint,transform);
	}

	void drawTexture(unsigned handle, float x, float y, float w, float h, float r, bool centered, unsigned index, unsigned int tint, const float transform[16])
	{
		drawTextureMatrix(handle, index,tint, glm::value_ptr(glm::make_mat4(transform) * glm::translate(x,y,0.f) *glm::rotate(r, 0.f, 0.f, 1.f)* glm::scale(w,h,0.f) * (centered? glm::mat4(1) : glm::translate(0.5f, -0.5f, 0.f))));
	}

	void drawString(unsigned handle, const char *text, float a_x, float a_y, float w, float h, float r, char o, unsigned int tint, const float transform[16])
	{
		float x = 0, y = 0;
		glm::mat4 local = glm::make_mat4(transform);
		glm::mat4 model = glm::translate(a_x, a_y, 0.f) * glm::rotate(r, 0.f, 0.f, 1.f) * glm::scale(w, h, 0.f) * glm::translate(0.5f,-0.5f,0.f);
		for (size_t i = 0; i < strlen(text); ++i)
		{
			if (text[i] == '\n' || text[i] == '\r') { y += 1; x = 0; }
			else if (text[i] == '\t') x += 4;
			else
			{
				drawTextureMatrix(handle, text[i]-o,tint, glm::value_ptr(local * model * glm::translate(x, y, 0.f)));
				x += 1;
			}
		}
	}

	bool initContext(unsigned width, unsigned height, const char * title)
	{
		if (isDead) { std::cerr << "SFW may only properly initialize a window once.\nWhine to esme @ esmes@aie.edu.au and she might implement a resizeContext and/or setFullscreen function." << std::endl; }
		if (!glfwInit()) return false;
		window = glfwCreateWindow( width, height, title, nullptr, nullptr );
		if (!window) { glfwTerminate(); return false; }
		glfwMakeContextCurrent(window);
		if (!ogl_LoadFunctions()) { termContext(); return false; }

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSwapInterval(1);

		auto ldr = glfwGetWin32Window(window);
		DWORD dwStyle = GetWindowLong(ldr, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZE | WS_SIZEBOX | WS_MAXIMIZEBOX);
		SetWindowLong(ldr, GWL_STYLE, dwStyle);


		quad = makeVAO(QuadVerts, 4, QuadTris, 6);

		const char *vsourceTex =   "#version 330\n\
									layout(location = 0) in vec4 Position;\
									layout(location = 2) in vec2 TexCoord;\
									out vec2 vTexCoord;\
									uniform mat4 Projection;\
									uniform mat4 View;\
									uniform mat4 Model;\
									uniform int rows = 1;\
									uniform int cols = 1;\
									uniform int index = 0;\
									void main()\
									{\
										float c = (index % cols) % cols; \
										float r = (index / cols) % rows; \
										vTexCoord = (vec2(c,r) + TexCoord) / vec2(rows,cols);\
										gl_Position = Projection * View * Model * Position;\
									}";

		const char *fsourceTex = "#version 330\n\
								  out vec4 outColor;\
								  in vec2 vTexCoord;\
								  uniform vec4 Tint = vec4(1,1,1,1); \
							      uniform sampler2D Texture; \
								  void main() { outColor = texture(Texture,vTexCoord) * Tint;  }";

		const char *vsourceLine =   "#version 330\n\
									layout(location = 0) in vec4 Position;\
									uniform mat4 Projection;\
									uniform mat4 View;\
									uniform mat4 Model;\
									void main()\
									{\
										gl_Position = Projection * View * Model * Position;\
									}";

		const char *fsourceLine = "#version 330\n\
								   out vec4 outColor;\
								   uniform vec4 Tint = vec4(1,1,1,1); \
								   void main() { outColor = Tint;  }";

		textureShader = makeShader(vsourceTex, fsourceTex);
		lineShader = makeShader(vsourceLine, fsourceLine);

		cameraProjection = glm::ortho(0.f, (float)width, 0.f, (float)height);
		cameraView = glm::mat4(1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.25f, 0.25f, 0.25f, 1);

		return true;
	}

	void termContext()
	{
		glDeleteBuffers(1, &quad.ibo);
		glDeleteBuffers(1, &quad.vbo);
		glDeleteVertexArrays(1, &quad.vao);
		glDeleteProgram(textureShader.handle);
		glDeleteProgram(lineShader.handle);

		for each(auto t in textures)
			glDeleteTextures(1, &t.second.handle);		
		

		glfwDestroyWindow(window);
		glfwTerminate();
		window = nullptr;
		isDead = true;
	}

	bool stepContext()
	{
		if (!window) return false;

		float t = getTime();
		deltaTime = t - lastTime;
		lastTime = t;

		glfwPollEvents();
		glfwSwapBuffers(window);

		glClear(GL_COLOR_BUFFER_BIT);

		return !glfwWindowShouldClose(window);
	}

	void setBackgroundColor(unsigned int tint)
	{
		auto t = hexToVec4(tint);
		glClearColor(t[0], t[1], t[2], t[3]);
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
		int h;
		double y;
		glfwGetCursorPos(window, nullptr, &y);
		glfwGetWindowSize(window, nullptr, &h);
		return (float)h - (float)y;
	}

	float sfw::getTime()	{ return (float)glfwGetTime(); }

	float sfw::getDeltaTime() { return deltaTime; }

}