#version 330

in vec3 Position;
in vec3 Color;

out vec3 FragColor;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
    FragColor = Color;
}