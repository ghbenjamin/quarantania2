#version 330 core

in vec2 TexCoords;
in vec4 ColourOut;

out vec4 FragColour;

uniform sampler2D image;

void main()
{
    FragColour = texture(image, TexCoords) * ColourOut;
}