#version 330 core
out vec4 fragColor;

in Data {
	vec3 position;
	vec3 normal;
} data;

uniform vec3 lightPosition;

void main()
{
    vec3 objectColor = vec3(1.0,1.0,1.0);
    vec3 lightColor = vec3(1,1,1);
    float d = length(data.position - lightPosition);
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(data.normal);
    vec3 lightDir = normalize(lightPosition - data.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(0,0,1) - data.position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}
