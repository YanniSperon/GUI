#version 460

layout (location = 0) in vec2 in_VertexPosition;
layout (location = 1) in vec2 in_TextureCoordinate;
layout (location = 2) in vec4 in_Color;
layout (location = 3) in int in_FontType;

out vec2 s_TextureCoordinate;
out vec4 s_Color;
out int s_FontType;

uniform mat4 u_P;
uniform mat4 u_M;

void main()
{
	s_TextureCoordinate = in_TextureCoordinate;
	s_Color = in_Color;
	s_FontType = in_FontType;
	gl_Position = u_P * u_M * vec4(in_VertexPosition, 0.0, 1.0);
}