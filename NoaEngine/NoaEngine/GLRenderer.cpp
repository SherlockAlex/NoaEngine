#ifdef _WIN64

#include "Graphic.h"
#include "Debug.h"
#include "GLRenderer.h"
#include "GLTexture.h"

namespace noa {
    Texture* GLRenderer::CreateTexture(int w, int h, uint32_t* pixelBuffer)
    {
        Texture* texture = new GLTexture(w,h,pixelBuffer);
        return texture;
    }
    GLRenderer::GLRenderer():Renderer() {

    }

    GLRenderer::~GLRenderer() {

        glDeleteProgram(shaderProgram);
    }

    void GLRenderer::InitRenderer()
    {

        if (glewInit() != GLEW_OK) {
            Debug::Error("Failed to initialize GLEW");
            exit(-1);
        }

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        tintLocation = glGetUniformLocation(shaderProgram,"tint");
        eulerAngleLocation = glGetUniformLocation(shaderProgram,"eulerAngle");

        // ʹ��OpenGL��������
        glUseProgram(shaderProgram);
    }

    void GLRenderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::DrawTexture(Texture* tex, int index, int x, int y, int w, int h,unsigned int tint,float eulerAngle, bool isFlipX)
    {
        
        //����һ�����һ��VAO

        GLTexture* texture = tex->GetTextureAs<GLTexture>();
        if (texture == nullptr) 
        {
            return;
        }

        glActiveTexture(GL_TEXTURE + index);
        glUniform4f(tintLocation, GetRValue(tint), GetGValue(tint), GetBValue(tint), GetAValue(tint));
        glUniform1f(eulerAngleLocation,eulerAngle);

        texture->Bind();
        const float left = 2.0f * x * invPixelWidth - 1.0f;
        const float right = 2.0f * (x + w) * invPixelWidth - 1.0f;
        const float bottom = -(2.0f * y * invPixelHeight - 1.0f);
        const float top = -(2.0f * (y + h) * invPixelHeight - 1.0f);

        float vertices[] = {
            // ��������        ��������
             right,  bottom,    1.0f, 0.0f, // ���½�
             right,  top,       1.0f, 1.0f, // ���Ͻ�
             left,   top,       0.0f, 1.0f, // ���Ͻ�
             left,   bottom,    0.0f, 0.0f  // ���½�
        };

        if (isFlipX)
        {
            vertices[2] = 0.0;
            vertices[6] = 0.0;
            vertices[10] = 1.0;
            vertices[14] = 1.0;
        }

        // ���¶��㻺��������
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // ������������������
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }

    void GLRenderer::SetContext(SDL_Window* windows)
    {
        this->context = SDL_GL_CreateContext(windows);
        if (context == nullptr) 
        {
            Debug::Error("Create GL Context failed");
            exit(-1);
        }
        SDL_GL_SetSwapInterval(0);
    }

    void GLRenderer::Present(SDL_Window* windows)
    {
        SDL_GL_SwapWindow(windows);
    }

}

#endif //_WIN64