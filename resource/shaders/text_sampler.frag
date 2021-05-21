#version 330 core

in vec2 TexCoords;
in vec4 ColourOut;

out vec4 FragColour;

uniform sampler2D image;

void main()
{
    vec4 texColour = texture(image, TexCoords);
    FragColour = vec4( ColourOut.rgb, texColour.a * ColourOut.a );
}