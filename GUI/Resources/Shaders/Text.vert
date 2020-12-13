#version 460

layout (location = 0) in vec2 in_VertexPosition;
layout (location = 1) in vec2 in_TextureCoordinate;

out vec2 s_TextureCoordinate;

uniform mat4 u_P;
uniform mat4 u_M;

void main()
{
	s_TextureCoordinate = in_TextureCoordinate;
	gl_Position = u_P * u_M * vec4(in_VertexPosition, 0.0, 1.0);
}