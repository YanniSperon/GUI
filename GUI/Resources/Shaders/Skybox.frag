#version 460

out vec4 o_FragColor;
in vec3 s_FragPos;

layout(binding = 0) uniform sampler2D m_EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(s_FragPos));
    vec3 color = texture(m_EquirectangularMap, uv).rgb;
    
    o_FragColor = vec4(color, 1.0);
}
