#version 460

layout (location = 0) in vec2 in_VertexPosition;
layout (location = 1) in vec2 in_TextureCoordinate;

out vec2 s_TextureCoordinate;

void main()
{
	s_TextureCoordinate = in_TextureCoordinate;
	gl_Position = vec4(in_VertexPosition, 0.0, 1.0);
}