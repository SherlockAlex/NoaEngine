#version 330 core //使用OpenGL3.3版本的核心模式

layout(location = 0) in vec2 aPos;

void main(){
    gl_Position = vec4(aPos,0.0,1.0);
}