#version 460

layout (location = 0) in vec3 in_VertexPosition;

out vec3 s_FragPos;

uniform mat4 u_P;
uniform mat4 u_V;

void main()
{
    s_FragPos = in_VertexPosition;

    mat4 rotView = mat4(mat3(u_V));
    vec4 clipPos = u_P * rotView * vec4(s_FragPos, 1.0);

    gl_Position = clipPos.xyww;
}