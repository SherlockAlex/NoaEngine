#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <SDL2/SDL_image.h>

class NoaTexture {
public:
    NoaTexture(const char* imagePath);
    ~NoaTexture();

    void Bind();
    int GetWidth();
    int GetHeight();

private:
    GLuint textureID;
    int width;
    int height;
};

NoaTexture::NoaTexture(const char* imagePath) {
    // ����ͼ��
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return;
    }

    // ����OpenGL����
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // �����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ��ͼ�����ݴ��ݸ�OpenGL����
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // ��ȡͼ��Ŀ�Ⱥ͸߶�
    width = surface->w;
    height = surface->h;

    // �ͷ�SDL����
    SDL_FreeSurface(surface);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

NoaTexture::~NoaTexture() {
    glDeleteTextures(1, &textureID);
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

class NoaRenderer {
public:
    NoaRenderer();
    ~NoaRenderer();

    void Clear();
    void Present();
    void DrawTexture(NoaTexture* texture, int x, int y, int w, int h);

private:
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint shaderProgram; // �����ɫ���������

    // �����ɫ������
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
    )";
};

NoaRenderer::NoaRenderer() {



    // ����SDL���ں�OpenGL�����ģ��ڳ�ʼ�����Ѿ���ɣ�
    window = SDL_CreateWindow("OpenGL Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    glContext = SDL_GL_CreateContext(window);

    // ���û����֧��͸����
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ������ɫ������
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // ���붥����ɫ����Ƭ����ɫ��
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    // ������ɫ������������ɫ��
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

NoaRenderer::~NoaRenderer() {
    glDeleteProgram(shaderProgram); // ɾ����ɫ������
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}

void NoaRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void NoaRenderer::Present() {
    SDL_GL_SwapWindow(window);
}

void NoaRenderer::DrawTexture(NoaTexture* texture, int x, int y, int w, int h) {
    // ʹ��OpenGL��������
    glUseProgram(shaderProgram); // ʹ����ɫ������
    texture->Bind();

    // ������ɫ���е�Uniform����
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + w, y);
    glTexCoord2f(1, 1); glVertex2i(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2i(x, y + h);
    glEnd();
}

int main(int argc,char * argv[])
{
    // ��ʼ��SDL2��OpenGL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    //SDL_Window* window = SDL_CreateWindow("OpenGL Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    //SDL_GLContext glContext = SDL_GL_CreateContext(window);

    //��ʼ��OpenGL
    if (!glfwInit()) {
        printf("Failed to initialize GLFW");
        exit(-1);
    }

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW");
        exit(-1);
    }

    NoaRenderer noaRenderer;
    NoaTexture texture1("Mario_Idle.png");
    NoaTexture texture2("Mario_Idle.png");
    NoaTexture texture3("Mario_Idle.png");

    SDL_Event event;

    while (true) {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) 
            {
                SDL_Quit();
                return 0;
            }
        }

        noaRenderer.Clear();

        // ��Ⱦͼ��
        int screenWidth = 800;
        int screenHeight = 600;
        noaRenderer.DrawTexture(&texture1, 0, 0, screenWidth, screenHeight);
        noaRenderer.DrawTexture(&texture2, 0, 0, screenWidth / 2, screenHeight / 2);
        noaRenderer.DrawTexture(&texture3, screenWidth / 2, screenHeight / 2, screenWidth / 4, screenHeight / 4);
        
        noaRenderer.Present();
    }

    return 0;
}