#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 tint;

void main()
{
    float invGray = 1.0f/255;
    color = invGray * vec4(tint, 1.0) * texture(image, TexCoords);
}