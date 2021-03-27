#version 460

out vec4 o_Color;

in vec2 s_TextureCoordinate;
in vec4 s_Color;
in int s_FontType;

layout (binding = 0) uniform sampler2D u_Image;

void main()
{
    o_Color = vec4(s_Color.rgb, s_Color.a * texture(u_Image, s_TextureCoordinate).r);
}