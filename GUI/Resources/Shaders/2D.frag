#version 460

out vec4 o_Color;

in vec2 s_TextureCoordinate;

in vec2 s_Scale;
in vec2 s_Translation;
in float s_BorderWeight;

uniform vec4 u_TintColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 u_BorderColor = vec4(0.0, 0.0, 0.0, 1.0);
uniform float u_Opacity = 1.0;
uniform float u_CornerRoundness = 0.0625;
uniform float u_ParentCornerRoundness = 0.0625;
uniform vec2 u_ParentScale;
uniform vec2 u_ParentTranslation;

uniform int u_ShouldClipToBounds;

uniform int u_HasImageBackground = 0;
layout (binding = 0) uniform sampler2D u_Image;

float roundRect(vec2 p, vec2 b, float r)
{
    return length(max(abs(p)-b+r,0.0))-r;
}

void main()
{
    vec2 scaleWithoutBorder = s_Scale - (2.0 * s_BorderWeight);

    float aOpacity = smoothstep(0.0, 1.0, 1.0 - roundRect(gl_FragCoord.xy - s_Translation, scaleWithoutBorder * 0.5, min(scaleWithoutBorder.x, scaleWithoutBorder.y) * u_CornerRoundness));
    float bOpacity = smoothstep(0.0, 1.0, 1.0 - roundRect(gl_FragCoord.xy - s_Translation, s_Scale * 0.5, min(s_Scale.x, s_Scale.y) * u_CornerRoundness));

    vec4 finalColor = (aOpacity * (((1 - u_HasImageBackground) * u_TintColor) + (u_HasImageBackground * texture(u_Image, s_TextureCoordinate) * u_TintColor))) + ((1.0 - aOpacity) * bOpacity * u_BorderColor);

    o_Color = (u_ShouldClipToBounds * vec4(finalColor.xyz, finalColor.w * u_Opacity * bOpacity * smoothstep(0.0, 1.0, 1.0 - roundRect(gl_FragCoord.xy - u_ParentTranslation, u_ParentScale * 0.5, min(u_ParentScale.x, u_ParentScale.y) * u_ParentCornerRoundness)))) + ((1 - u_ShouldClipToBounds) * vec4(finalColor.xyz, finalColor.w * u_Opacity * bOpacity));
    
    // EXPANDED VIEW
    /*
    vec2 scaleWithoutBorder = s_Scale - (2.0 * s_BorderWeight);

    // Content
    float a = roundRect(gl_FragCoord.xy - s_Translation, scaleWithoutBorder * 0.5, min(scaleWithoutBorder.x, scaleWithoutBorder.y) * u_CornerRoundness);
    // Border
    float b = roundRect(gl_FragCoord.xy - s_Translation, s_Scale * 0.5, min(s_Scale.x, s_Scale.y) * u_CornerRoundness);
    // Clip To Bounds
    float c = roundRect(gl_FragCoord.xy - u_ParentTranslation, u_ParentScale * 0.5, min(u_ParentScale.x, u_ParentScale.y) * u_ParentCornerRoundness);

    float aOpacity = smoothstep(0.0, 1.0, 1.0 - a);
    float bOpacity = smoothstep(0.0, 1.0, 1.0 - b);
    float cOpacity = smoothstep(0.0, 1.0, 1.0 - c);

    vec4 sampleColor = texture(u_Image, s_TextureCoordinate);
    vec4 finalColor = (aOpacity * (((1 - u_HasImageBackground) * u_TintColor) + (u_HasImageBackground * sampleColor * u_TintColor))) + ((1.0 - aOpacity) * bOpacity * u_BorderColor);

    float clipOpacity = finalColor.w * u_Opacity * bOpacity * cOpacity;
    float nonClipOpacity = finalColor.w * u_Opacity * bOpacity;

    vec4 clippedToBoundsColor = (u_ShouldClipToBounds * vec4(finalColor.xyz, clipOpacity));
    vec4 nonClippedToBoundsColor = ((1 - u_ShouldClipToBounds) * vec4(finalColor.xyz, nonClipOpacity));

    o_Color = clippedToBoundsColor + nonClippedToBoundsColor;
    */
}