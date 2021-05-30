#version 330 core

in vec2 TexCoords;
in vec4 ColourOut;

out vec4 FragColour;

uniform sampler2D image;
uniform float fadeFrac;

void main()
{
    vec4 mixedCol = mix( vec4( 0.0, 0.0, 0.0, 1.0 ), texture(image, TexCoords), fadeFrac );
    FragColour = mixedCol * ColourOut;
}