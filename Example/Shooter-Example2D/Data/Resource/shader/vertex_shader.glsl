#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoord;

uniform float eulerAngle;
uniform mat4 projection;

void main()
{
    vec2 originTextCoord = vertex.zw;
    mat4 rotate4 = mat4(
        cos(eulerAngle),-sin(eulerAngle),0.0f,0.0f
        ,sin(eulerAngle),cos(eulerAngle),0.0f,0.0f
        ,0.0f,0.0f,1.0f,0.0f
        ,0.0f,0.0f,0.0f,1.0f
    );

    texCoord = originTextCoord;
    vec4 position = vec4(vertex.xy, 0.0f, 1.0f);
    gl_Position = projection*(position);
}