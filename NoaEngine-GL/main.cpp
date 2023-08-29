#include "NoaEngine.h"
#include <iostream>

// �������Ѿ�������uint32_t���͵����ػ���������ΪpixelBuffer  
// ������֪�����Ŀ�Ⱥ͸߶ȣ��ֱ�Ϊwidth��height  


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

    // ��ʼ��GLFW  
    glfwInit();

    // ����һ�����ڣ����Ϊ800���߶�Ϊ600  
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ��ʼ��GLEW  
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // �����ػ�����ת��ΪOpenGL�����ʽ  
    GLuint texture = pixelBufferToTexture(pixelBuffer, width, height);

    // ����һ��֡���������󲢽��丽�ӵ����������  
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // ���֡��������������  
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (int x = 0;x<width;x++)
    {
        for (int y = 0; y < height;y++)
        {
            uint32_t color = 0xFF0000FF; // ��ɫ  ;
            DrawPixel(x, y, color);
        }
    }

    // ����Ļ�ϻ�������  
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(1); // ʹ��Ĭ�ϵ���ɫ�����򣨼���Ⱦ���ߣ�  
        glBindVertexArray(1); // ʹ��Ĭ�ϵĶ���������󣨼���Ⱦ���ߣ�  
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // ��֡�������󶨵���Ⱦ������  
        glViewport(0, 0, width, height); // �����ӿڵĴ�С��λ�ã�ʹ�������ػ������Ĵ�С��λ����ƥ��  
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����Ļ�ϻ���һ�������Σ�������  
        glfwSwapBuffers(window); // ����ǰ�󻺳�����ʹ�µ�һ֡��ʾ����  
        glfwPollEvents(); // �����¼������û����룩�ʹ�����Ϣ���細�ڴ�С�ı䣩  
    }
    glDeleteFramebuffers(1, &framebuffer); // ɾ��֡��������������������������������ݻ�������������еĻ���  
    glDeleteTextures(1, &texture); // ɾ�����������������������ݻ�������������еĻ���  
    glfwTerminate(); // �ر�GLFW�Ⲣ�ͷ���ռ�õ���Դ���������ں������ģ�  
    
    delete pixelBuffer;
    return 0;
}