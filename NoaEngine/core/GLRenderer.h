#ifndef NOAENGINE_GLRENDERER_H
#define NOAENGINE_GLRENDERER_H

#include "Renderer.h"

namespace noa {
	class GLRenderer :public Renderer
	{
	public:
		Texture* CreateTexture(int w, int h, uint32_t* pixelBuffer) override;

		GLRenderer();
		~GLRenderer();

		void InitRenderer() override;
		void Clear() override;
		void Present(GLFWwindow* window);
		void DrawTexture(Texture* texture, int index, int x, int y, int w, int h, float eulerAngle = 0, bool isFlipX = false) override;
		void SetContext(SDL_Window* windows) override;
		void Present(SDL_Window* windows) override;

	private:
		SDL_GLContext context;

		GLuint VAO, VBO, EBO;
		GLint modelLoc, projectionLoc;

		GLuint fragmentShader;

		GLuint shaderProgram;

		GLuint vertexShader;

		const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)glsl";

		const char* fragmentShaderSource = R"glsl(
// Æ¬¶Î×ÅÉ«Æ÷
#version 330 core
out lowp vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    lowp vec4 texColor = texture(ourTexture, TexCoord);
    FragColor = texColor;
}
)glsl";

	};
}

#endif

