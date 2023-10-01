#pragma once
#include "Texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

namespace noa {
	class GLTexture :public Texture
	{
	public:
		GLTexture(int w, int h, uint32_t* pixelBuffer);
		~GLTexture();

		void UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height) override;
		void Bind();
		void EnableAlpha() override;
		int GetWidth();
		int GetHeight();

	private:
		GLuint textureID;
		int width;
		int height;
	};
}

