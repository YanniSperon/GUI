#version 460

layout (location = 0) in vec2 in_VertexPosition;
layout (location = 1) in vec2 in_TextureCoordinate;

uniform mat4 u_P;
uniform mat4 u_M;

uniform float u_BorderWeight = 0.0;

out vec2 s_TextureCoordinate;
out vec2 s_Scale;
out vec2 s_Translation;
out float s_BorderWeight;

void main()
{
	s_TextureCoordinate = in_TextureCoordinate;

	s_BorderWeight = u_BorderWeight;

	mat4 finalTransformation = u_M;
	finalTransformation[0][0] += u_BorderWeight * 2.0;
	finalTransformation[1][1] += u_BorderWeight * 2.0;

	s_Scale = vec2(finalTransformation[0][0], finalTransformation[1][1]);
	s_Translation = vec2(finalTransformation[3][0], finalTransformation[3][1]);
	gl_Position = u_P * finalTransformation * vec4(in_VertexPosition, 0.0, 1.0);
}