#version 330 core
out vec4 fragColor;

in Data {
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} data;

uniform sampler2D tex;
uniform vec3 lightPosition;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.2;	
	float specularStrength = 0.5;
	vec3 viewPos = vec3(0, 0, 3);
	
	vec3 objectColor = texture(tex, data.texCoord).rgb;

	vec3 c = texture(tex, data.texCoord).rgb;
    fragColor = vec4(c, 1.0);

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