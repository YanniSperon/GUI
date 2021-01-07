#version 460

out vec4 o_Color;

in vec2 s_TextureCoordinate;

layout (binding = 0) uniform sampler2D u_Image;
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    o_Color = vec4(u_Color.rgb, u_Color.a * texture(u_Image, s_TextureCoordinate).r);
}