#ifndef NOAENGINE_GRAPHIC_H
#define NOAENGINE_GRAPHIC_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "NoaMath.h"


namespace noa {

	typedef unsigned char Uint8;
	typedef unsigned int Uint32;

	// Color constant
	#define	BLACK			0xFF000000
	#define	BLUE			0xFFAA0000
	#define	GREEN			0xFF00AA00
	#define	CYAN			0xFFAAAA00
	#define	RED				0xFF0000AA
	#define	MAGENTA			0xFFAA00AA
	#define	BROWN			0xFF0055AA
	#define	LIGHTGRAY		0xFFAAAAAA
	#define	DARKGRAY		0xFF555555
	#define	LIGHTBLUE		0xFFFF5555
	#define	LIGHTGREEN		0xFF55FF55
	#define	LIGHTCYAN		0xFFFFFF55
	#define	LIGHTRED		0xFF5555FF
	#define	LIGHTMAGENTA	0xFFFF55FF
	#define	YELLOW			0xFF55FFFF
	#define	WHITE			0xFFFFFFFF
	#define ALPHA			0x00000000
	
	
	//取低位
	#define LOBYTE(w)           ((Uint8)(w&0xff))
	
	// Color conversion macro
	#define BGR(color)	((((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16))
	
	#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))
	
	#define GetRValue(rgb)      (LOBYTE(rgb))
	#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
	#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
	#define GetAValue(argb)      (LOBYTE((argb) >> 24))

	#define MULTICOLOR(color,mutiColor) (RGB(GetRValue(color)* (GetRValue(mutiColor) / 255.0), GetGValue(color)* (GetGValue(mutiColor) / 255.0), GetBValue(color)* (GetBValue(mutiColor) / 255.0)))
	#define ERRORCOLOR RGB(152,0,136)

	typedef Uint32 ColorRef;

	#define BRIGHTER(color,brightnessIncrease)(RGB(min(255, GetRValue(color) + brightnessIncrease), min(255, GetGValue(color) + brightnessIncrease), min(255, GetBValue(color) + brightnessIncrease)))
	#define DARKER(color,darknessIncrease)(RGB(max(0, GetRValue(color) - darknessIncrease), max(0, GetGValue(color) - darknessIncrease), max(0, GetBValue(color) - darknessIncrease)))

	extern Uint32* pixelBuffer;
	extern int pixelWidth;
	extern int pixelHeight;
	#define DRAWPIXEL(x,y,color) ((x < 0 || y < 0 || x >= pixelWidth || y >= pixelHeight)?( - 1):(((Uint32*)pixelBuffer)[y * pixelWidth + x] = color))


#pragma region OPENGL

	extern const char* vertexShaderSource;

	extern const char* fragmentShaderSource;

	extern GLuint fragmentShader;

	extern GLuint shaderProgram;

	extern GLuint vertexShader;
	
	class NoaTexture {
	public:
		NoaTexture(int w,int h,uint32_t * pixelBuffer);
		~NoaTexture();

		void UpdateTexture(uint32_t* pixelBuffer);
		void Bind();
		int GetWidth();
		int GetHeight();

	private:
		GLuint textureID;
		int width;
		int height;
	};

	class NoaRenderer {
	public:
		NoaRenderer();
		~NoaRenderer();

		void Clear();
		void Present(GLFWwindow * window);
		void DrawTexture(NoaTexture* texture, int x, int y, int w, int h);

	private:

		GLuint shaderProgram;
		GLuint VAO, VBO,EBO;
		GLint modelLoc, projectionLoc;

		/*// 添加着色器代码
		const char* vertexShaderCode = R"(
        #version 330 core
        layout(location = 0) in vec2 inPosition;
        layout(location = 1) in vec2 inTexCoord;
        out vec2 fragTexCoord;
        void main() {
            gl_Position = vec4(inPosition, 0.0, 1.0);
            fragTexCoord = inTexCoord;
        }
    )";

		const char* fragmentShaderCode = R"(
        #version 330 core
        in vec2 fragTexCoord;
        out vec4 fragColor;
        uniform sampler2D textureSampler;
        void main() {
            fragColor = texture(textureSampler, fragTexCoord);
        }
    )";*/


	};


#pragma endregion


}

#endif // !NOAENGINE_GRAPHIC