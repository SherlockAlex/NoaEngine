#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoord;

uniform float eulerAngle;
uniform mat4 projection;

void main()
{
    texCoord = vertex.zw;
    mat4 ratate = mat4(
        cos(eulerAngle),-sin(eulerAngle),0.0f,0.0f
        ,sin(eulerAngle),cos(eulerAngle),0.0f,0.0f
        ,0.0f,0.0f,1.0f,0.0f
        ,0.0f,0.0f,0.0f,1.0f
    );
    gl_Position = ratate*projection*vec4(vertex.xy, 0.0f, 1.0f);
}