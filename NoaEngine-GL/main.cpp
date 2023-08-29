#include "NoaEngine.h"
#include <iostream>

// 假设你已经创建了uint32_t类型的像素缓冲区，名为pixelBuffer  
// 并且你知道它的宽度和高度，分别为width和height  


const int width = 1920 / 2;
const int height = 1080 / 2;
uint32_t* pixelBuffer;

GLuint pixelBufferToTexture(uint32_t* pixelBuffer, int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

void DrawPixel(const int x,const int y,const uint32_t color)
{
    pixelBuffer[y * width + x] = color;
}

int main() {
    pixelBuffer = new uint32_t[width * height];

    for (int i=0;i<width * height;i++)
    {
        pixelBuffer[i] = 0xFF0000FF;
    }

    // 初始化GLFW  
    glfwInit();

    // 创建一个窗口，宽度为800，高度为600  
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化GLEW  
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // 将像素缓冲区转换为OpenGL纹理格式  
    GLuint texture = pixelBufferToTexture(pixelBuffer, width, height);

    // 创建一个帧缓冲区对象并将其附加到纹理对象上  
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // 检查帧缓冲区的完整性  
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (int x = 0;x<width;x++)
    {
        for (int y = 0; y < height;y++)
        {
            uint32_t color = 0xFF0000FF; // 红色  ;
            DrawPixel(x, y, color);
        }
    }

    // 在屏幕上绘制纹理  
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(1); // 使用默认的着色器程序（即渲染管线）  
        glBindVertexArray(1); // 使用默认的顶点数组对象（即渲染管线）  
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // 将帧缓冲区绑定到渲染管线上  
        glViewport(0, 0, width, height); // 设置视口的大小和位置，使其与像素缓冲区的大小和位置相匹配  
        glDrawArrays(GL_TRIANGLES, 0, 3); // 在屏幕上绘制一个三角形（即纹理）  
        glfwSwapBuffers(window); // 交换前后缓冲区，使新的一帧显示出来  
        glfwPollEvents(); // 处理事件（如用户输入）和窗口消息（如窗口大小改变）  
    }
    glDeleteFramebuffers(1, &framebuffer); // 删除帧缓冲区对象及其关联的纹理对象和纹理数据缓冲区对象（如果有的话）  
    glDeleteTextures(1, &texture); // 删除纹理对象及其关联的纹理数据缓冲区对象（如果有的话）  
    glfwTerminate(); // 关闭GLFW库并释放其占用的资源（包括窗口和上下文）  
    
    delete pixelBuffer;
    return 0;
}