#ifdef _WIN64

#include "GLRenderer.h"
#include "NoaEngine.h"
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
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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


        float vertices[16] = {
            // ��������        ��������
             1.0f,  1.0f,  1.0f, 0.0f, // ���½�
             1.0f, -1.0f,  1.0f, 1.0f, // ���Ͻ�
            -1.0f, -1.0f,  0.0f, 1.0f, // ���Ͻ�
            -1.0f,  1.0f,  0.0f, 0.0f  // ���½�
        };

        unsigned int indices[6] = {
           0, 1, 3, // ��һ��������
           1, 2, 3  // �ڶ���������
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // ��ȡ��ɫ���е�uniformλ��
        modelLoc = glGetUniformLocation(shaderProgram, "model");
    }

    void GLRenderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::DrawTexture(Texture* tex, int index, int x, int y, int w, int h, float eulerAngle, bool isFlipX)
    {
        

        GLTexture* texture = dynamic_cast<GLTexture*>(tex);
        if (texture == nullptr) 
        {
            return;
        }

        glActiveTexture(GL_TEXTURE + index);
        // ʹ��OpenGL��������
        glUseProgram(shaderProgram);

        // ��Ⱦ����
        glBindVertexArray(VAO);
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

        // ���þ��ε�����
        unsigned int indices[] = {
           0, 1, 3, // ��һ��������
           1, 2, 3  // �ڶ���������
        };

        // ���¶��㻺��������
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // ������������������
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // ���ƾ���
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

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