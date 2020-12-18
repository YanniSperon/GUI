#version 460

out vec4 o_Color;

in vec2 s_TextureCoordinate;

layout (binding = 0) uniform sampler2D u_Image;

vec3 textColor = vec3(1.0, 1.0, 1.0);

void main()
{
    float texSample = texture(u_Image, s_TextureCoordinate).r;
    if (texSample > 0.5) {
        o_Color = vec4(textColor, texSample);
    } else {
        o_Color = vec4(1.0, 0.0, 0.0, 0.5);
    }
}