#version 330

uniform mat4 MVP;
uniform float Scale;

in vec3 Position;
in vec3 Color;

out vec3 FragColor;

void main()
{
	gl_Position = MVP * vec4(Position * Scale, 1.0);
    FragColor = Color;
}