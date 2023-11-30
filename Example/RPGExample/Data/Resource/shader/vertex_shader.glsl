#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoord;

uniform float eulerAngle;
uniform mat4 projection;
uniform vec2 offset;
uniform vec2 size;
uniform bool isFlipX;

void main()
{
    mat2 rotationMatrix = mat2(
        cos(eulerAngle),-sin(eulerAngle)
        ,sin(eulerAngle),cos(eulerAngle)
    );

    vec2 finalPositon = vertex.xy*size*0.5f;
     //图像顶点进行一个旋转
    finalPositon = rotationMatrix*finalPositon;

    finalPositon = finalPositon + offset + 0.5f*vec2(size.x,-size.y);
    vec2 finalTexCoord = vertex.zw;

    if(isFlipX)
    {
        finalTexCoord.x = 1.0f-finalTexCoord.x;
    }

    gl_Position = projection*vec4(finalPositon,0.0f,1.0f);

    texCoord = finalTexCoord;

}