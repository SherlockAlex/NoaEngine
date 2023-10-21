#version 330 core
out vec4 fragColor;		//最终要显示的颜色
in vec2 texCoord;

uniform sampler2D picture;	//采样图片
uniform vec4 tint;

uniform float eulerAngle;//欧拉角

extern vec4 getColor();

void main()
{
	vec4 finalColor = getColor();
    fragColor = finalColor;
}

vec4 getColor(){
    vec4 texColor = texture(picture, texCoord);
	const float invGrayValue = 1.0f/255;
	vec4 finalColor = texColor * tint * invGrayValue;
	return finalColor;
}
