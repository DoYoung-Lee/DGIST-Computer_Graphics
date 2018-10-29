#version 330

uniform float x;
uniform float y;
uniform float rot;

in vec3 Position;
in vec3 Color;

out vec3 FragColor;

void main()
{
    mat2 rot_mat = mat2(cos(rot), -sin(rot), sin(rot), cos(rot));
    vec2 xy = rot_mat * 0.1 * Position.xy + vec2(x,y);
    gl_Position = vec4(xy, Position.z, 1.0);
    FragColor = Color;
}
