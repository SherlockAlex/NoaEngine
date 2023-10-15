#ifdef _WIN64

#ifndef NOAENGINE_GLRENDERER_H
#define NOAENGINE_GLRENDERER_H

#include "GLHelper.h"
#include "Renderer.h"

namespace noa {
	class GLRenderer :public Renderer
	{
	private:
		bool isInit = false;
	public:
		Texture* CreateTexture(int w, int h, uint32_t* pixelBuffer) override;

		GLRenderer();
		~GLRenderer();

		void InitRenderer() override;
		void Clear() override;
		void DrawTexture(Texture* texture, int index, int x, int y, int w, int h, unsigned int tint, float eulerAngle, bool isFlipX) override;
		void SetContext(SDL_Window* windows) override;
		void Present(SDL_Window* windows) override;

	private:
		SDL_GLContext context = nullptr;

		GLuint fragmentShader;

		GLuint shaderProgram;

		GLuint vertexShader;

		int tintLocation;

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
// 片段着色器
#version 330 core
out vec4 FragColor;		//最终要显示的颜色
in vec2 TexCoord;

uniform sampler2D picture;	//采样图片
uniform vec4 tint;

extern vec4 getColor();

void main()
{
	vec4 finalColor = getColor();
    FragColor = finalColor;
}

vec4 getColor(){
    vec4 texColor = texture(picture, TexCoord);
	const float invGrayValue = 1.0f/255;
	vec4 finalColor = texColor * tint * invGrayValue;
	return finalColor;
}

)glsl";

	};
}

#endif

#endif // _WIN64

