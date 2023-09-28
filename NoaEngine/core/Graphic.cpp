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
// 片段着色器
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

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 创建纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

    }

    NoaTexture::~NoaTexture() {
        glDeleteTextures(1, &textureID);
    }

    void NoaTexture::UpdateTexture(uint32_t* pixelBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        // 更新纹理数据
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

        // 创建和编译着色器程序
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
            // 顶点坐标        纹理坐标
             1.0f,  1.0f,  1.0f, 0.0f, // 右下角
             1.0f, -1.0f,  1.0f, 1.0f, // 右上角
            -1.0f, -1.0f,  0.0f, 1.0f, // 左上角
            -1.0f,  1.0f,  0.0f, 0.0f  // 左下角
        };

        unsigned int indices[6] = {
           0, 1, 3, // 第一个三角形
           1, 2, 3  // 第二个三角形
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

        // 获取着色器中的uniform位置
        modelLoc = glGetUniformLocation(shaderProgram, "model");

    }

    NoaRenderer::~NoaRenderer() {
        glDeleteProgram(shaderProgram); // 删除着色器程序
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

        // 如果isFlipX = true 关于 y = 0.5*w 进行反转

        // 使用OpenGL绘制纹理
        glUseProgram(shaderProgram);

        // 渲染矩形
        glBindVertexArray(VAO);
        texture->Bind();
        
        const float invPixelWidth = 1.0 / pixelWidth;
        const float invPixelHeight = 1.0 / pixelHeight;

        const float left = 2.0f * x * invPixelWidth - 1.0f;
        const float right = 2.0f * (x + w) * invPixelWidth - 1.0f;
        const float bottom = -(2.0f * y * invPixelHeight - 1.0f);
        const float top = -(2.0f * (y + h) * invPixelHeight - 1.0f);

        float vertices[] = {
            // 顶点坐标        纹理坐标
             right,  bottom,    1.0f, 0.0f, // 右下角
             right,  top,       1.0f, 1.0f, // 右上角
             left,   top,       0.0f, 1.0f, // 左上角
             left,   bottom,    0.0f, 0.0f  // 左下角
        };

        if (isFlipX)
        {
            vertices[2] = 0.0;
            vertices[6] = 0.0;
            vertices[10] = 1.0;
            vertices[14] = 1.0;

        }
        

        // 设置矩形的索引
        unsigned int indices[] = {
           0, 1, 3, // 第一个三角形
           1, 2, 3  // 第二个三角形
        };

        // 更新顶点缓冲区数据
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 更新索引缓冲区数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 绘制矩形
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

    }

}




