#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 colourIn;

out vec2 TexCoords;
out vec4 ColourOut;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 localTransform;

void main()
{
    gl_Position = projection * model * localTransform * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
    ColourOut = colourIn;
}