#include "Graphic.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

namespace noa {
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
// Ƭ����ɫ��
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

    NoaTexture::NoaTexture(int w, int h, uint32_t* pixelBuffer)
        : width(w), height(h) 
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // �����������
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ��������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

    }

    NoaTexture::~NoaTexture() {
        glDeleteTextures(1, &textureID);
    }

    void NoaTexture::UpdateTexture(uint32_t* pixelBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        // ������������
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
    }

    void NoaTexture::Bind() {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    int NoaTexture::GetWidth() {
        return width;
    }

    int NoaTexture::GetHeight() {
        return height;
    }

    NoaRenderer::NoaRenderer() {

        // �����ͱ�����ɫ������
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

    NoaRenderer::~NoaRenderer() {
        glDeleteProgram(shaderProgram); // ɾ����ɫ������
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void NoaRenderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    

    void NoaRenderer::Present(GLFWwindow* window) {
        glfwSwapBuffers(window);
    }

    void NoaRenderer::DrawTexture(NoaTexture* texture, int x, int y, int w, int h, bool isFlipX) {

        // ���isFlipX = true ���� y = 0.5*w ���з�ת

        // ʹ��OpenGL��������
        glUseProgram(shaderProgram);

        // ��Ⱦ����
        glBindVertexArray(VAO);
        texture->Bind();
        
        const float invPixelWidth = 1.0 / pixelWidth;
        const float invPixelHeight = 1.0 / pixelHeight;

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

}




