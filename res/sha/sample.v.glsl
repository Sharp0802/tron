#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColour;

out vec3 colour;

void main()
{
    colour = vertexColour;
    gl_Position = vec4(vertexPosition, 1.0);
}
