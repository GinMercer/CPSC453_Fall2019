#version 330 core
out vec4 fragColor;

in Data {
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} data;

uniform sampler2D tex;
uniform vec3 lightPosition; 
uniform vec3 lightColor;
uniform float ambientStrength;	
uniform float specularStrength;
uniform vec3 viewPos;

void main()
{
	vec3 objectColor = vec3(0.74, 0.93, 1.0);
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(data.normal);
	vec3 lightDir = normalize(lightPosition - data.position); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - data.position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
	
}
