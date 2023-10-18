#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D inputTexture;
uniform vec4 tint;

void main(){
    vec4 textureColor = texture(inputTexture,texCoord);
    float invGrayColor = 1.0/255;
    vec4 finalColor = textureColor * tint*invGrayColor;
    fragColor = finalColor;
}
