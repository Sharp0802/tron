#version 400 core

in  vec3 colour;
in  vec2 texCoord;

out vec4 fragColour;

uniform sampler2D myTexture;

void main()
{
    fragColour = texture(myTexture, texCoord);
}
