#version 460

out vec4 o_FragColor;
in vec3 s_FragPos;

layout(binding = 0) uniform samplerCube m_EnvironmentMap;

void main()
{
    vec3 envColor = texture(m_EnvironmentMap, s_FragPos).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    o_FragColor = vec4(envColor, 1.0);
}
