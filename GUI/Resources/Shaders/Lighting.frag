#version 460

out vec4 out_Color;

in vec2 s_TexCoord;
in vec3 s_Normal;
in vec3 s_FragPos;

uniform vec3 u_CamPos;

layout(binding = 0) uniform sampler2D u_Albedo;
layout(binding = 1) uniform sampler2D u_Normal;
//layout(binding = 2) uniform sampler2D u_Metallic;
//layout(binding = 2) uniform sampler2D u_Specular;
//layout(binding = 2) uniform sampler2D u_Roughness;
//layout(binding = 2) uniform sampler2D u_Emissive;
//layout(binding = 2) uniform sampler2D u_Displacement;
layout(binding = 2) uniform sampler2D u_AO;

const vec3 ambientLight = vec3(0.2, 0.2, 0.2);
//const vec3 diffuseLight = vec3(0.75, 0.75, 0.75);
const vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
//const vec3 specularLight = vec3(0.9, 0.9, 0.9);
const vec3 specularLight = vec3(1.0, 1.0, 1.0);

const vec3 ambientMaterial = vec3(1.0, 1.0, 1.0);
const vec3 diffuseMaterial = vec3(1.0, 1.0, 1.0);
const vec3 specularMaterial = vec3(1.0, 1.0, 1.0);
const float shininessMaterial = 16.0;

const vec3 lightPosition = vec3(0.0, 10.0, 0.0);

void main()
{
    vec4 baseColor = texture(u_Albedo, s_TexCoord);
    vec4 aoColor = texture(u_AO, s_TexCoord);

    // ambient
    vec3 ambient = ambientLight * ambientMaterial;
  	
    // diffuse
    vec3 normalFromMap = texture(u_Normal, s_TexCoord).rgb;
	normalFromMap = normalFromMap * 2.0 - 1.0;
    vec3 norm = normalize(normalize(s_Normal) * normalFromMap);
    vec3 lightDir = normalize(lightPosition - s_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseLight * diff * diffuseMaterial;
    
    // specular
    vec3 viewDir = normalize(u_CamPos - s_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessMaterial);
    vec3 specular = specularLight * (spec * specularMaterial);  
        
    vec3 result = ambient + diffuse + specular;
    out_Color = vec4(result, 1.0) * baseColor * aoColor;

    /*vec4 texColor = texture(tex, o_TexCoord);



	vec3 ambient = light.ambient * material.ambient;



	vec3 norm = normalize(o_Normal);
	vec3 lightDir = normalize(light.position - o_Fragpos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);



	vec3 viewDir = normalize(o_CameraPos - o_Fragpos);
	vec3 reflectDir = reflect(-lightDir, o_Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);



	color = vec4(ambient + diffuse + specular, 1.0) * texColor;*/
}