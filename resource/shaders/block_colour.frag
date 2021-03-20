#version 330 core

in vec2 TexCoords;
in vec4 ColourOut;

out vec4 FragColour;

void main()
{
    FragColour = ColourOut;
}